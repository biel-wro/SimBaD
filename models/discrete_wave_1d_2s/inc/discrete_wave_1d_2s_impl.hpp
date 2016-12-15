#ifndef DISCRETE_WAVE_1D_IMPL_HPP
#define DISCRETE_WAVE_1D_IMPL_HPP

#include "discrete_wave_1d_2s_fwd.hpp"
#include "discrete_wave_1d_templ.hpp"
#include "computational/distributions/power_law_distribution.hpp"

#include <random>
#include <vector>

BEGIN_NAMESPACE_DISCRETE_WAVE_1D_2S

class discrete_wave_1d_2s_impl
    : public simbad::model_templates::discrete_wave_1d::discrete_wave_1d_impl<
          discrete_wave_1d_2s_impl> {
public:
  using self_base =
      simbad::model_templates::discrete_wave_1d::discrete_wave_1d_impl<
          discrete_wave_1d_2s_impl>;

  using self_base::self_base;
  ~discrete_wave_1d_2s_impl();

  double distance_from_origin(size_t cell) const;
  std::size_t sample_birth_pos(std::mt19937_64 &engine) const;
};

/*
class discrete_wave_1d_2s_impl
{
public:
  using Event = event_1d;

  discrete_wave_1d_2s_impl(double alpha, double intenisty_cap, std::size_t
length,
                   double spacing = 1.0, std::size_t seed = 0);
  ~discrete_wave_1d_2s_impl();
  Event initial_event();

  Event next_event();

  double get_spacing() const;
  std::vector<std::size_t> const &get_counts() const;
protected:
  void update_birth_uncapped_density(const Event &e);
  void recompute_birth_cumulated_density();


  double capped_intensity(double uncapped_density) const;
  double distance_from_origin(size_t cell) const;
  double total_birth_intensity() const;

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
*/
END_NAMESPACE_DISCRETE_WAVE_1D_2S

#endif // DISCRETE_WAVE_1D_HPP
