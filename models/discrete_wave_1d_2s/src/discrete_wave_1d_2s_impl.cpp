#include "discrete_wave_1d_2s_impl.hpp"

#include "interface/event_kind.hpp"

BEGIN_NAMESPACE_DISCRETE_WAVE_1D_2S


discrete_wave_1d_2s_impl::~discrete_wave_1d_2s_impl() {}

double discrete_wave_1d_2s_impl::distance_from_origin(size_t cell) const {
    std::ptrdiff_t celldiff =
            std::ptrdiff_t(cell) - std::ptrdiff_t(grid_length / 2);
    return celldiff * grid_spacing;
}

std::size_t discrete_wave_1d_2s_impl::sample_birth_pos(std::mt19937_64 &engine) const
{
  std::uniform_real_distribution<double> udist(0.0, total_birth_intensity());
  double pseudo_quantile = udist(engine);

  std::vector<double>::const_iterator beg = birth_cumulated_density.begin();
  std::vector<double>::const_iterator end = birth_cumulated_density.end();
  std::vector<double>::const_iterator it =
      std::upper_bound(beg, end, pseudo_quantile);

  if (it == end)
    throw std::runtime_error("reached boundary position");

  return std::distance(beg, it);
}

END_NAMESPACE_DISCRETE_WAVE_1D_2S
