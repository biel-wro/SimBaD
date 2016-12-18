#include "parameter_evolution_3d.hpp"
#include "interface/attribute.hpp"
#include "interface/attribute_mapping.hpp"
#include "interface/event.hpp"
#include "interface/model_factory.hpp"
#include "interface/particle.hpp"
#include "interface/particle.hpp"
#include "utils/attribute_exceptions.hpp"
#include <iostream>
#include <stdexcept>

using simbad::core::EVENT_KIND;

BEGIN_NAMESPACE_PARAMETER_EVOLUTION_3D

parameter_evolution_3d::parameter_evolution_3d(
    const simbad::core::property_tree &pt)
    : m_time(0),
      m_rng(pt.get<uint64_t>("seed")),
      m_spacetime(pt.get<double>("space.tile_size")),
      m_model_params(pt)
{
}

std::size_t parameter_evolution_3d::dimension() const { return 3; }
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

    if(EVENT_KIND::BIRTH == event_kind)
    {
      if(compute_success_rate(c) >= std::uniform_real_distribution<>()(m_rng))
        execute_birth(v);
      else
        execute_death(v);
    }
    else if(EVENT_KIND::DEATH == event_kind)
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

void parameter_evolution_3d::visit_configuration(particle_visitor v) const
{
  m_spacetime.visit([this, v](cell const &p) {
    struct : public simbad::core::particle
    {
      double coord(std::size_t d) const override
      {
        return cell_ptr->position()[d];
      }
      simbad::core::attribute extra_attribute(std::size_t idx) const override
      {
        return self->attribute(*cell_ptr, idx);
      }
      cell const *cell_ptr;
      parameter_evolution_3d const *self;
    } view;
    view.self = this;
    view.cell_ptr = &p;
    v(view);
  });
}

void parameter_evolution_3d::read_configuration(const configuration_view &conf)
{
  std::vector<std::size_t> attribute_indices =
      cell_params::get_attribute_indices(conf.attr_map());

  conf.visit_configuration([&](simbad::core::particle const &p) {
    cell::position_type pos;
    pos[0] = p.coord(0);
    pos[1] = p.coord(1);
    pos[2] = p.coord(2);
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

const simbad::core::attribute_mapping &parameter_evolution_3d::attr_map() const
{
  static std::unique_ptr<simbad::core::attribute_mapping> p_mapping;
  if(nullptr == p_mapping)
  {
    p_mapping.reset(new simbad::core::attribute_mapping);
    using simbad::core::ATTRIBUTE_KIND;
    p_mapping->emplace(0, "density", ATTRIBUTE_KIND::ACCUMULATED);
    p_mapping->emplace(1, "event.time", ATTRIBUTE_KIND::INFO);
    p_mapping->emplace(2, "event.kind", ATTRIBUTE_KIND::INFO);
    p_mapping->emplace(3, "birth.efficiency", ATTRIBUTE_KIND::INTRINSIC);
    p_mapping->emplace(4, "birth.resistance", ATTRIBUTE_KIND::INTRINSIC);
    p_mapping->emplace(5, "lifespan.efficiency", ATTRIBUTE_KIND::INTRINSIC);
    p_mapping->emplace(6, "lifespan.resistance", ATTRIBUTE_KIND::INTRINSIC);
    p_mapping->emplace(7, "success.efficiency", ATTRIBUTE_KIND::INTRINSIC);
    p_mapping->emplace(8, "success.resistance", ATTRIBUTE_KIND::INTRINSIC);
    p_mapping->emplace(9, "birth.rate", ATTRIBUTE_KIND::OBSERVABLE);
    p_mapping->emplace(10, "death.rate", ATTRIBUTE_KIND::OBSERVABLE);
    p_mapping->emplace(11, "success.rate", ATTRIBUTE_KIND::OBSERVABLE);
  }
  return *p_mapping;
}
simbad::core::attribute
parameter_evolution_3d::attribute(const cell &c, std::size_t attr_idx) const
{
  switch(attr_idx)
  {
  case 0: return c.density();
  case 1: return c.event_time();
  case 2: return std::int64_t(c.event_kind());
  case 3: return c.params().birth_eff();
  case 4: return c.params().birth_res();
  case 5: return c.params().lifespan_eff();
  case 6: return c.params().lifespan_res();
  case 7: return c.params().success_eff();
  case 8: return c.params().success_res();
  case 9: return compute_birth_rate(c);
  case 10: return compute_death_rate(c);
  case 11: return compute_success_rate(c);
  }
  throw simbad::core::unrecognized_attribute_number(attr_idx);
}
void parameter_evolution_3d::resample_event(cell &c)
{
  double birth_rate = compute_birth_rate(c);
  double death_rate = compute_death_rate(c);

  double birth_time = std::exponential_distribution<>(birth_rate)(m_rng);
  double death_time = std::exponential_distribution<>(death_rate)(m_rng);

  if(birth_time < death_time)
  {
    c.set_event_kind(EVENT_KIND::BIRTH);
    c.set_event_time(birth_time + time());
  }
  else
  {
    c.set_event_kind(EVENT_KIND::DEATH);
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
  struct : public simbad::core::event
  {
    cell const *cell_ptr;
    double time() const override { return cell_ptr->event_time(); }
    std::size_t dimension() const override { return 3; }
    std::size_t npartials() const override { return 1; }
    EVENT_KIND partial_type(std::size_t p) const override
    {
      assert(p == 0);
      return EVENT_KIND::DEATH;
    }
    double coord(std::size_t partialno, std::size_t dimno) const override
    {
      assert(partialno == 0);
      return cell_ptr->position()[dimno];
    }
  } death_view;

  death_view.cell_ptr = &m_spacetime.top();
  v(death_view);
  pop();
}
void parameter_evolution_3d::execute_birth(event_visitor v)
{
  struct : public simbad::core::event
  {
    cell const *cell_ptr;
    cell::position_type const *newpos_ptr;

    double time() const override { return cell_ptr->event_time(); }
    std::size_t dimension() const override { return 3; }
    std::size_t npartials() const override { return 2; }
    EVENT_KIND partial_type(std::size_t p) const override
    {
      assert(p < 2);
      if(0 == p)
        return EVENT_KIND::BIRTH;
      return EVENT_KIND::NONE;
    }
    double coord(std::size_t partialno, std::size_t dimno) const override
    {
      assert(partialno < 2);
      assert(dimno < 3);
      if(0 == partialno)
        return (*newpos_ptr)[dimno];
      return cell_ptr->position()[dimno];
    }
  } birth_view;

  cell const &parent = m_spacetime.top();
  cell::position_type new_position(parent.position());
  new_position[0] += m_model_params.dispersion()(m_rng);
  new_position[1] += m_model_params.dispersion()(m_rng);
  new_position[2] += m_model_params.dispersion()(m_rng);
  birth_view.cell_ptr = &parent;
  birth_view.newpos_ptr = &new_position;

  v(birth_view);

  cell child(m_spacetime.top());
  child.set_position(new_position);
  child.reset_interaction();

  mutate(child);
  insert(child);

  spacetime::dirty_handle_type parent_handle = m_spacetime.first_dirty();
  mutate(*parent_handle);
  resample_event(*parent_handle);
  m_spacetime.repair_order(parent_handle);
}
END_NAMESPACE_PARAMETER_EVOLUTION_3D

SIMBAD_MAKE_MODEL_FACTORY(parameter_evolution_3d, 3)
