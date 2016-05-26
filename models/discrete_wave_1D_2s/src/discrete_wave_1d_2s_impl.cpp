#include "discrete_wave_1d_2s_impl.hpp"

#include "event_kind.hpp"

#include "event_1d.hpp"

BEGIN_NAMESPACE_DISCRETE_WAVE_1D_2S

discrete_wave_1d_2s_impl::discrete_wave_1d_2s_impl(double alpha,
                                                   double intensity_cap,
                                                   std::size_t length,
                                                   double spacing,
                                                   std::size_t seed)
    : time(0),
      intensity_cap(intensity_cap),
      grid_spacing(spacing),
      random_engine(seed),
      birth_power_law(1.0, alpha),
      grid_length(length),
      grid_count(grid_length, 0),
      birth_density(grid_length, 0.0),
      birth_cumulated_density(grid_length, 0.0)
{
}

discrete_wave_1d_2s_impl::~discrete_wave_1d_2s_impl() {}

discrete_wave_1d_2s_impl::Event discrete_wave_1d_2s_impl::initial_event()
{
  grid_count[0] += 1;
  Event e(0, simbad::core::EVENT_KIND::BIRTH);
  e.set_coordinate(0, 0);

  update_birth_uncapped_density(e);
  recompute_birth_cumulated_density();

  return e;
}

discrete_wave_1d_2s_impl::Event discrete_wave_1d_2s_impl::next_event()
{
  std::exponential_distribution<double> waitinig_time_dist(
      total_birth_intensity());
  time += waitinig_time_dist(random_engine);

  std::size_t birth_position = sample_birth_pos(random_engine);
  ++grid_count[birth_position];

  Event e(time, simbad::core::EVENT_KIND::BIRTH);
  e.set_coordinate(0, distance_from_origin(birth_position));

  update_birth_uncapped_density(e);
  recompute_birth_cumulated_density();

  return e;
}

double discrete_wave_1d_2s_impl::get_spacing() const { return grid_spacing; }

const std::vector<std::size_t> &discrete_wave_1d_2s_impl::get_counts() const
{
  return grid_count;
}

void discrete_wave_1d_2s_impl::update_birth_uncapped_density(Event const &e)
{
  if (e.event_kind() != simbad::core::EVENT_KIND::BIRTH)
    throw std::runtime_error("unsupported event type");

  const double birth_pos = e.coordinate(0);

  std::vector<double>::iterator beg = birth_density.begin();
  std::vector<double>::iterator end = birth_density.end();

  for (std::vector<double>::iterator it = beg; it < end; ++it)
  {
    std::vector<double>::difference_type xind = it - beg;
    const double xpos = distance_from_origin(xind);
    const double rel_pos = xpos - birth_pos;
    *it += boost::math::pdf(birth_power_law, std::fabs(rel_pos) + 1.0);
  }
}

void discrete_wave_1d_2s_impl::recompute_birth_cumulated_density()
{
  std::vector<double>::const_iterator idf_it, idf_end;
  idf_it = birth_density.begin();
  idf_end = birth_density.end();

  std::vector<double>::iterator cdf_it = birth_cumulated_density.begin();

  double acc = 0;
  for (; idf_it < idf_end; ++cdf_it, ++idf_it)
  {
    acc += capped_intensity(*idf_it);
    *cdf_it = acc;
  }
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

double discrete_wave_1d_2s_impl::capped_intensity(double uncapped_density) const
{
  return std::min(uncapped_density, intensity_cap);
}

double discrete_wave_1d_2s_impl::distance_from_origin(size_t cell) const
{
  std::ptrdiff_t celldiff = std::ptrdiff_t(cell)-std::ptrdiff_t(grid_length/2);
  return celldiff * grid_spacing;
}

double discrete_wave_1d_2s_impl::total_birth_intensity() const
{
  return birth_cumulated_density.back();
}

END_NAMESPACE_DISCRETE_WAVE_1D_2S
