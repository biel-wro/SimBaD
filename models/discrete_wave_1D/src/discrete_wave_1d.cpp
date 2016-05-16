#include "discrete_wave_1d.hpp"

#include "event_kind.hpp"

#include "event_1d.hpp"

BEGIN_NAMESPACE_DISCRETE_WAVE_1D

discrete_wave_1d::discrete_wave_1d(double alpha, double spacing,
                                   std::size_t length, std::size_t seed)
    : grid_spacing(spacing),
      random_engine(seed),
      birth_power_law(1.0, alpha),
      grid_count(length, 0),
      birth_density(length, 0.0),
      birth_cumulated_density(length, 0.0)
{
}

discrete_wave_1d::Event discrete_wave_1d::next_event() {}

void discrete_wave_1d::update_birth_density(Event const &e)
{
  if (e.event_kind() != simbad::core::EVENT_KIND::BIRTH)
    throw std::runtime_error("unsupported event type");

  const double birth_pos = distance_from_origin( e.coordinate(0) );

  std::vector<double>::iterator beg = birth_density.begin();
  std::vector<double>::iterator end = birth_density.end();

  for (std::vector<double>::iterator it = beg; it < end; ++it)
  {
    std::vector<double>::difference_type xind = it - beg;
    const double xpos = distance_from_origin(xind);

    const double rel_pos = xpos - birth_pos;

  }
}

void discrete_wave_1d::update_birth_cumulated_density()
{
  std::vector<double>::const_iterator idf_it, idf_end;
  idf_it = birth_density.begin();
  idf_end = birth_density.end();

  std::vector<double>::iterator cdf_it = birth_cumulated_density.begin();

  double acc = 0;
  for (; idf_it < idf_end; ++cdf_it, ++idf_it)
  {
    acc += *idf_it;
    *cdf_it = acc;
  }
}

double discrete_wave_1d::distance_from_origin(size_t cell) const
{
  return cell * grid_spacing;
}

END_NAMESPACE_DISCRETE_WAVE_1D
