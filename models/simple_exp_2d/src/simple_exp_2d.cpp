#include "simple_exp_2d.hpp"
#include <assert.h>

#include <boost/property_tree/ptree.hpp>

#include "particle.hpp"
#include "simple_event_queue.hpp"
#include "simple_event_schedule.hpp"

#include "density_accumulator.hpp"
#include "kernel.hpp"

#include "event.hpp"

BEGIN_NAMESPACE_SIMPLE_EXP_2D

simple_exp_2d::simple_exp_2d(dispersion disp, kernel interaction_kernel,
                             saturation_intensity s,
                             std::mt19937_64 random_engine,
                             std::exponential_distribution<> death_distribution,
                             double tile_size)
    : m_time(0),
      m_death_distribution(std::move(death_distribution)),
      m_queue(),
      m_space(config::coord_tiler(tile_size)),
      m_dispersion(std::move(disp)),
      m_interaction_kernel(std::move(interaction_kernel)),
      m_saturation_intensity(std::move(s)),
      m_random_engine(std::move(random_engine))
{
  set_configuration();
}

namespace
{
struct my_death_view : public simbad::core::event
{
  double t, x, y;
  my_death_view(double t, double x, double y) : t(t), x(x), y(y) {}
  double time() const override { return t; }
  std::size_t dimension() const override { return 2; }
  std::size_t npartials() const override { return 1; }
  simbad::core::EVENT_KIND partial_type(std::size_t) const override
  {
    return simbad::core::EVENT_KIND::DEATH;
  }
  double coord(std::size_t, std::size_t nc) const override
  {
    return 0 == nc ? x : y;
  }
};

struct my_birth_view : public simbad::core::event
{
  double t, x, y, x2, y2;
  my_birth_view(double t, double x, double y, double x2, double y2)
      : t(t), x(x), y(y), x2(x2), y2(y2)
  {
  }
  double time() const override { return t; }
  std::size_t dimension() const override { return 2; }
  std::size_t npartials() const override { return 2; }
  simbad::core::EVENT_KIND partial_type(std::size_t pn) const override
  {
    return pn == 0 ? simbad::core::EVENT_KIND::BIRTH
                   : simbad::core::EVENT_KIND::NONE;
  }
  double coord(std::size_t pn, std::size_t nc) const override
  {
    if(0 == pn)
      return 0 == nc ? x : y;
    else
      return 0 == nc ? x2 : y2;
  }
};
}

simple_exp_2d::simple_exp_2d::~simple_exp_2d() {}
void simple_exp_2d::generate_events(core::model::event_visitor visitor,
                                    size_t nevents)
{
  using simbad::core::simple_event_schedule;
  using simbad::core::EVENT_KIND;

  assert(!m_queue.empty());

  for(size_t i = 0; i < nevents; ++i)
  {
    if(m_queue.empty())
    {
      assert(m_space.empty());
      return;
    }

    simple_event_schedule const &ev = m_queue.top();
    update_time(ev.get_time());

    EVENT_KIND event_kind = ev.get_event_kind();

    particle *p = ev.get_particle_ptr_as_nonconst<particle>();

    if(EVENT_KIND::DEATH == event_kind)
    {
      visitor(my_death_view(ev.get_time(), p->coords()[0], p->coords()[1]));
      remove_particle(*p);
    }
    else if(EVENT_KIND::BIRTH == event_kind)
    {
      float birth_x = p->coords()[0] + m_dispersion(m_random_engine);
      float birth_y = p->coords()[1] + m_dispersion(m_random_engine);
      visitor(my_birth_view(ev.get_time(), birth_x, birth_y, p->coords()[0],
                            p->coords()[1]));
      config::space_coords birth_coords{birth_x, birth_y};
      add_particle(birth_coords);
      resample_event(*p);
    }
  }
}
namespace
{
struct my_particle_view : public simbad::core::particle
{
  config::space_coords sc;
  my_particle_view(config::space_coords sc) : sc(sc) {}
  std::size_t dimension() const override { return 2; }
  double coord(std::size_t d) const override { return sc[d]; }
};
}

std::size_t simple_exp_2d::configuration_size() const { return m_space.size(); }
void simple_exp_2d::visit_configuration(
    core::model::particle_visitor visitor) const
{
  m_space.visit([this, visitor](particle const &p) {
    my_particle_view particle_view(p.coords());
    visitor(particle_view);
  });
}

void simple_exp_2d::set_configuration()
{
  m_space.clear();
  m_queue.clear();

  config::space_coords coords{0, 0};
  auto it = m_space.emplace(coords, coords);
  full_update_particle(*it);
}

double simple_exp_2d::get_time() const { return m_time; }
void simple_exp_2d::update_time(double new_time) { m_time = new_time; }
void simple_exp_2d::reset_queue()
{
  m_queue.clear();

  auto visitor = [this](particle &p) { full_update_particle(p); };

  m_space.visit(visitor);
}

void simple_exp_2d::full_update_particle(particle_type &p)
{
  p.get_density_accumulator().reset();

  double range = m_interaction_kernel.range();

  config::space_coords cmin = p.coords() - range;
  config::space_coords cmax = p.coords() + range;
  auto visitor = [this, &p](particle_type const &other_particle) {
    lazy_update_particle_include(p, other_particle);
  };

  m_space.visit_box(cmin, cmax, visitor);
  resample_event(p);
}

void simple_exp_2d::lazy_update_particle_exclude(
    simple_exp_2d::particle_type &p, const simple_exp_2d::particle_type &o)
{
  p.get_density_accumulator().exclude(m_interaction_kernel, p, o);
}

void simple_exp_2d::lazy_update_particle_include(
    simple_exp_2d::particle_type &p, const simple_exp_2d::particle_type &o)
{
  p.get_density_accumulator().include(m_interaction_kernel, p, o);
}

void simple_exp_2d::remove_particle(simple_exp_2d::particle_type &p)
{
  double range = m_interaction_kernel.range();
  config::space_coords neigh_cmin = p.coords() - range;
  config::space_coords neigh_cmax = p.coords() + range;

  particle_type const &cp = p;

  m_space.visit_box(neigh_cmin, neigh_cmax, [this, &cp](particle &neighbour) {
    lazy_update_particle_exclude(neighbour, cp);
    resample_event(neighbour);
  });

  particle_type::queue_handle_type handle = p.get_handle();
  m_space.remove(p.coords(), p);
  m_queue.erase(handle);
}

simple_exp_2d::particle_type &
simple_exp_2d::add_particle(config::space_coords coords)
{
  particle_type &p = *m_space.emplace(coords, coords);

  double range = m_interaction_kernel.range();

  auto visitor = [this, &p](particle &neighbour) {
    lazy_update_particle_include(p, neighbour);
    lazy_update_particle_include(neighbour, p);
    resample_event(neighbour);
  };

  m_space.visit_box(coords - range, coords + range, visitor);

  resample_event(p);
  return p;
}

void simple_exp_2d::resample_event(simple_exp_2d::particle_type &p)
{
  double density = p.get_density_accumulator().current_value();
  double birth_rate = m_saturation_intensity(density);

  std::exponential_distribution<> birth_distribution(birth_rate);
  double birth_time = birth_distribution(m_random_engine);
  double death_time = m_death_distribution(m_random_engine);

  using simbad::core::EVENT_KIND;
  EVENT_KIND event_kind;
  double time;

  if(birth_time <= death_time)
  {
    event_kind = simbad::core::EVENT_KIND::BIRTH;
    time = get_time() + birth_time;
  }
  else
  {
    event_kind = simbad::core::EVENT_KIND::DEATH;
    time = get_time() + death_time;
  }

  particle::queue_handle_type handle = p.get_handle();
  using simbad::core::simple_event_schedule;

  if(handle != particle::queue_handle_type(nullptr))
  {
    handle.operator*().set_time(time);
    handle.operator*().set_event_kind(event_kind);
    m_queue.update(handle);
  }
  else
  {
    handle = m_queue.emplace(time, event_kind, &p);
    p.set_handle(handle);
    p.force_listener_update();
  }
}

END_NAMESPACE_SIMPLE_EXP_2D
