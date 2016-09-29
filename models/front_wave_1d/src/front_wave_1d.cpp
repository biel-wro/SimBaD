#include "front_wave_1d.hpp"

#include "front_wave_1d_impl.hpp"
#include "simple_event_view.hpp"
#include "simple_particle_view.hpp"

BEGIN_NAMESPACE_FRONT_WAVE_1D
front_wave_1d::front_wave_1d(double alpha, double x0 = 1.0)
    : impl(new front_wave_1d_impl(alpha, x0))
{
}

void front_wave_1d::generate_events(event_visitor visitor, size_t nevents)
{
  CORE_NAMESPACE::simple_event_view<front_wave_1d_impl::Event> event_view;
  for (size_t i = 0; i < nevents; ++i)
  {
    front_wave_1d_impl::Event event = impl->next_event();
    event_view = event;
    visitor(event_view);
  }
}

std::size_t front_wave_1d::configuration_size() const { return impl->size(); }

void front_wave_1d::visit_configuration(particle_visitor visitor) const
{
  using orig_particle = front_wave_1d_impl::Particle;
  using view = simbad::core::simple_particle_view<orig_particle>;
  for (orig_particle const &p : *impl)
    visitor(view(p));
}

END_NAMESPACE_FRONT_WAVE_1D
