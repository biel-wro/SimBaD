#ifndef SPACE_IMPL_HPP
#define SPACE_IMPL_HPP

#include "core_def.hpp"

#include "board_impl.hpp"

BEGIN_NAMESPACE_CORE
template <class T, class config> class space_impl
{
public:
  using board_impl_type = board_impl<T, config>;
  using board_coords = typename config::board_coords;
  using space_coords = typename config::space_coords;
  using coord_tiler = typename config::coord_tiler;

  // dependent types
  using iterator = typename board_impl_type::iterator;
  /*
   * constructors
   */

  explicit space_impl(coord_tiler tiler = coord_tiler()) : m_coord_tiler(tiler)
  {
  }

  /*
   *  accessors
   */
  std::size_t size() const { return m_board.size(); }

  /*
   *  modifiers
   */
  template <class... Args>
  iterator emplace(space_coords const &sc, Args... args)
  {
    board_coords bc(m_coord_tiler(sc));
    return m_board.emplace(bc, args...);
  }

  void remove(space_coords const &sc, T &v)
  {
    board_coords bc(m_coord_tiler(sc));
    m_board.remove(bc, v);
  }

  void clear() { m_board.clear(); }

  /*
   * visitors
   */
  template <class Visitor> void visit(Visitor v = Visitor())
  {
    m_board.visit(v);
  }

  template <class Visitor> void visit(Visitor v = Visitor()) const
  {
    m_board.visit(v);
  }

  template <class Inc, class Visitor>
  void visit_region(Inc inc = Inc(), Visitor v = Visitor())
  {
    m_board.visit_region(inc, v);
  }

  template <class Inc, class Visitor>
  void visit_region(Inc inc = Inc(), Visitor v = Visitor()) const
  {
    m_board.visit_region(inc, v);
  }

  template <class Visitor>
  void visit_box(space_coords const &cmin, space_coords const &cmax,
                 Visitor v = Visitor())
  {
    board_coords rmin(m_coord_tiler(cmin));
    board_coords rmax(m_coord_tiler(cmax));
    coord_incrementer_box<board_coords> box_incrementer(rmin, rmax);
    visit_region(box_incrementer, v);
  }

  template <class Visitor>
  void visit_box(space_coords const &cmin, space_coords const &cmax,
                 Visitor v = Visitor()) const
  {
    board_coords rmin(m_coord_tiler(cmin));
    board_coords rmax(m_coord_tiler(cmax));
    coord_incrementer_box<board_coords> box_incrementer(rmin, rmax);
    visit_region(box_incrementer, v);
  }

  /*
   * Iterators
   */

protected:
  coord_tiler m_coord_tiler;
  board_impl_type m_board;
};

END_NAMESPACE_CORE

#endif // SPACE_IMPL_HPP
