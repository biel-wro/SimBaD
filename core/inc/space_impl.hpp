#ifndef SPACE_IMPL_HPP
#define SPACE_IMPL_HPP

#include "core_def.hpp"

#include "board_impl.hpp"

BEGIN_NAMESPACE_CORE
template <class T, class config>
class space_impl
{
public:
  using board_impl_type = board_impl<T, config>;
  using board_coords = typename config::board_coords;
  using space_coords = typename config::space_coords;
  using coord_tiler = typename config::coord_tiler;

  // dependent types
  using iterator = typename board_impl_type::iterator;
  /*
   *  accessors
   */
  std::size_t size() const { return board.size(); }

  /*
   *  modifiers
   */
  template <class... Args>
  iterator emplace(space_coords const &sc, Args... args)
  {
    board_coords bc(m_coord_tiler(sc));
    board.emplace(bc, args...);
  }

  void remove(space_coords const &sc, T &v)
  {
    board_coords bc(m_coord_tiler(sc));
    board.remove(bc, v);
  }

  /*
   * visitors
   */
  template <class Visitor>
  void visit(Visitor v = Visitor())
  {
    board.visit(v);
  }

  template <class Visitor>
  void visit(Visitor v = Visitor()) const
  {
    board.visit(v);
  }

  template <class Inc, class Visitor>
  void visit_region(Inc inc = Inc(), Visitor v = Visitor())
  {
    board.visit_region(inc, v);
  }

  template <class Inc, class Visitor>
  void visit_region(Inc inc = Inc(), Visitor v = Visitor()) const
  {
    board.visit_region(inc, v);
  }

  /*
   * Iterators
   */

protected:
  coord_tiler m_coord_tiler;
  board_impl_type board;
};

END_NAMESPACE_CORE

#endif // SPACE_IMPL_HPP
