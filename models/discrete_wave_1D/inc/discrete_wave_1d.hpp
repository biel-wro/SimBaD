#ifndef DISCRETE_WAVE_1D_HPP
#define DISCRETE_WAVE_1D_HPP

#include "discrete_wave_1d_fwd.hpp"
#include "power_law_distribution.hpp"


#include <random>
#include <vector>

BEGIN_NAMESPACE_DISCRETE_WAVE_1D

class discrete_wave_1d
{
public:
  using Event = event_1d;

  discrete_wave_1d(double alpha, double spacing, std::size_t length, std::size_t seed);

  Event next_event();

protected:
  void update_birth_density(const Event &e);
  void update_birth_cumulated_density();

  double distance_from_origin(size_t cell) const;
private:
  double grid_spacing;
  std::mt19937_64 random_engine;
  simbad::core::power_law_distribution<double> birth_power_law;

  std::vector<std::size_t> grid_count;
  std::vector<double> birth_density;
  std::vector<double> birth_cumulated_density;
};

END_NAMESPACE_DISCRETE_WAVE_1D

#endif // DISCRETE_WAVE_1D_HPP
