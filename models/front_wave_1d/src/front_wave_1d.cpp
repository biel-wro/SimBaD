#include "front_wave_1d.hpp"

#include "front_wave_1d_impl.hpp"
#include "legacy/simple/simple_event_view.hpp"
#include "legacy/simple/simple_particle_view.hpp"

BEGIN_NAMESPACE_FRONT_WAVE_1D
front_wave_1d::front_wave_1d(double alpha, double x0 = 1.0)
    : impl(new front_wave_1d_impl(alpha, x0))
{
}

namespace
{
struct particle_view : public simbad::core::particle
{
  double pos;
  particle_view(double c) : pos(c) {}
  double coord(std::size_t d) const override
  {
    assert(0 == d);
    return pos;
  }
};

struct event_view : public simbad::core::event
{
  using base_type = front_wave_1d_impl::Event;
  base_type const &base;
  particle_view p;

  event_view(base_type const &base) : base(base), p(base.coordinate(0)) {}
  double time() const override { return base.time(); }
  std::size_t partials_left() const override { return 0; }
  simbad::core::EVENT_KIND event_kind() const override
  {
    return simbad::core::EVENT_KIND::CREATED;
  }
  simbad::core::particle const &subject() const override { return p; }
};
}

void front_wave_1d::generate_events(event_visitor visitor, size_t nevents)
{
  //CORE_NAMESPACE::simple_event_view<front_wave_1d_impl::Event> event_view;

  for(size_t i = 0; i < nevents; ++i)
  {
    front_wave_1d_impl::Event event = impl->next_event();
    event_view view(event);
    visitor(view);
  }
}

std::size_t front_wave_1d::dimension() const { return 1; }
std::size_t front_wave_1d::configuration_size() const { return impl->size(); }
void front_wave_1d::visit_configuration(particle_visitor visitor) const
{
  using orig_particle = front_wave_1d_impl::Particle;
  using view = simbad::core::simple_particle_view<orig_particle>;
  for(orig_particle const &p : *impl)
    visitor(view(p));
}

void front_wave_1d::read_configuration(const simbad::core::configuration_view &)
{
  assert(false);
}

END_NAMESPACE_FRONT_WAVE_1D
