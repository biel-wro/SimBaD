#ifndef DISCRETE_WAVE_1D_TEMPL_HPP
#define DISCRETE_WAVE_1D_TEMPL_HPP

#include <cmath>
#include <cstddef>
#include <random>
#include <vector>

#include "event_kind.hpp"
#include "power_law_distribution.hpp"
#include "simple_event.hpp"

namespace simbad
{
namespace model_templates
{
namespace discrete_wave_1d
{

template <class Derived>
class discrete_wave_1d
{
public:
  class Event : public core::simple_event<long, long, 1>
  {
    using simple_event<long, long, 1>::simple_event;
  };

  discrete_wave_1d(double alpha, double intensity_cap, std::size_t length,
                   double spacing = 1.0, std::size_t seed = 0)
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

  discrete_wave_1d() {}
  Event initial_event()
  {
    grid_count[0] += 1;
    Event e(0, simbad::core::EVENT_KIND::BIRTH);
    e.set_coordinate(0, 0);

    Derived::update_birth_uncapped_density(e);
    Derived::recompute_birth_cumulated_density();

    return e;
  }

  Event next_event()
  {
    std::exponential_distribution<double> waitinig_time_dist(
        Derived::total_birth_intensity());
    time += waitinig_time_dist(random_engine);

    std::size_t birth_position = Derived::sample_birth_pos(random_engine);
    ++grid_count[birth_position];

    Event e(time, simbad::core::EVENT_KIND::BIRTH);
    e.set_coordinate(0, Derived::distance_from_origin(birth_position));

    Derived::update_birth_uncapped_density(e);
    Derived::recompute_birth_cumulated_density();

    return e;
  }

  double get_spacing() const { return grid_spacing; }
  std::vector<std::size_t> const &get_counts() const { return grid_count; }

protected:
  void update_birth_uncapped_density(const Event &e)
  {
    if (e.event_kind() != simbad::core::EVENT_KIND::BIRTH)
      throw std::runtime_error("unsupported event type");

    const double birth_pos = e.coordinate(0);

    std::vector<double>::iterator beg = birth_density.begin();
    std::vector<double>::iterator end = birth_density.end();

    for (std::vector<double>::iterator it = beg; it < end; ++it)
    {
      std::vector<double>::difference_type xind = it - beg;
      const double xpos = Derived::distance_from_origin(xind);
      const double rel_pos = xpos - birth_pos;
      *it += boost::math::pdf(birth_power_law, std::fabs(rel_pos) + 1.0);
    }
  }
  void recompute_birth_cumulated_density()
  {
    std::vector<double>::const_iterator idf_it, idf_end;
    idf_it = birth_density.begin();
    idf_end = birth_density.end();

    std::vector<double>::iterator cdf_it = birth_cumulated_density.begin();

    double acc = 0;
    for (; idf_it < idf_end; ++cdf_it, ++idf_it)
    {
      acc += Derived::capped_intensity(*idf_it);
      *cdf_it = acc;
    }
  }
  std::size_t sample_birth_pos(std::mt19937_64 &engine) const
  {
    std::uniform_real_distribution<double> udist(
        0.0, Derived::total_birth_intensity());
    double pseudo_quantile = udist(engine);

    std::vector<double>::const_iterator beg = birth_cumulated_density.begin();
    std::vector<double>::const_iterator end = birth_cumulated_density.end();
    std::vector<double>::const_iterator it =
        std::upper_bound(beg, end, pseudo_quantile);

    if (it == end)
      throw std::runtime_error("reached boundary position");

    return std::distance(beg, it);
  }

  double capped_intensity(double uncapped_density) const
  {
    return std::min(uncapped_density, intensity_cap);
  }
  double distance_from_origin(size_t cell) const
  {
    std::ptrdiff_t celldiff =
        std::ptrdiff_t(cell) - std::ptrdiff_t(grid_length / 2);
    return celldiff * grid_spacing;
  }

  double total_birth_intensity() const
  {
    return birth_cumulated_density.back();
  }

private:
  double time;
  const double intensity_cap;
  const double grid_spacing;
  std::mt19937_64 random_engine;

  simbad::core::power_law_distribution<double> birth_power_law;

  const std::size_t grid_length;
  std::vector<std::size_t> grid_count;
  std::vector<double> birth_density;
  std::vector<double> birth_cumulated_density;
};

// end namespaces
}
}
}

#endif // DISCRETE_WAVE_1D_HPP
