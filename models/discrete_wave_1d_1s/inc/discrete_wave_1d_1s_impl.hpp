#ifndef discrete_wave_1d_1s_impl_HPP
#define discrete_wave_1d_1s_impl_HPP

#include "discrete_wave_1d_1s_fwd.hpp"
#include "discrete_wave_1d_templ.hpp"
#include "power_law_distribution.hpp"

#include <random>
#include <vector>

BEGIN_NAMESPACE_DISCRETE_WAVE_1D_1S

class discrete_wave_1d_1s_impl
    : public simbad::model_templates::discrete_wave_1d::discrete_wave_1d_impl<
          discrete_wave_1d_1s_impl> {
public:
  using self_base =
      simbad::model_templates::discrete_wave_1d::discrete_wave_1d_impl<
          discrete_wave_1d_1s_impl>;

  using self_base::self_base;
  ~discrete_wave_1d_1s_impl();

  double distance_from_origin(size_t cell) const;
  std::ptrdiff_t sample_birth_pos(std::mt19937_64 &engine) const;
};
END_NAMESPACE_DISCRETE_WAVE_1D_1S

#endif // DISCRETE_WAVE_1D_HPP
