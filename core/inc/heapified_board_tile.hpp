#ifndef CORE_HEAPIFIED_BOARD_TILE_HPP
#define CORE_HEAPIFIED_BOARD_TILE_HPP
#include "core_def.hpp"

#include <algorithm>
#include <utility>

#include <boost/intrusive/unordered_set_hook.hpp>

BEGIN_NAMESPACE_CORE

template <class T, class BC>
class heapified_board_tile : public boost::intrusive::unordered_set_base_hook<>
{
public:
  using value_type = T;

  using subcontainer_type = std::vector<value_type>;
  using reference_type = typename subcontainer_type::reference_type;
  using const_reference_type = typename subcontainer_type::const_reference_typ;

  using coordinates_type = BC;
  using iterator = typename subcontainer_type::iterator;
  using const_iterator = typename subcontainer_type::const_interator;

  heapified_board_tile() = delete;
  heapified_board_tile(const BC &coords) : m_coords(coords) {}
  heapified_board_tile(const heapified_board_tile &) = delete;
  heapified_board_tile(heapified_board_tile &&) = default;
  heapified_board_tile &operator=(const heapified_board_tile &) = delete;
  heapified_board_tile &operator=(heapified_board_tile &&) = default;
  ~heapified_board_tile() {}
  coordinates_type const &get_coords() const { return m_coords; }
  /*
   * Modifiers
   */
  template <class... Args> iterator emplace(Args &&... args)
  {
    m_subcontainer.emplace_back(std::forward<Args>(args)...);
    throw "not implemented yet";
  }

  void clear() { m_subcontainer.clear(); }
  /*
   * Status
   */
  size_t size() const { return m_subcontainer.size(); }
  bool empty() const { return m_subcontainer.empty(); }
  /*
   * Ordering
   */

  template <class Compare> void update(reference_type &ref, Compare cmp)
  {
    throw "not implemented yet";
  }

  template <class Compare> void heapify(Compare cmp = Compare())
  {
    std::sort_heap(begin(), end(), cmp);
    throw "not implemented yet";
  }

  /*
   * Visitors
   */
  template <class Visitor> void visit(Visitor v = Visitor()) const
  {
    std::for_each(begin(), end(), v);
  }

  template <class Visitor, class Cmp>
  void visit(Visitor v = Visitor(), Cmp cmp = Cmp())
  {
    std::for_each(begin(), end(), v);
    heapify(cmp);
  }

  /*
   * Iterators
   */
  iterator begin() { return m_subcontainer.begin(); }
  iterator end() { return m_subcontainer.end(); }
  const_iterator begin() const { return m_subcontainer.begin(); }
  const_iterator end() const { return m_subcontainer.end(); }
protected:
  coordinates_type m_coords;
  subcontainer_type m_subcontainer;
};

END_NAMESPACE_CORE

#endif
