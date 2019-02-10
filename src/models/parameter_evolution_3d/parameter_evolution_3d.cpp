#include "parameter_evolution_3d.hpp"

#include "intrinsic_params.hpp"

#include "interface/attribute.hpp"
#include "interface/attribute_description.hpp"
#include "interface/attribute_list.hpp"
#include "interface/configuration_view.hpp"
#include "interface/model_factory.hpp"
#include "utils/attribute_converter.hpp"
#include "utils/attribute_exceptions.hpp"

#include <cstddef>
#include <fstream>
#include <iostream>
#include <stdexcept>

using simbad::core::EVENT_KIND;

BEGIN_NAMESPACE_PARAMETER_EVOLUTION_3D

static std::unique_ptr<simbad::core::attribute_description>
make_particle_descriptor()
{
  using KIND = simbad::core::ATTRIBUTE_KIND;
  using SCALAR = simbad::core::ATTRIBUTE_SCALAR;
  SCALAR REAL = SCALAR::REAL;
  SCALAR INT = SCALAR::INT;

  std::unique_ptr<core::attribute_description> description_ptr(
      new core::attribute_description);
  core::attribute_description &desc(*description_ptr);

  desc.add_attribute_auto_idx("position", KIND::PARTICLE_POSITION, REAL, 3);
  desc.add_attribute_auto_idx("density", KIND::ACCUMULATED, REAL, 1);
  desc.add_attribute_auto_idx("next_event.time", KIND::INFO, REAL, 1);
  desc.add_attribute_auto_idx("next_event.kind", KIND::INFO, INT, 1);
  desc.add_attribute_auto_idx("birth.efficiency", KIND::INTRINSIC, REAL, 1);
  desc.add_attribute_auto_idx("birth.resistance", KIND::INTRINSIC, REAL, 1);
  desc.add_attribute_auto_idx("death.efficiency", KIND::INTRINSIC, REAL,
      1);
  desc.add_attribute_auto_idx("death.resistance", KIND::INTRINSIC, REAL, 1);
  desc.add_attribute_auto_idx("success.efficiency", KIND::INTRINSIC, REAL, 1);
  desc.add_attribute_auto_idx("success.resistance", KIND::INTRINSIC, REAL, 1);
  desc.add_attribute_auto_idx("mutation.id", KIND::INTRINSIC, INT, 1);
  desc.add_attribute_auto_idx("birth.rate", KIND::OBSERVABLE, REAL, 1);
  desc.add_attribute_auto_idx("death.rate", KIND::OBSERVABLE, REAL, 1);
  desc.add_attribute_auto_idx("success.probability", KIND::OBSERVABLE, REAL, 1);
  desc.add_attribute_auto_idx("lifespan", KIND::OBSERVABLE, REAL, 1);
#ifdef PARAMETER_EVOLUTION_3D_MUTATION_TREE
  desc.add_attribute_auto_idx("parent.mutation.id", KIND::INFO, INT, 1);
#endif
  return description_ptr;
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
  case 6: return c.params().death_eff();
  case 7: return c.params().death_res();
  case 8: return c.params().success_eff();
  case 9: return c.params().success_res();
  case 10: return c.params().mutation_id();
  case 11: return compute_birth_rate(c);
  case 12: return compute_death_rate(c);
  case 13: return compute_success_rate(c);
  case 14: return 1.0 / compute_death_rate(c);
#ifdef PARAMETER_EVOLUTION_3D_MUTATION_TREE
  case 15:
    return c.params().parent_ptr() ? c.params().parent_ptr()->mutation_id() : 0;
#endif
  }
  throw simbad::core::unrecognized_attribute_number(attr_idx);
}

static std::unique_ptr<simbad::core::attribute_description>
make_event_descriptor()
{
  using KIND = simbad::core::ATTRIBUTE_KIND;
  using SCALAR = simbad::core::ATTRIBUTE_SCALAR;

  std::unique_ptr<simbad::core::attribute_description> map_ptr =
      make_particle_descriptor();

  map_ptr->add_attribute(100, "event.time", KIND::EVENT_TIME, SCALAR::REAL, 1);
  map_ptr->add_attribute(101, "event.delta_time", KIND::EVENT_DELTA_TIME,
                         SCALAR::INT, 1);
  map_ptr->add_attribute(102, "event.kind", KIND::EVENT_KIND, SCALAR::INT, 1);

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
  simbad::core::attribute_description const &description() const override
  {
    static std::unique_ptr<simbad::core::attribute_description> ptr =
        make_particle_descriptor();
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
    if(100 > idx)
      return m_model.particle_attribute(m_cell, idx);

    switch(idx)
    {
    case 100: return m_model.time();
    case 101: return PARTIALS_LEFT;
    case 102: return std::int64_t(to_numeric(EK));
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

parameter_evolution_3d::parameter_evolution_3d(core::property_tree const &pt)
    : m_time(0),
      m_last_mutation_id(0),
      m_rng(pt.get<uint64_t>("seed")),
      m_spacetime(pt.get<double>("space.tile_size")),
      m_model_params(pt),
      m_configurtation_view(new configuration_view(*this))
#ifdef PARAMETER_EVOLUTION_3D_MUTATION_TREE
      ,
      m_tree_dump_path(pt.get("dump.tree_path", std::string())),
      m_stats_dump_path(pt.get("dump.stats_path", std::string())),
      m_parent_mutations_dump_path(
          pt.get("dump.parent_mutations", std::string()))
#endif
{
}

parameter_evolution_3d::~parameter_evolution_3d()
{
#ifdef PARAMETER_EVOLUTION_3D_MUTATION_TREE
  if(!m_tree_dump_path.empty())
    dump_mutation_tree(m_tree_dump_path);
  if(!m_stats_dump_path.empty())
    dump_mutation_stats(m_stats_dump_path);
  if(!m_parent_mutations_dump_path.empty())
    dump_parent_mutations(m_parent_mutations_dump_path);
#endif
}

const core::attribute_description &
parameter_evolution_3d::event_description() const
{
  static std::unique_ptr<simbad::core::attribute_description> ptr =
      make_event_descriptor();
  return *ptr;
}

bool parameter_evolution_3d::generate_events(event_visitor v,
                                             std::size_t nevents)
{
  using simbad::core::EVENT_KIND;
  for(std::size_t iter = 0; iter < nevents; ++iter)
  {
    if(m_spacetime.empty())
      return false;

    cell const &c = m_spacetime.top();

    assert(m_time <= c.event_time());
    m_time = c.event_time();

    EVENT_KIND event_kind = c.event_kind();

    if(EVENT_KIND::CREATED == event_kind)
    {
      if(compute_success_rate(c) >= std::uniform_real_distribution<>()(m_rng))
        execute_division(v);
      else
        execute_death(v);
    }
    else if(EVENT_KIND::REMOVED == event_kind)
      execute_death(v);
    else
      assert(false);
  }
  return true;
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
      cell_params::get_attribute_indices(conf.description());

  std::size_t pos_idx = conf.position_attr_idx();
  std::size_t mut_idx = conf.description()["mutation.id"].attribute_idx();

  conf.visit_records(
      [this, pos_idx, mut_idx, attribute_indices](
          simbad::core::configuration_view::particle_attributes const &p) {
        cell::position_type pos;
        pos[0] = p[pos_idx].get_real_ref(0);
        pos[1] = p[pos_idx].get_real_ref(1);
        pos[2] = p[pos_idx].get_real_ref(2);
        std::shared_ptr<cell_params const> params_ptr =
            std::make_shared<cell_params>(p, attribute_indices);
#ifdef PARAMETER_EVOLUTION_3D_MUTATION_TREE
        m_all_mutations.push_back((params_ptr));
#endif
        insert(cell(pos, params_ptr));
        std::size_t mutation_id = p[mut_idx].get_int_val();
        m_last_mutation_id = std::max(m_last_mutation_id, mutation_id);
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

void parameter_evolution_3d::check_accumulators() const
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

namespace
{
using count_map = std::unordered_map<std::size_t, std::size_t>;
}
#ifdef PARAMETER_EVOLUTION_3D_MUTATION_TREE
std::unordered_map<std::size_t, std::size_t>
parameter_evolution_3d::count_clones() const
{
  count_map counts;
  m_spacetime.visit([&counts](cell const &c) {
    cell_params const &params = c.params();
    std::size_t mutation_id = params.mutation_id();

    count_map::iterator it;
    bool inserted;
    std::tie(it, inserted) = counts.emplace(mutation_id, 1);
    if(inserted)
      return;
    std::size_t &count = it->second;
    ++count;
  });

  return counts;
}

std::unordered_map<std::size_t, std::size_t>
parameter_evolution_3d::count_mutations(
    std::unordered_map<std::size_t, std::size_t> const &clone_counts) const
{
  count_map mutation_counts;
  for(auto id_count_pair : clone_counts)
  {
    std::size_t clone_id = id_count_pair.first;
    std::size_t clone_count = id_count_pair.second;
    count_map::iterator it;
    bool inserted;
    std::tie(it, inserted) = mutation_counts.emplace(clone_id, clone_count);
    if(inserted)
      continue;
    std::size_t &mutation_count = it->second;
    mutation_count += clone_count;
  }
  return mutation_counts;
}

void parameter_evolution_3d::dump_mutation_stats(std::string const &path) const
{
  count_map clone_counts = count_clones();
  count_map mutation_counts = count_mutations(clone_counts);
  std::ofstream output_file(path);
  output_file << "id, count" << std::endl;
  for(auto id_count_pair : clone_counts)
  {
    std::size_t id = id_count_pair.first, clone_count = id_count_pair.second,
                mutation_count = mutation_counts[id];
    output_file << id << "," << clone_count << "," << mutation_count
                << std::endl;
  }
}

void parameter_evolution_3d::dump_mutation_tree(const std::string &path) const
{
  std::vector<std::shared_ptr<cell_params const>> mutations = all_mutations();
  std::ofstream output_file(path);

  std::size_t i = 1;
  output_file << "*vertices " << mutations.size() << std::endl;
  for(std::shared_ptr<cell_params const> const &mutation_ptr : mutations)
  {
    output_file << i << " \"" << mutation_ptr->mutation_id() << "\" "
                << mutation_ptr.use_count() - 2 << std::endl;
    ++i;
  }

  i = 0;
  output_file << "*arcs " << std::endl;
  for(std::shared_ptr<cell_params const> const &mutation_ptr : mutations)
  {
    ++i;
    std::shared_ptr<cell_params const> const &parent_ptr =
        mutation_ptr->parent_ptr();

    if(!parent_ptr)
      continue;

    std::size_t j = 1;
    for(std::shared_ptr<cell_params const> const &search_ptr : mutations)
    {
      if(search_ptr->mutation_id() == parent_ptr->mutation_id())
        break;
      j++;
    }

    assert(i != j);
    output_file << j << " " << i << std::endl;
  }
}

void parameter_evolution_3d::dump_parent_mutations(
    const std::string &path) const
{
  std::vector<std::shared_ptr<cell_params const>> mutations = all_mutations();
  std::ofstream output_file(path);
  for(std::shared_ptr<cell_params const> mutation_ptr : mutations)
  {
    std::size_t mutation_id = mutation_ptr->mutation_id(),
                parent_id = mutation_ptr->parent_ptr()
                                ? mutation_ptr->parent_ptr()->mutation_id()
                                : 0;
    output_file << mutation_id << ", " << parent_id << std::endl;
  }
}
#endif

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
  double rate = m_model_params.death_rate(c.density(), cp.death_eff(),
                                          cp.death_res());
  return rate;
}

double parameter_evolution_3d::compute_success_rate(const cell &c) const
{
  cell_params const &cp = c.params();
  double r = m_model_params.success_prob(c.density(), cp.success_eff(),
                                         cp.success_res());
  return r;
}

std::size_t parameter_evolution_3d::last_mutation_id() const
{
  return m_last_mutation_id;
}

std::size_t parameter_evolution_3d::next_mutation_id()
{
  ++m_last_mutation_id;
  return m_last_mutation_id;
}

void parameter_evolution_3d::mutate(cell &c)
{
  if(!m_model_params.sample_mutation(c, m_rng))
    return;

  std::shared_ptr<cell_params> mutated_params_ptr =
      std::make_shared<cell_params>(c.params());
#ifdef PARAMETER_EVOLUTION_3D_MUTATION_TREE
  m_all_mutations.push_back(mutated_params_ptr);
  mutated_params_ptr->set_parent_ptr(c.params_ptr());
#endif
  mutated_params_ptr->set_mutation_id(next_mutation_id());

  m_model_params.mutate_birth(*mutated_params_ptr, m_rng);
  m_model_params.mutate_death(*mutated_params_ptr, m_rng);
  m_model_params.mutate_success(*mutated_params_ptr, m_rng);

  c.set_params_ptr(mutated_params_ptr);
}

void parameter_evolution_3d::execute_death(event_visitor v)
{
  event_view<EVENT_KIND::REMOVED, 0> death_view(m_spacetime.top(), *this);
  v(death_view);
  pop();
}
void parameter_evolution_3d::execute_division(
    parameter_evolution_3d::event_visitor v)
{
  //  cell const &parent = m_spacetime.top();
  spacetime::dirty_handle_type parent_handle = m_spacetime.first_dirty();

  cell::position_type new_position(parent_handle->position());
  new_position[0] += m_model_params.dispersion()(m_rng);
  new_position[1] += m_model_params.dispersion()(m_rng);
  new_position[2] += m_model_params.dispersion()(m_rng);

  cell child(*parent_handle);
  child.set_position(new_position);
  child.reset_interaction();

  mutate(*parent_handle);
  resample_event(*parent_handle);

  m_spacetime.repair_order(parent_handle);
  event_view<EVENT_KIND::TRANSFORMED, 1> parent_birth_view(*parent_handle,
                                                           *this);
  v(parent_birth_view);

  mutate(child);
  insert(child); // invalidates parent reference

  event_view<EVENT_KIND::CREATED, 0> child_birth_view(child, *this);
  v(child_birth_view);
}
#ifdef PARAMETER_EVOLUTION_3D_MUTATION_TREE
std::vector<std::shared_ptr<cell_params const>>
parameter_evolution_3d::all_mutations() const
{
  return m_all_mutations;
}
#endif
END_NAMESPACE_PARAMETER_EVOLUTION_3D

SIMBAD_MAKE_MODEL_FACTORY(parameter_evolution_3d, 3)
