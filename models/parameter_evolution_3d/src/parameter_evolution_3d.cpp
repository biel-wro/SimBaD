#include "parameter_evolution_3d.hpp"
#include "interface/event.hpp"
#include "interface/particle.hpp"
#include "model_factory.hpp"
#include "particle.hpp"
#include <stdexcept>

using simbad::core::EVENT_KIND;

BEGIN_NAMESPACE_PARAMETER_EVOLUTION_3D
/*
std::string parameter_evolution_3d::model_name()
{
  return "parameter_evolution_3d";
}
std::size_t parameter_evolution_3d::s_dimension() { return 3; }
*/
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
      if(compute_success_rate(c) < std::uniform_real_distribution<>()(m_rng))
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
  m_spacetime.visit([v](cell const &p) {
    struct : public simbad::core::particle
    {
      std::size_t dimension() const override { return 3; }
      double coord(std::size_t d) const override
      {
        return parent_ptr->position()[d];
      }
      cell const *parent_ptr;
    } view;
    view.parent_ptr = &p;
    v(view);
  });
}

void parameter_evolution_3d::read_configuration(
    const configuration_view &conf,
    simbad::core::property_tree const &default_properties)
{
  conf.visit_configuration(
      [this, &default_properties](simbad::core::particle const &p) {
        cell::position_type pos;
        pos[0] = p.coord(0);
        pos[1] = p.coord(1);
        pos[2] = p.coord(2);
        insert(cell(pos, default_properties));
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
    cell const *parent_ptr;
    double time() const override { return parent_ptr->event_time(); }
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
      return parent_ptr->position()[dimno];
    }
  } death_view;

  death_view.parent_ptr = &m_spacetime.top();
  v(death_view);
  pop();
}
void parameter_evolution_3d::execute_birth(event_visitor v)
{
  struct : public simbad::core::event
  {
    cell const *parent_ptr;
    cell::position_type const *newpos_ptr;

    double time() const override { return parent_ptr->event_time(); }
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
      return parent_ptr->position()[dimno];
    }
  } birth_view;

  cell const &parent = m_spacetime.top();
  cell::position_type new_position(parent.position());
  new_position[0] += m_model_params.dispersion()(m_rng);
  new_position[1] += m_model_params.dispersion()(m_rng);
  new_position[2] += m_model_params.dispersion()(m_rng);
  birth_view.parent_ptr = &parent;
  birth_view.newpos_ptr = &new_position;

  v(birth_view);

  cell child(m_spacetime.top());

  mutate(child);
  insert(child);

  spacetime::dirty_handle_type parent_handle = m_spacetime.first_dirty();
  mutate(*parent_handle);
  resample_event(*parent_handle);
  m_spacetime.repair_order(parent_handle);
}
END_NAMESPACE_PARAMETER_EVOLUTION_3D

SIMBAD_MAKE_MODEL_FACTORY(parameter_evolution_3d, 3)
