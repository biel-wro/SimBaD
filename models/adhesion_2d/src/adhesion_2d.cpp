#include "adhesion_2d.hpp"
#include "core_fwd.hpp"

#include "event_kind.hpp"
#include "interface/event.hpp"
#include "model_factory.hpp"

#include <cassert>
#include <iostream>
#include <stdexcept>
using namespace simbad::models::adhesion_2d;
using simbad::core::property_tree;

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
class event_view : public simbad::core::event
{
public:
  explicit event_view(cell const *ptr = nullptr,
                      cell::position_type const *newpos_ptr = nullptr)
      : m_particle_ptr(ptr), m_oldpos_ptr(newpos_ptr)
  {
  }
  double time() const override { return m_particle_ptr->event_time(); }
  std::size_t dimension() const override { return cell::dimension; }
  std::size_t npartials() const override { return 2; }
  core::EVENT_KIND partial_type(std::size_t partialno) const override
  {
    assert(partialno < 2);
    if(0 == partialno)
      return simbad::core::EVENT_KIND::DEATH;
    else // if(1 == partialno)
      return simbad::core::EVENT_KIND::BIRTH;
  }
  double coord(std::size_t partialno, std::size_t dimno) const override
  {
    assert(partialno < 2);
    assert(dimno < dimension());
    if(0 == partialno)
      return (*m_oldpos_ptr)[dimno];
    else // if(1 == partialno)
      return m_particle_ptr->position()[dimno];
  }

private:
  cell const *m_particle_ptr;
  cell::position_type const *m_oldpos_ptr;
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

    cell particle = pop_particle();

    m_time = particle.event_time();

    // extract properties
    double delta_time = particle.delta_time();
    position_type old_position = particle.position();

    particle.velocity() = viscosus_velocity(delta_time, particle.velocity());

    // compute brownian term
    position_type brownian_displacement{
        m_parameters.brownian_displacement(rng, delta_time),
        m_parameters.brownian_displacement(rng, delta_time)};

    // compute displacement
    position_type displacement =
        position_type(delta_time * particle.velocity());

    // compute new position
    particle.position() += brownian_displacement + displacement;

    // create and call visitor
    {
      event_view view(&particle, &old_position);
      v(view);
    }

    push_particle(particle);
  }
}

adhesion_2d::size_type adhesion_2d::dimension() const { return 2; }
adhesion_2d::size_type adhesion_2d::configuration_size() const
{
  return m_spacetime.size();
}

void adhesion_2d::visit_configuration(particle_visitor v) const
{
  particle_view view;
  m_spacetime.visit([this, v, &view](const cell &p) {
    view.set_orig(&p);
    v(view);
  });
}

void adhesion_2d::read_configuration(const configuration_view &configuration)
{
  assert(configuration.dimension() == dimension());

  m_spacetime.clear();

  configuration.visit_configuration([this](simbad::core::particle const &p) {
    assert(dimension() == p.dimension());

    position_type position{p.coord(0), p.coord(1)};
    velocity_type velocity(0);
    push_particle(cell(position, velocity, time_type(0), time_type(0)));
  });
}
double adhesion_2d::time() const { return m_time; }
adhesion_2d::acceleration_type
adhesion_2d::compute_acceleration(const cell &p1, const cell &p2) const
{
  position_type displacement = p2.position() - p1.position();
  double distance = displacement.hypot();
  double acc_scalar = -m_parameters.compute_force(distance);
  double factor = acc_scalar / distance;
  if(std::isnan(factor))
    factor = 0;
  acceleration_type acc = factor * displacement;
  return acc;
}

adhesion_2d::acceleration_type
adhesion_2d::compute_acceleration(const cell &p) const
{
  position_type const &position = p.position();
  double interaction_range = m_parameters.interaction_range();

  acceleration_type acc(0);

  m_spacetime.visit_ball(position, interaction_range,
                         [this, &p, &acc](cell const &neighbor) {
                           if(std::addressof(p) == std::addressof(neighbor))
                             return;
                           acc += compute_acceleration(p, neighbor);
                         });
  return acc;
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

cell adhesion_2d::pop_particle()
{
  cell p = m_spacetime.top();
  m_spacetime.pop();
  return p;
}

void adhesion_2d::push_particle(cell particle_tmp)
{
  update_velocity(particle_tmp, particle_tmp.delta_time());
  update_time(particle_tmp);

  m_spacetime.insert(particle_tmp);
}

double adhesion_2d::optimal_time_step(cell const &p) const
{
  // double acc = p.acceleration().hypot();
  double speed = p.velocity().hypot();
  double max_dist = m_parameters.max_jump();
  double diffusion = m_parameters.diffusion();

  // optimally dt would be a solution of the following equation
  // dist == dt*speed + diffusion*sqrt(dt)

  assert(speed >= 0);
  assert(max_dist > 0);
  assert(diffusion > 0);

  double delta_sqrt = std::sqrt(diffusion * diffusion + 4 * max_dist * speed);
  double rt = (delta_sqrt - max_dist) / (2 * speed);

  if(0 == speed || !std::isfinite(rt))
    rt = max_dist / diffusion;

  assert(rt > 0);

  double dt = rt * rt;
  return dt;
}

void adhesion_2d::update_time(cell &p) const
{
  time_type dt = optimal_time_step(p);
  dt = std::max(m_parameters.min_time_step(), dt);
  dt = std::min(m_parameters.max_time_step(), dt);

  p.delta_time() = dt;
  p.event_time() = time() + dt;
}

void adhesion_2d::update_velocity(cell &p, double dt) const
{
  acceleration_type acc = compute_acceleration(p);
  p.velocity() += acc * dt;
}

void adhesion_2d::print_nicely(std::string header)
{
  std::cout << header << std::endl;
  m_spacetime.visit([](cell const &p) {
    std::cout << "(" << p.position()[0] << ", " << p.position()[1] << "); "
              << "(" << p.velocity()[0] << ", " << p.velocity()[1] << "); "
              << "dt=" << p.delta_time() << "; t=" << p.event_time()
              //          << "(" << p.acceleration()[0] << "," <<
              //          p.acceleration()[1] << ")"
              << std::endl;
  });
}
END_NAMESPACE_ADHESION_2D

SIMBAD_MAKE_MODEL_FACTORY(adhesion_2d, 2)
