#include "discrete_wave_1d_2s.hpp"

#include "simple_event_view.hpp"

#include "discrete_wave_1d_2s_impl.hpp"
#include "event_1d.hpp"
#include "particle.hpp"

BEGIN_NAMESPACE_DISCRETE_WAVE_1D_2S

discrete_wave_1d_2s::discrete_wave_1d_2s(double alpha, double intensity_cap,
                                   std::size_t length, double spacing,
                                   std::size_t seed)
    : impl(std::make_unique<discrete_wave_1d_2s_impl>(alpha, intensity_cap, length,
                                                   spacing, seed))
{
  impl->initial_event();
}

discrete_wave_1d_2s::~discrete_wave_1d_2s() {}

void discrete_wave_1d_2s::generate_events(event_visitor visitor,
                                       size_t nevents) const
{
  CORE_NAMESPACE::simple_event_view<discrete_wave_1d_2s_impl::Event> event_view;
  for (size_t i = 0; i < nevents; ++i)
  {
    discrete_wave_1d_2s_impl::Event event = impl->next_event();
    event_view = event;
    visitor(event_view);
  }
}

std::size_t discrete_wave_1d_2s::configuration_size() const
{
  uint64_t total_count = 0;
  for (std::size_t site_count : impl->get_counts())
    total_count += site_count;

  return total_count;
}

namespace
{
struct particle_view : public CORE_NAMESPACE::particle
{
  double pos;
  std::size_t dimension() const override { return 1; }
  double coord(std::size_t d) const override { return pos; }
};
}

void discrete_wave_1d_2s::visit_configuration(particle_visitor v) const
{
  double spacing = impl->get_spacing();
  particle_view view;
  std::size_t siteno = 0;
  for (std::size_t site_count : impl->get_counts())
  {
    view.pos = spacing * siteno;

    for (std::size_t count = 0; count < site_count; ++count)
      v(view);

    ++siteno;
  }
}

END_NAMESPACE_DISCRETE_WAVE_1D_2S
