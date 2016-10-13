#ifndef ORDERED_BOARD_TILE_HPP
#define ORDERED_BOARD_TILE_HPP
#ifndef CORE_HEAPIFIED_BOARD_TILE_HPP
#define CORE_HEAPIFIED_BOARD_TILE_HPP
#include "core_def.hpp"

#include <algorithm>
#include <utility>

#include <boost/intrusive/unordered_set_hook.hpp>

BEGIN_NAMESPACE_CORE
template <class T, class BC, class Subcontainer>
class ordered_board_tile : public boost::intrusive::unordered_set_base_hook<>
{
public:
  using value_type = T;
  using coordinates_type = BC;
  using subcontainer_type = Subcontainer;

  using size_type = typename subcontainer_type::size_type;
  using reference_type = typename subcontainer_type::reference_type;
  using const_reference_type = typename subcontainer_type::const_reference_type;

  using iterator = typename subcontainer_type::iterator;
  using const_iterator = typename subcontainer_type::const_iterator;

  /*
   * Modifiers
   */
  template <class... Args> iterator dirty_emplace(Args &&... args)
  {
    iterator it = m_subcontainer.emplace(std::forward(args)...);
    return it;
  }

  template <class Compare, class... Args>
  iterator emplace(Compare cmp, Args &&... args)
  {
    dirty_emplace(std::forward(args)...);
    incremental_find_top(it, cmp);
    return it;
  }

  void dirty_remove(const_iterator it)
  {
    std::swap(mutable_iter(it), m_subcontainer.back());
    m_subcontainer.pop_back();
  }

  template <class Compare>
  void remove(const_iterator it, Compare cmp = Compare())
  {
    dirty_remove(it);
    find_top(cmp);
  }

  void clear() { m_subcontainer.clear(); }
  /*
   * Status
   */
  size_type size() const { return m_subcontainer.size(); }
  bool empty() const { return m_subcontainer.empty(); }
  /*
   * Ordering
   */
  template <class Compare> void find_top(Compare cmp = Compare())
  {
    m_top_it = std::max_element(begin(), end(), cmp);
  }

  void incremental_find_top_reset() { m_top_it = end(); }
  template <class Compare>
  void incremental_find_top(const_iterator v, Compare cmp = Compare())
  {
    if(end() == m_top_it)
    {
      m_top_it = v;
      return;
    }

    if(cmp(*m_top_it, v))
      m_top_it = v;
  }

  /*
   * Visitors
   */
  template <class Visitor> void visit(Visitor v = Visitor()) const
  {
    std::for_each(begin(), end(), v);
  }

  template <class Visitor> void dirty_visit(Visitor v = Visitor())
  {
    std::for_each(begin(), end(), v);
  }

  template <class Visitor, class Cmp>
  void visit(Visitor v = Visitor(), Cmp cmp = Cmp())
  {
    std::for_each(begin(), end(), [this, &v](reference_type &ref) {
      v(ref);
      incremental_find_top(v, cmp)
    });
  }

  /*
   * Iterators
   */
  iterator mutable_iter(const_iterator cit)
  {
    return const_cast<iterator>(cit);
  }
  iterator begin() { return m_subcontainer.begin(); }
  iterator end() { return m_subcontainer.end(); }
  const_iterator begin() const { return m_subcontainer.begin(); }
  const_iterator end() const { return m_subcontainer.end(); }
  /*
   * Accessors
   */
  //reference_type top() { return *mutable_iter(m_top_it); }
  const_reference_type top() const { return *m_top_it; }
  //reference_type front() { return *m_subcontainer.begin(); }
  const_reference_type front() const { return *m_subcontainer.begin(); }
  //reference_type end() { return *(--m_subcontainer.end()); }
  const_reference_type end() const { return *(--m_subcontainer.end()); }
protected:
  coordinates_type m_coordinates;
  subcontainer_type m_subcontainer;
  const_iterator m_top_it;
};

END_NAMESPACE_CORE

#endif // ORDERED_BOARD_TILE_HPP
