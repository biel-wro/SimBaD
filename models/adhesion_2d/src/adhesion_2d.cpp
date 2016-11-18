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
      : m_particle_ptr(ptr), m_newpos_ptr(newpos_ptr)
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
    if(0 == partialno)
      return m_particle_ptr->position()[dimno];
    else // if(1 == partialno)
      return (*m_newpos_ptr)[dimno];
  }

private:
  cell const *m_particle_ptr;
  cell::position_type const *m_newpos_ptr;
};
}

adhesion_2d::adhesion_2d(simbad::core::property_tree const &pt)
    : adhesion_2d(model_parameters(pt), pt.get<double>("space.tile_size"),
                  pt.get<uint64_t>("seed"))
{
}

adhesion_2d::adhesion_2d(model_parameters mp, double tile_size,
                         std::uint64_t seed)
    : m_parameters(std::move(mp)), m_spacetime(tile_size), rng(seed)
{
}

void adhesion_2d::generate_events(event_visitor v, size_type nevents)
{
  for(size_type i = 0; i < nevents; ++i)
  {
    if(m_spacetime.empty())
      return;
    //print_nicely("entering loop");

    // get next particle
    cell &particle_out = m_spacetime.top_dirty();

    // update global clock
    m_time = particle_out.event_time();

    // extract properties
    double delta_time = particle_out.delta_time();
    velocity_type &velocity = particle_out.velocity();
    acceleration_type &acceleration = particle_out.acceleration();

    velocity = viscosus_velocity(delta_time, velocity);
    velocity += delta_time * acceleration;

    // compute brownian term
    position_type brownian_displacement{
        m_parameters.brownian_displacement(rng, delta_time),
        m_parameters.brownian_displacement(rng, delta_time)};

    // compute new position
    position_type new_position = particle_out.position() +
                                 brownian_displacement +
                                 position_type(delta_time * velocity);

    // create and call visitor
    event_view view(&particle_out, &new_position);
    v(view);

    // create new temparary particle
    cell particle_tmp(new_position, velocity);
    m_spacetime.pop();

    // exclude all interactions
    exclude_all_onesided(particle_tmp);

    // create in new position
    spacetime::dirty_handle_type particle_in_handle =
        m_spacetime.insert_dirty(particle_tmp);

    // include all interaction
    include_all_twosided(*particle_in_handle);

    // resample time & repair ordering
    update_time(*particle_in_handle);
    m_spacetime.repair_order(particle_in_handle);
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
    m_spacetime.insert_dirty(cell(position_type{p.coord(0), p.coord(1)}));
  });

  m_time = 0;
  resample_all();
}
double adhesion_2d::time() const { return m_time; }
adhesion_2d::acceleration_type
adhesion_2d::compute_acceleration(const cell &p1, const cell &p2) const
{
  position_type displacement = p2.position() - p1.position();
  double distance = displacement.hypot();
  double acc_scalar = m_parameters.compute_force(distance);
  double factor = acc_scalar / distance;
  if(std::isnan(factor))
    factor = 0;
  return factor * displacement;
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
void adhesion_2d::update_time(cell &p) const
{
  if(m_parameters.max_time_step() == m_parameters.min_time_step())
  {
    p.delta_time() = m_parameters.min_time_step();
    p.event_time() = time() + p.delta_time();
    return;
  }

  double acc = p.acceleration().hypot();
  double speed = p.velocity().hypot();
  double max_dist = m_parameters.max_jump();
  double diffusion = m_parameters.diffusion();
  // optimally dt would be a solution of the following equation
  // dist == dt^2 * acceleration/2 + dt*speed + diffusion*sqrt(dt)

  assert(acc >= 0);
  assert(speed >= 0);
  assert(max_dist > 0);
  assert(diffusion > 0);

  // make 3 initial estimates
  double t_est0 = std::sqrt(2 * max_dist / acc);
  double t_est1 = max_dist / speed;
  double t_est2 = (max_dist / diffusion) * (max_dist / diffusion);

  // take the smallest out of 3 to get a nicer initial estimate
  double t = std::min(std::min(t_est0, t_est1), t_est2);

  // run Newton's method on the equation for square root of t,
  // for say 10 iterations
  double rt = std::sqrt(t);
  for(int i = 0; i < 10; ++i)
  {
    double t = rt * rt, t2 = t * t;

    double val = acc * t2 / 2 + speed * t + diffusion * rt - max_dist;
    double deriv = 2 * acc * rt * t + 2 * speed * rt + diffusion;
    rt -= val / deriv;
  }
  assert(rt > 0);

  double dt = rt * rt;
  dt = std::min(dt, m_parameters.max_time_step());
  dt = std::max(dt, m_parameters.min_time_step());

  p.delta_time() = dt;
  p.event_time() = time() + p.delta_time();
  assert(p.event_time() >= m_time);
}

void adhesion_2d::exclude(cell &p, const acceleration_type &acc) const
{
  p.acceleration() -= acc;
}
void adhesion_2d::include(cell &p, const acceleration_type &acc) const
{
  p.acceleration() += acc;
}

void adhesion_2d::exclude_all_onesided(cell const &p)
{
  cell::position_type const &pos = p.position();
  double interaction_range = m_parameters.interaction_range();

  auto exclude_particle = [this, &p](cell &neighbor) {
    assert(std::addressof(neighbor) != std::addressof(p));

    acceleration_type acc = compute_acceleration(p, neighbor);
    exclude(neighbor, -acc);
    update_time(neighbor);
  };
  m_spacetime.visit_ball_guarded_order(pos, interaction_range,
                                       exclude_particle);
}

void adhesion_2d::include_all_twosided(cell &p)
{
  position_type const &new_position = p.position();
  double interaction_range = m_parameters.interaction_range();

  auto include_interaction = [this, &p](cell &neighbor) {
    if(std::addressof(p) == std::addressof(neighbor))
      return;
    acceleration_type acc = compute_acceleration(p, neighbor);
    include(p, acc);
    include(neighbor, -acc);
    update_time(neighbor);
  };
  m_spacetime.visit_ball_guarded_order(new_position, interaction_range,
                                       include_interaction);
}
void adhesion_2d::resample_all()
{
  m_spacetime.visit_guarded_order([this](cell &p) {
    position_type const &center = p.position();
    p.acceleration() = acceleration_type(0);
    double radius = m_parameters.interaction_range();
    m_spacetime.visit_ball(
        center, radius, [this, &p](cell const &neighbor) {
          if(std::addressof(p) == std::addressof(neighbor))
            return;
          acceleration_type acc = compute_acceleration(p, neighbor);
          include(p, acc);
        });
    update_time(p);
  });
}

void adhesion_2d::print_nicely(std::string header)
{
  std::cout << header << std::endl;
  m_spacetime.visit([](cell const &p) {
    std::cout << "(" << p.position()[0] << ", " << p.position()[1] << "); "
              << "(" << p.velocity()[0] << ", " << p.velocity()[1] << "); "
              << "(" << p.acceleration()[0] << "," << p.acceleration()[1] << ")"
              << std::endl;
  });
}
END_NAMESPACE_ADHESION_2D

SIMBAD_MAKE_MODEL_FACTORY(adhesion_2d, 2)
