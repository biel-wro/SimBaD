#include "parameter_evolution_3d.hpp"

#include <cstddef>
#include <iostream>
#include <stdexcept>

#include "interface/attribute.hpp"
#include "interface/attribute_description.hpp"
#include "interface/attribute_list.hpp"
#include "interface/configuration_view.hpp"
#include "interface/model_factory.hpp"
#include "utils/attribute_converter.hpp"
#include "utils/attribute_exceptions.hpp"

using simbad::core::EVENT_KIND;

BEGIN_NAMESPACE_PARAMETER_EVOLUTION_3D

static std::unique_ptr<simbad::core::attribute_description>
make_attribute_descriptor()
{
  using KIND = simbad::core::ATTRIBUTE_KIND;
  using SCALAR = simbad::core::ATTRIBUTE_SCALAR;
  using rec = simbad::core::attribute_descriptor;

  return std::unique_ptr<simbad::core::attribute_description>{

      new simbad::core::attribute_description{
          rec{0, "position", KIND::POSITION, SCALAR::REAL, 3},
          rec{1, "density", KIND::ACCUMULATED, SCALAR::REAL, 1},
          rec{2, "event.time", KIND::INFO, SCALAR::REAL, 1},
          rec{3, "event.kind", KIND::INFO, SCALAR::INT, 1},
          rec{4, "birth.efficiency", KIND::INTRINSIC, SCALAR::REAL, 1},
          rec{5, "birth.resistance", KIND::INTRINSIC, SCALAR::REAL, 1},
          rec{6, "lifespan.efficiency", KIND::INTRINSIC, SCALAR::REAL, 1},
          rec{7, "lifespan.resistance", KIND::INTRINSIC, SCALAR::REAL, 1},
          rec{8, "success.efficiency", KIND::INTRINSIC, SCALAR::REAL, 1},
          rec{9, "success.resistance", KIND::INTRINSIC, SCALAR::REAL, 1},
          rec{10, "birth.rate", KIND::OBSERVABLE, SCALAR::REAL, 1},
          rec{11, "death.rate", KIND::OBSERVABLE, SCALAR::REAL, 1},
          rec{12, "success.probability", KIND::OBSERVABLE, SCALAR::REAL, 1},
          rec{13, "lifespan", KIND::OBSERVABLE, SCALAR::REAL, 1},
      }};
}

static std::unique_ptr<simbad::core::attribute_description>
make_event_descriptor()
{
  using KIND = simbad::core::ATTRIBUTE_KIND;
  using SCALAR = simbad::core::ATTRIBUTE_SCALAR;

  std::unique_ptr<simbad::core::attribute_description> map_ptr =
      make_attribute_descriptor();

  map_ptr->add_attribute(15, "time", KIND::TIME, SCALAR::REAL, 1);
  map_ptr->add_attribute(16, "delta time", KIND::TIME, SCALAR::INT, 1);
  map_ptr->add_attribute(17, "event", KIND::EVENT_KIND, SCALAR::INT, 1);

  return map_ptr;
}

namespace
{
struct particle_view : public simbad::core::attribute_list
{
  particle_view(cell const &c, parameter_evolution_3d const &m)
      : m_cell(c), m_model(m)
  {
  }
  cell const &get_cell() const { return m_cell; }
  parameter_evolution_3d const &model() const { return m_model; }
  simbad::core::attribute get_attribute(std::size_t attr_idx) const override
  {
    return m_model.particle_attribute(m_cell, attr_idx);
  }

private:
  cell const &m_cell;
  parameter_evolution_3d const &m_model;
};

class configuration_view : public simbad::core::configuration_view
{
public:
  configuration_view(parameter_evolution_3d const &model) : m_model(model) {}
  std::size_t size() const override { return m_model.particle_count(); }
  void visit_records(record_visitor visitor) const override
  {
    m_model.current_spacetime().visit([this, visitor](cell const &p) {
      particle_view view(p, m_model);
      visitor(view);
    });
  }
  simbad::core::attribute_description const &descriptor() const override
  {
    static std::unique_ptr<simbad::core::attribute_description> ptr =
        make_attribute_descriptor();
    return *ptr;
  }

private:
  parameter_evolution_3d const &m_model;
};

template <EVENT_KIND EK, std::int64_t PARTIALS_LEFT>
struct event_view : public simbad::core::attribute_list
{
  event_view(cell const &c, parameter_evolution_3d const &m)
      : m_cell(c), m_model(m)
  {
  }
  simbad::core::attribute get_attribute(std::size_t idx) const override
  {
    if(15 > idx)
      return m_model.particle_attribute(m_cell, idx);

    switch(idx)
    {
    case 15: return m_model.time();
    case 16: return PARTIALS_LEFT;
    case 17: return std::int64_t(to_numeric(EK));
    default:
      throw std::invalid_argument("attribute index " + std::to_string(idx) +
                                  " is not recognized");
    }
  }

private:
  cell const &m_cell;
  parameter_evolution_3d const &m_model;
};

} // namespace

parameter_evolution_3d::parameter_evolution_3d(
    const simbad::core::property_tree &pt)
    : m_time(0),
      m_rng(pt.get<uint64_t>("seed")),
      m_spacetime(pt.get<double>("space.tile_size")),
      m_model_params(pt),
      m_configurtation_view(new configuration_view(*this))
{
}

const core::attribute_description &
parameter_evolution_3d::event_descriptor() const
{
  static std::unique_ptr<simbad::core::attribute_description> ptr =
      make_event_descriptor();
  return *ptr;
}

void parameter_evolution_3d::generate_events(event_visitor v,
                                             std::size_t nevents)
{
  using simbad::core::EVENT_KIND;
  for(std::size_t iter = 0; iter < nevents; ++iter)
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

const core::configuration_view &
parameter_evolution_3d::current_configuration() const
{
  return *m_configurtation_view;
}

void parameter_evolution_3d::read_configuration(
    const simbad::core::configuration_view &conf)
{
  std::vector<std::size_t> attribute_indices =
      cell_params::get_attribute_indices(conf.descriptor());

  std::size_t idx = conf.position_attr_idx();
  conf.visit_records(
      [=](simbad::core::configuration_view::particle_attributes const &p) {
        cell::position_type pos;
        pos[0] = p[idx].get_real_ref(0);
        pos[1] = p[idx].get_real_ref(1);
        pos[2] = p[idx].get_real_ref(2);
        insert(cell(pos, cell_params(p, attribute_indices)));
      });
}

double parameter_evolution_3d::time() const { return m_time; }
std::size_t parameter_evolution_3d::particle_count() const
{
  return m_spacetime.size();
}
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

simbad::core::attribute
parameter_evolution_3d::particle_attribute(const cell &c,
                                           std::size_t attr_idx) const
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

const spacetime &parameter_evolution_3d::current_spacetime() const
{
  return m_spacetime;
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
void parameter_evolution_3d::execute_birth(
    parameter_evolution_3d::event_visitor v)
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
