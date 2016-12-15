#ifndef SPACE_HPP
#define SPACE_HPP

#include "space_impl.hpp"

BEGIN_NAMESPACE_CORE
template <class T, class config> class space
{
public:
  // member types
  using implementation_type = space_impl<T, config>;
  using iterator = typename implementation_type::iterator;
  using board_coords = typename implementation_type::board_coords;
  using space_coords = typename implementation_type::space_coords;
  using coord_tiler = typename implementation_type::coord_tiler;

  space(coord_tiler ct = coord_tiler()) : m_impl(ct) {}
  space(space const &) = delete;
  space(space &&) = default;
  space &operator=(space const &) = delete;
  space &operator=(space &&) = default;

  std::size_t size() const { return m_impl.size(); }
  bool empty() const { return m_impl.size() == 0; }
  template <class... Args>
  iterator emplace(space_coords const &sc, Args... args)
  {
    return m_impl.emplace(sc, args...);
  }

  void remove(space_coords const &sc, T &v) { m_impl.remove(sc, v); }
  void clear() { m_impl.clear(); }
  template <class Visitor> void visit(Visitor v = Visitor())
  {
    m_impl.visit(v);
  }
  template <class Visitor> void visit(Visitor v = Visitor()) const
  {
    m_impl.visit(v);
  }
  template <class Visitor>
  void visit_box(space_coords const &cmin, space_coords const &cmax,
                 Visitor v = Visitor())
  {
    m_impl.visit_box(cmin, cmax, v);
  }

  template <class Visitor>
  void visit_box(space_coords const &cmin, space_coords const &cmax,
                 Visitor v = Visitor()) const
  {
    m_impl.visit_box(cmin, cmax, v);
  }

private:
  implementation_type m_impl;
};

END_NAMESPACE_CORE

#endif // SPACE_HPP
