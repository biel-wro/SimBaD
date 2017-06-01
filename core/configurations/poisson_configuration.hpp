#ifndef POISSON_CONFIGURATION_HPP
#define POISSON_CONFIGURATION_HPP

#include "core_def.hpp"

#include "interface/configuration_view.hpp"
#include "interface/property_tree.hpp"

#include <cstddef>
#include <random>

BEGIN_NAMESPACE_CORE
class poisson_configuration final : public simbad::core::configuration_view
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

  size_type size() const override;
  void visit_records(particle_visitor v) const override;
  attribute_descriptor const &descriptor() const override;


  ~poisson_configuration();

  void set_size(size_type n);
  void set_sample_size(double lambda);

protected:
   void init_descriptor();
  size_type sample_poisson(double lambda);

private:
  coord_scalar m_radius;
  dimension_type m_dimension;
  std::mt19937_64 m_rng;
  std::size_t m_size;
  bool m_is_ball;
};

END_NAMESPACE_CORE

#endif
