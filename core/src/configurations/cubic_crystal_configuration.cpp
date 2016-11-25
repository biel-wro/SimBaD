#include "configurations/cubic_crystal_configuration.hpp"

#include "property_tree.hpp"

#include <cmath>
BEGIN_NAMESPACE_CORE
cubic_crystal_configuration::cubic_crystal_configuration(
    const property_tree &pt)
    : cubic_crystal_configuration(pt.get<size_type>("dimension"),
                                  pt.get<double>("radius"),
                                  pt.get<double>("spacing"))
{
}
cubic_crystal_configuration::cubic_crystal_configuration(size_type dimension,
                                                         double radius,
                                                         double spacing)
    : m_dimension(dimension), m_radius(radius), m_spacing(spacing)
{
}
configuration_view::size_type
cubic_crystal_configuration::configuration_size() const
{
  return std::pow(2 * spacings_per_radius() + 1, m_dimension);
}
configuration_view::size_type cubic_crystal_configuration::dimension() const
{
  return m_dimension;
}

namespace
{
struct particle_view : public simbad::core::particle
{
  particle_view(std::size_t dim) : m_position(dim) {}
  std::size_t dimension() const override { return m_position.size(); }
  double coord(std::size_t d) const override
  {
    assert(dimension() > d);
    return m_position[d];
  }
  std::vector<double> m_position;
};
}

void cubic_crystal_configuration::visit_configuration(
    particle_visitor visitor) const
{
  std::size_t const dim = dimension();

  using index_type = std::int64_t;

  auto ind2pos = [this](index_type ind) { return ind * m_spacing; };

  index_type idx_max = spacings_per_radius();
  index_type idx_min = -idx_max;

  std::vector<index_type> const lower_bound(dim, idx_min);
  std::vector<index_type> const upper_bound(dim, idx_max);

  std::vector<index_type> index = lower_bound;

  particle_view view(dim);
  std::vector<double> &position = view.m_position;
  std::fill(position.begin(), position.end(), ind2pos(idx_min));

  bool last = true;
  do
  {
    visitor(view);
    for(std::size_t d = 0; d < dim; ++d)
    {
      index_type &idx = index[d];
      if(upper_bound[d] == idx)
      {
        idx = lower_bound[d];
        position[d] = ind2pos(idx);
        last = true;
      }
      else
      {
        ++idx;
        position[d] = ind2pos(idx);
        last = false;
        break;
      }
    }
  } while(!last);
}

cubic_crystal_configuration::size_type
cubic_crystal_configuration::spacings_per_radius() const
{
  return size_type(m_radius / m_spacing);
}
END_NAMESPACE_CORE
