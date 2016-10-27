#ifndef CORE_COORD_GENERATORS_HPP
#define CORE_COORD_GENERATORS_HPP
#include "core_def.hpp"
#include <boost/iterator/iterator_facade.hpp>

BEGIN_NAMESPACE_CORE

template <class CV, class RLB = CV, class RUB = CV, class BLB = CV,
          class BUB = CV>
class coord_generator_grid : public boost::iterator_facade<
                                 coord_generator_grid<CV, RLB, RUB, BLB, BUB>,
                                 CV, boost::forward_traversal_tag, CV const &>
{
public:
  using coord_vector_type = CV;

  using region_lb = RLB;
  using region_ub = RUB;
  using board_lb = BLB;
  using board_ub = BUB;

  using coord_scalar_type = typename coord_vector_type::scalar_type;
  using dimension_type = typename coord_vector_type::dimension_type;
  static constexpr dimension_type dimension = coord_vector_type::dimension;

  coord_generator_grid(region_lb rlb = region_lb(), region_ub rub = region_ub(),
                       board_lb blb = board_lb(), board_ub bub = board_ub(),
                       coord_vector_type start = coord_vector_type(),
                       bool end = false)
      : m_coords(start),
        m_region_lb(rlb),
        m_region_ub(rub),
        m_board_lb(blb),
        m_board_ub(bub),
        m_end(end)
  {
  }

  void increment()
  {
    for(dimension_type d = 0; d < dimension; d++)
    {
      coord_scalar_type &c = m_coords(d);
      if(m_board_ub(d) == c)
      {
        c = m_board_lb(d);
        return;
      }
      else if(m_region_ub(d) == c)
        c = m_region_lb(d);
      else
      {
        c++;
        return;
      }
    }

    m_end = true;
  }
  coord_vector_type operator()() const
  {
    coord_vector_type res = dereference();
    increment();
    return res;
  }
  coord_vector_type const &dereference() const { return m_coords; }
  bool equal(coord_generator_grid const &o) const
  {
    return m_end == o.m_end && m_coords == o.m_curr_coord;
  }
  operator bool() const { return !is_end(); }
  bool is_end() const { return m_end; }
private:
  friend class boost::iterator_core_access;

  coord_vector_type m_coords;
  region_lb m_region_lb;
  region_ub m_region_ub;
  board_lb m_board_lb;
  board_ub m_board_ub;
  bool m_end;
};

struct never_equal
{
  template <class numeric> operator numeric() const { return 0; }
};

template <class numeric> bool operator==(never_equal, numeric) { return false; }
template <class numeric> bool operator==(numeric, never_equal) { return false; }
struct no_bound
{
  template <class numeric> never_equal operator()(numeric) const
  {
    return never_equal();
  }
};

template <class CV, class RLB = CV, class RUB = CV>
using coord_generator_box =
    coord_generator_grid<CV, RLB, RUB, no_bound, no_bound>;

END_NAMESPACE_CORE
#endif // COORD_GENERATORS_HPP
