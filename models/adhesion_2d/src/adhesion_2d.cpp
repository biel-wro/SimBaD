#include "adhesion_2d.hpp"
#include "core_fwd.hpp"

#include "interface/event.hpp"
#include "interface/event_kind.hpp"
#include "interface/model_factory.hpp"

#include <cassert>
#include <iostream>
#include <stdexcept>
using namespace simbad::models::adhesion_2d;
using simbad::core::property_tree;
using simbad::core::EVENT_KIND;

BEGIN_NAMESPACE_CORE
template <>
adhesion_2d *create_from_property_tree<adhesion_2d>(const property_tree &pt)
{
  return new adhesion_2d(pt);
}
template adhesion_2d *
create_from_property_tree<adhesion_2d>(property_tree const &);
END_NAMESPACE_CORE

BEGIN_NAMESPACE_ADHESION_2D

namespace
{
template <EVENT_KIND EK, std::size_t PL>
class event_view : public simbad::core::event
{
public:
  explicit event_view(cell const &c, double t) : m_paricle_view(c), m_time(t) {}
  double time() const override { return m_time; }
  std::size_t partials_left() const override { return PL; }
  EVENT_KIND event_kind() const override { return EK; }
  simbad::core::particle const &subject() const override
  {
    return m_paricle_view;
  }

private:
  particle_view m_paricle_view;
  double m_time;
};
}

adhesion_2d::adhesion_2d(simbad::core::property_tree const &pt)
    : adhesion_2d(model_parameters(pt), pt.get<double>("space.tile_size"),
                  pt.get<uint64_t>("seed"))
{
}

adhesion_2d::adhesion_2d(model_parameters mp, double tile_size,
                         std::uint64_t seed)
    : m_time(0.0),
      m_parameters(std::move(mp)),
      m_spacetime(tile_size),
      rng(seed)
{
}

void adhesion_2d::generate_events(event_visitor v, size_type nevents)
{
  for(size_type i = 0; i < nevents; ++i)
  {
    if(m_spacetime.empty())
      return;

    cell const &old_particle = m_spacetime.top();
    // update time
    m_time = old_particle.next_jump_time();

    // extract properties
    double delta_time = old_particle.delta_time();
    position_type old_pos = old_particle.position();

    // compute brownian term
    position_type brownian_displacement{
        m_parameters.brownian_displacement(rng, delta_time),
        m_parameters.brownian_displacement(rng, delta_time)};

    // compute new position
    position_type new_pos = old_pos + brownian_displacement +
                            position_type(delta_time * old_particle.velocity());

    // create jump_out event and call visitor
    v(event_view<EVENT_KIND::JUMPED_OUT, 1>(old_particle, time()));

    // compute forces and update neighbor
    force_type total_force(0);
    pressure_type total_pressure(0);

    double radius = m_parameters.interaction_range();
    m_spacetime.visit_double_ball_guarded_order(
        old_pos, new_pos, radius, [&](cell &neighbor) {

          if(std::addressof(old_particle) == std::addressof(neighbor))
            return;
          position_type const &neigh_pos = neighbor.position();
          force_type old_force, new_force;
          pressure_type old_pressure, new_pressure;
          std::tie(old_force, old_pressure) = compute_force(old_pos, neigh_pos);
          std::tie(new_force, new_pressure) = compute_force(new_pos, neigh_pos);

          total_force += new_force;
          total_pressure += new_pressure;

          neighbor.force() -= new_force - old_force;
          neighbor.pressure() += new_pressure - old_pressure;

          update_time(neighbor, false);

        });

    // TODO: move past update
    // create jump_in event
    v(event_view<EVENT_KIND::JUMPED_IN, 0>(m_spacetime.top(), time()));

    // move particle
    m_spacetime.visit_top_guarded([&](cell &c) {
      c.position() = new_pos;
      c.force() = total_force;
      c.pressure() = total_pressure;
      update_velocity(c, c.delta_time());
      update_time(c, true);
    });
  }
}

adhesion_2d::size_type adhesion_2d::dimension() const { return 2; }
adhesion_2d::size_type adhesion_2d::configuration_size() const
{
  return m_spacetime.size();
}

void adhesion_2d::visit_configuration(particle_visitor v) const
{
  m_spacetime.visit([v](const cell &p) { v(particle_view(p)); });
}

void adhesion_2d::read_configuration(const configuration_view &configuration)
{
  assert(configuration.dimension() == dimension());

  m_spacetime.clear();

  configuration.visit_configuration([this](simbad::core::particle const &p) {
    // assert(dimension() == p.dimension());

    position_type position{p.coord(0), p.coord(1)};
    velocity_type velocity(0);
    // push_particle(cell(position, velocity, time_type(0), time_type(0)));
  });
}
double adhesion_2d::time() const { return m_time; }
double adhesion_2d::optimal_time_step(cell const &p) const
{
  double acceleration_scalar =
      m_parameters.acceleration_with_friction(p.force().hypot());
  double speed = p.velocity().hypot();

  double max_dist = m_parameters.max_jump();
  double diffusion = m_parameters.diffusion();
  double viscosity = m_parameters.viscosity();

  // optimally dt would be a solution of the following equation
  // dist == dt*speed + diffusion*sqrt(dt)

  assert(speed >= 0);
  assert(max_dist > 0);
  assert(diffusion > 0);

  double speed_limit = acceleration_scalar / viscosity;
  double max_speed = std::max(speed_limit, speed);

  double delta_sqrt =
      std::sqrt(diffusion * diffusion + 4 * max_dist * max_speed);
  double rt = (delta_sqrt - max_dist) / (2 * max_speed);

  if(0 == max_speed || !std::isfinite(rt))
    rt = max_dist / diffusion;

  assert(rt > 0);

  double dt = rt * rt;
  return dt;
}

void adhesion_2d::update_time(cell &p, bool new_timestep) const
{
  time_type dt = optimal_time_step(p);
  dt = std::max(m_parameters.min_time_step(), dt);
  dt = std::min(m_parameters.max_time_step(), dt);

  if(new_timestep)
  {
    assert(p.next_jump_time() == time());
    p.prev_jump_time() = p.next_jump_time();
    p.next_jump_time() = time() + dt;
  }
  else
  {
    time_type new_time = p.prev_jump_time() + dt;
    p.next_jump_time() = std::max(new_time, time());
  }
}

void adhesion_2d::update_velocity(cell &p, double dt) const
{
  force_type force = p.force();
  velocity_type &v = p.velocity();
  v += force * dt;
  v = viscosus_velocity(dt, v);
}

void adhesion_2d::print_nicely(std::string header)
{
  std::cout << header << std::endl;
  m_spacetime.visit([](cell const &p) {
    std::cout << "(" << p.position()[0] << ", " << p.position()[1] << "); "
              << "(" << p.velocity()[0] << ", " << p.velocity()[1] << "); "
              << "dt=" << p.delta_time() << "; t=" << p.next_jump_time()
              << std::endl;
  });
}

std::pair<adhesion_2d::force_type, adhesion_2d::pressure_type>
adhesion_2d::compute_force(const position_type &target_pos,
                           const position_type &other_pos) const
{
  position_type displacement = target_pos - other_pos;
  double distance = displacement.hypot();
  pressure_type force_scalar = -m_parameters.compute_force(distance);

  double factor = force_scalar / distance;
  if(std::isnan(factor))
    factor = 0;
  force_type force = factor * displacement;
  return std::make_pair(force, force_scalar);
}

adhesion_2d::velocity_type
adhesion_2d::viscosus_velocity(double dt, adhesion_2d::velocity_type v) const
{
  double old_speed = v.hypot();
  double new_speed = m_parameters.viscosus_speed(dt, old_speed);
  double factor = new_speed / old_speed;
  if(std::isnan(factor))
    factor = 0;

  return v * factor;
}

END_NAMESPACE_ADHESION_2D

SIMBAD_MAKE_MODEL_FACTORY(adhesion_2d, 2)
