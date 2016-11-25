#ifndef POISSON_CONFIGURATION_HPP
#define POISSON_CONFIGURATION_HPP

#include "core_def.hpp"

#include "interface/configuration_view.hpp"
#include "property_tree.hpp"

#include <cstddef>
#include <random>

BEGIN_NAMESPACE_CORE
class poisson_configuration : public simbad::core::configuration_view
{
public:
  using dimension_type = std::size_t;
  using coord_scalar = double;

  explicit poisson_configuration(property_tree const &pt);
  explicit poisson_configuration(dimension_type dimension, size_type n,
                                 double radius = 1, bool ball = false,
                                 std::uint64_t seed = 0);
  explicit poisson_configuration(dimension_type dimension, double lambda,
                                 double radius = 1, bool ball = false,
                                 std::uint64_t seed = 0);

  size_type configuration_size() const override;
  size_type dimension() const override;
  void visit_configuration(particle_visitor v) const override;

  void set_dimension(dimension_type d);
  void set_size(size_type n);
  void set_sample_size(double lambda);

protected:
  size_type sample_poisson(double lambda);

private:
  coord_scalar m_radius;
  dimension_type m_dimension;
  std::size_t m_size;
  std::mt19937_64 m_rng;
  bool m_is_ball;
};

END_NAMESPACE_CORE

#endif