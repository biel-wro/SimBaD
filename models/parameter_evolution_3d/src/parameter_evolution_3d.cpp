#include "parameter_evolution_3d.hpp"
#include "interface/attribute.hpp"
#include "interface/attribute_descriptor.hpp"
#include "interface/attribute_list.hpp"
#include "interface/event.hpp"
#include "interface/model_factory.hpp"
#include "interface/particle.hpp"
#include "interface/particle.hpp"
#include "utils/attribute_converter.hpp"
#include "utils/attribute_exceptions.hpp"
#include <iostream>
#include <stdexcept>

using simbad::core::EVENT_KIND;

BEGIN_NAMESPACE_PARAMETER_EVOLUTION_3D

namespace
{
struct particle_view : public simbad::core::particle
{
  cell const &m_cell;
  parameter_evolution_3d const &m_model;
  particle_view(cell const &c, parameter_evolution_3d const &model)
      : m_cell(c), m_model(model)
  {
  }
  double coord(std::size_t d) const override { return m_cell.position()[d]; }
  simbad::core::attribute extra_attribute(std::size_t idx) const override
  {
    return m_model.attribute(m_cell, idx);
  }
};

struct particle_attr : public simbad::core::attribute_list
{
  cell const &m_cell;
  parameter_evolution_3d const &m_model;
  particle_attr(cell const &c, parameter_evolution_3d const &model)
      : m_cell(c), m_model(model)
  {
  }
  simbad::core::attribute get_attribute(std::size_t idx) const override
  {
    return m_model.new_attribute(m_cell, idx);
  }
};

template <EVENT_KIND EK, std::size_t PARTIALS_LEFT>
struct event_view : public simbad::core::event
{
  particle_view m_particle_view;

  event_view(cell const &c, parameter_evolution_3d const &m)
      : m_particle_view(c, m)
  {
  }

  double time() const override { return m_particle_view.m_model.time(); }
  std::size_t partials_left() const override { return PARTIALS_LEFT; }
  EVENT_KIND event_kind() const override { return EK; }
  simbad::core::particle const &subject() const override
  {
    return m_particle_view;
  }
};
}

parameter_evolution_3d::parameter_evolution_3d(
    const simbad::core::property_tree &pt)
    : m_time(0),
      m_rng(pt.get<uint64_t>("seed")),
      m_spacetime(pt.get<double>("space.tile_size")),
      m_model_params(pt)
{
}

void parameter_evolution_3d::generate_events(event_visitor v, size_type nevents)
{
  using simbad::core::EVENT_KIND;
  for(size_type iter = 0; iter < nevents; ++iter)
  {
    if(m_spacetime.empty())
      return;

    cell const &c = m_spacetime.top();

    assert(m_time <= c.event_time());
    m_time = c.event_time();

    EVENT_KIND event_kind = c.event_kind();

    if(EVENT_KIND::CREATED == event_kind)
    {
      if(compute_success_rate(c) >= std::uniform_real_distribution<>()(m_rng))
        execute_birth(v);
      else
        execute_death(v);
    }
    else if(EVENT_KIND::REMOVED == event_kind)
      execute_death(v);
    else
      assert(false);
  }
}

parameter_evolution_3d::size_type
parameter_evolution_3d::configuration_size() const
{
  return m_spacetime.size();
}

void parameter_evolution_3d::visit_configuration(
    core::configuration_view::particle_visitor v) const
{
  m_spacetime.visit([this, v](cell const &p) {
    particle_attr view(p, *this);
    v(view);
  });
}

void parameter_evolution_3d::read_configuration(const configuration_view &conf)
{
  std::vector<std::size_t> attribute_indices =
      cell_params::get_attribute_indices(conf.new_attr_map());

  std::size_t idx = conf.position_attr_idx();
  conf.visit_configuration([=](particle_attributes const &p) {
    cell::position_type pos;
    pos[0] = p[idx].get_real_ref(0);
    pos[1] = p[idx].get_real_ref(1);
    pos[2] = p[idx].get_real_ref(2);
    insert(cell(pos, cell_params(p, attribute_indices)));
  });
}

double parameter_evolution_3d::time() const { return m_time; }
void parameter_evolution_3d::insert(cell c)
{
  auto visitor = [this, &c](cell &neighbor) {
    neighbor.include_interaction(c, m_model_params);
    resample_event(neighbor);
    c.include_interaction(neighbor, m_model_params);
  };
  double range = m_model_params.interaction().range();
  m_spacetime.visit_ball_guarded_order(c.position(), range, visitor);
  resample_event(c);
  m_spacetime.insert(std::move(c));
}

void parameter_evolution_3d::pop()
{
  cell c = m_spacetime.pop_value();
  auto visitor = [this, &c](cell &neighbor) {
    neighbor.exclude_interaction(c, m_model_params);
    resample_event(neighbor);
  };
  double range = m_model_params.interaction().range();
  m_spacetime.visit_ball_guarded_order(c.position(), range, visitor);
}

const core::attribute_descriptor &parameter_evolution_3d::new_attr_map() const
{
  static std::unique_ptr<simbad::core::attribute_descriptor> map_p;
  if(nullptr == map_p)
  {
    map_p.reset(new simbad::core::attribute_descriptor);
    using simbad::core::ATTRIBUTE_KIND;
    map_p->add_attribute(0, "position", ATTRIBUTE_KIND::POSITION, 3);
    map_p->add_attribute(1, "density", ATTRIBUTE_KIND::ACCUMULATED, 1);
    map_p->add_attribute(2, "event.time", ATTRIBUTE_KIND::INFO, 1);
    map_p->add_attribute(3, "event.kind", ATTRIBUTE_KIND::INFO, 1);
    map_p->add_attribute(4, "birth.efficiency", ATTRIBUTE_KIND::INTRINSIC, 1);
    map_p->add_attribute(5, "birth.resistance", ATTRIBUTE_KIND::INTRINSIC, 1);
    map_p->add_attribute(6, "lifespan.efficiency", ATTRIBUTE_KIND::INTRINSIC,
                         1);
    map_p->add_attribute(7, "lifespan.resistance", ATTRIBUTE_KIND::INTRINSIC,
                         1);
    map_p->add_attribute(8, "success.efficiency", ATTRIBUTE_KIND::INTRINSIC, 1);
    map_p->add_attribute(9, "success.resistance", ATTRIBUTE_KIND::INTRINSIC, 1);
    map_p->add_attribute(10, "birth.rate", ATTRIBUTE_KIND::OBSERVABLE, 1);
    map_p->add_attribute(11, "death.rate", ATTRIBUTE_KIND::OBSERVABLE, 1);
    map_p->add_attribute(12, "success.probability", ATTRIBUTE_KIND::OBSERVABLE,
                         1);
    map_p->add_attribute(13, "lifespan", ATTRIBUTE_KIND::OBSERVABLE, 1);
  }
  return *map_p;
}
void parameter_evolution_3d::check_accumulators()
{
  double range = m_model_params.interaction().range();
  m_spacetime.visit([this, range](cell const &c) {
    double density = 0.0;
    m_spacetime.visit_ball(
        c.position(), range, [this, &c, &density](cell const &neighbor) {
          if(std::addressof(c) == std::addressof(neighbor))
            return;
          double distance = (c.position() - neighbor.position()).hypot();
          density += m_model_params.interaction()(distance);
        });

    std::cout << "stored:" << c.density();
    std::cout << " measured:" << density << std::endl;
  });
}
simbad::core::attribute
parameter_evolution_3d::attribute(const cell &c, std::size_t attr_idx) const
{
  switch(attr_idx)
  {
  case 0:
    return simbad::core::attribute_converter::convert_to<
               simbad::core::coordinates<double, 3>>(c.position())
        .get();
  case 1: return c.density();
  case 2: return c.event_time();
  case 3: return std::int64_t(c.event_kind());
  case 4: return c.params().birth_eff();
  case 5: return c.params().birth_res();
  case 6: return c.params().lifespan_eff();
  case 7: return c.params().lifespan_res();
  case 8: return c.params().success_eff();
  case 9: return c.params().success_res();
  case 10: return compute_birth_rate(c);
  case 11: return compute_death_rate(c);
  case 12: return compute_success_rate(c);
  case 13: return 1.0 / compute_death_rate(c);
  }
  throw simbad::core::unrecognized_attribute_number(attr_idx);
}
core::attribute parameter_evolution_3d::new_attribute(const cell &c,
                                                      std::size_t idx) const
{

  return attribute(c, idx);
}

void parameter_evolution_3d::resample_event(cell &c)
{
  double birth_rate = compute_birth_rate(c);
  double death_rate = compute_death_rate(c);

  double birth_time = std::exponential_distribution<>(birth_rate)(m_rng);
  double death_time = std::exponential_distribution<>(death_rate)(m_rng);

  if(birth_time < death_time)
  {
    c.set_event_kind(EVENT_KIND::CREATED);
    c.set_event_time(birth_time + time());
  }
  else
  {
    c.set_event_kind(EVENT_KIND::REMOVED);
    c.set_event_time(death_time + time());
  }
}
double parameter_evolution_3d::compute_birth_rate(const cell &c) const
{
  cell_params const &cp = c.params();
  double rate =
      m_model_params.birth_rate(c.density(), cp.birth_eff(), cp.birth_res());
  return rate;
}

double parameter_evolution_3d::compute_death_rate(const cell &c) const
{
  cell_params const &cp = c.params();
  double rate = m_model_params.lifespan(c.density(), cp.lifespan_eff(),
                                        cp.lifespan_res());
  return 1.0 / rate;
}

double parameter_evolution_3d::compute_success_rate(const cell &c) const
{
  cell_params const &cp = c.params();
  double r = m_model_params.success_prob(c.density(), cp.success_eff(),
                                         cp.success_res());
  return r;
}

void parameter_evolution_3d::mutate(cell &c)
{
  m_model_params.mutate(c.params(), m_rng);
}

void parameter_evolution_3d::execute_death(event_visitor v)
{
  event_view<EVENT_KIND::REMOVED, 0> death_view(m_spacetime.top(), *this);
  v(death_view);
  pop();
}
void parameter_evolution_3d::execute_birth(event_visitor v)
{
  cell const &parent = m_spacetime.top();
  cell::position_type new_position(parent.position());
  new_position[0] += m_model_params.dispersion()(m_rng);
  new_position[1] += m_model_params.dispersion()(m_rng);
  new_position[2] += m_model_params.dispersion()(m_rng);

  cell child(m_spacetime.top());
  child.set_position(new_position);
  child.reset_interaction();

  mutate(child);
  insert(child);

  spacetime::dirty_handle_type parent_handle = m_spacetime.first_dirty();
  mutate(*parent_handle);
  resample_event(*parent_handle);
  m_spacetime.repair_order(parent_handle);

  event_view<EVENT_KIND::TRANSFORMED, 1> parent_birth_view(parent, *this);
  v(parent_birth_view);

  event_view<EVENT_KIND::CREATED, 0> child_birth_view(child, *this);
  v(child_birth_view);
}
END_NAMESPACE_PARAMETER_EVOLUTION_3D

SIMBAD_MAKE_MODEL_FACTORY(parameter_evolution_3d, 3)
