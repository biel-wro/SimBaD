#ifndef BOARD_HPP
#define BOARD_HPP

#include "board_impl.hpp"
#include "coordinates/coordinates.hpp"

namespace simbad
{
namespace core
{
template <class coord, int dim, int chunk_sz_> struct board_config
{
  using board_coord_scalar = coord;

  using board_coords = coordinates<board_coord_scalar, dim>;

  // using int_chunk_size = std::integral_constant<int, chunk_sz>;
  static constexpr unsigned dimension = dim;
  static constexpr unsigned chunk_size = chunk_sz_;
};

template <class T, int dim, class coord = long, int chunk_sz = 1000> class board
{
public:
  typedef board_config<coord, dim, chunk_sz> config;
  typedef board_impl<T, config> implementation;
  typedef typename implementation::coordinates_type coordinates_type;
  typedef typename implementation::iterator iterator;
  typedef typename implementation::const_iterator const_iterator;

  using size_type = typename implementation::size_type;

  using box_iterator = typename implementation::box_iterator;
  using const_box_iterator = typename implementation::const_box_iterator;

  using box_region = typename implementation::box_region;
  using const_box_region = typename implementation::const_box_region;

protected:
  implementation m_impl;

public:
  void rehash(size_t new_size) { m_impl.rehash(new_size); }

  iterator begin() { return m_impl.begin(); }

  iterator end() { return m_impl.end(); }

  const_iterator begin() const { return m_impl.begin(); }

  const_iterator end() const { return m_impl.end(); }

  box_region get_box(coordinates_type min, coordinates_type max)
  {
    return m_impl.get_box(min, max);
  }

  const_box_region get_box(coordinates_type min, coordinates_type max) const
  {
    return m_impl.get_box(min, max);
  }

  iterator iterator_to(coordinates_type const &cs, T const &v)
  {
    return m_impl.iterator_to(cs, v);
  }

  const_iterator iterator_to(coordinates_type const &cs, T const &v) const
  {
    return m_impl.iterator_to(cs, v);
  }

  template <class... Args>
  iterator emplace(coordinates_type const &c, Args... args)
  {
    return m_impl.emplace(c, std::forward<Args>(args)...);
  }

  void remove(coordinates_type const &c, T &v) { m_impl.remove(c, v); }

  void clear() { m_impl.clear(); }

  size_type size() const { return m_impl.size(); }

  template <class Visitor> void visit(Visitor v = Visitor())
  {
    m_impl.visit(v);
  }

  template <class ConstVisitor>
  void visit(ConstVisitor v = ConstVisitor()) const
  {
    m_impl.visit(v);
  }

  template <class Incrementer, class Visitor>
  void visit_region(Incrementer inc = Incrementer(), Visitor v = Visitor())
  {
    m_impl.visit_region(inc, v);
  }

  template <class Incrementer, class Visitor>
  void visit_region(Incrementer inc = Incrementer(),
                    Visitor v = Visitor()) const
  {
    m_impl.visit_region(inc, v);
  }

  template <class Visitor>
  void visit_box(coordinates_type bmin, coordinates_type bmax,
                 Visitor v = Visitor())
  {
    m_impl.visit_box(bmin, bmax, v);
  }

  template <class ConstVisitor>
  void visit_box(coordinates_type bmin, coordinates_type bmax,
                 ConstVisitor v = ConstVisitor()) const
  {
    m_impl.visit_box(bmin, bmax, v);
  }
};
}
}

#endif
