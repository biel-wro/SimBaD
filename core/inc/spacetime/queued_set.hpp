#ifndef CORE_QUEUED_SET_HPP
#define CORE_QUEUED_SET_HPP

#include "core_def.hpp"

#include <algorithm>
#include <utility>
#include <vector>

BEGIN_NAMESPACE_CORE

template <class T, class Subcontainer = std::vector<T>> class queued_set
{
public:
  using value_type = T;
  using subcontainer_type = Subcontainer;

  using size_type = typename subcontainer_type::size_type;
  using difference_type = typename subcontainer_type::difference_type;

  using dirty_reference = typename subcontainer_type::reference;
  using const_reference = typename subcontainer_type::const_reference;
  using reference = const_reference;

  using dirty_iterator = typename subcontainer_type::iterator;
  using const_iterator = typename subcontainer_type::const_iterator;
  using iterator = const_iterator;

  using dirty_handle_type = dirty_iterator;
  using const_handle_type = const_iterator;

  template <class Compare> struct iterator_compare
  {
    explicit iterator_compare(Compare cmp = Compare()) : m_vcmp(cmp) {}
    bool operator()(const_iterator it1, const_iterator it2) const
    {
      return m_vcmp(*it1, *it2);
    }

  protected:
    Compare m_vcmp;
  };

  queued_set() : m_subcontainer(), m_top_it(m_subcontainer.end()) {}
  /*
   * Modifiers
   */

  template <class Compare, class... Args>
  const_iterator emplace(Compare cmp, Args &&... args)
  {
    if(m_subcontainer.end() == m_top_it)
    {
      dirty_iterator it = emplace_dirty(std::forward<Args>(args)...);
      m_top_it = it;
      return it;
    }

    difference_type idx = m_top_it - m_subcontainer.begin();
    const_iterator it = emplace_dirty(std::forward<Args>(args)...);

    const_iterator top_it =
        std::max(begin() + idx, it, iterator_compare<Compare>(cmp));
    m_top_it = dirty_iter(top_it);

    return it;
  }
  template <class... Args> dirty_iterator emplace_dirty(Args &&... args)
  {
    bool was_empty = empty();
    m_subcontainer.emplace_back(std::forward<Args>(args)...);
    if(was_empty)
      m_top_it = begin_dirty();
    return --m_subcontainer.end();
  }

  template <class Compare>
  void remove(const_iterator it, Compare cmp = Compare())
  {
    if(m_top_it == it)
    {
      remove_dirty(it);
      find_top(cmp);
      return;
    }

    difference_type idx;

    if(m_top_it == --end())
      idx = it - m_subcontainer.begin();
    else
      idx = m_top_it - m_subcontainer.begin();

    remove_dirty(it);
    m_top_it = begin_dirty() + idx;
  }
  void remove_dirty(const_iterator it)
  {
    if(it != --m_subcontainer.end())
      std::swap(*dirty_iter(it), m_subcontainer.back());

    m_subcontainer.pop_back();
  }
  template <class Compare> void pop(Compare cmp = Compare())
  {
    remove(m_top_it, std::move(cmp));
  }
  void pop_dirty() { remove_dirty(m_top_it); }
  void clear()
  {
    m_subcontainer.clear();
    m_top_it = m_subcontainer.end();
  }
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
    m_top_it = dirty_iter(std::max_element(begin(), end(), cmp));
  }
  template <class Compare> void repair_order(Compare cmp = Compare())
  {
    m_top_it = dirty_iter(std::max_element(begin(), end(), cmp));
  }
  template <class Compare>
  void repair_order(const_iterator it, Compare cmp = Compare())
  {
    if(it == m_top_it)
      repair_order(cmp);
    iterator_compare<Compare> it_cmp(cmp);
    it = std::max(const_iterator(m_top_it), it, it_cmp);
  }

  /*
   * Visitors
   */
  template <class Visitor> void visit(Visitor v = Visitor()) const
  {
    std::for_each(begin(), end(), v);
  }
  template <class Visitor> void visit_dirty(Visitor v = Visitor())
  {
    std::for_each(begin_dirty(), end_dirty(), v);
  }
  template <class Visitor, class Cmp>
  void visit_guarded_order(Visitor v = Visitor(), Cmp cmp = Cmp())
  {
    dirty_iterator it = m_top_it = begin_dirty();

    if(empty())
      return;

    iterator_compare<Cmp> icmp(cmp);
    for(dirty_iterator end = end_dirty(); it != end; ++it)
    {
      v(*it);
      m_top_it = std::max(m_top_it, it, icmp);
    }
  }

  /*
   * Iterators
   */
  dirty_iterator get_dirty_handle(const_iterator cit)
  {
    return dirty_iter(cit);
  }
  dirty_iterator dirty_iter(const_iterator cit)
  {
    difference_type diff = cit - m_subcontainer.begin();
    return m_subcontainer.begin() + diff;
  }
  const_iterator begin() const { return m_subcontainer.begin(); }
  dirty_iterator begin_dirty() { return m_subcontainer.begin(); }
  const_iterator end() const { return m_subcontainer.end(); }
  dirty_iterator end_dirty() { return m_subcontainer.end(); }
  const_iterator first() const { return m_top_it; }
  dirty_iterator first_dirty() const { return m_top_it; }
  /*
   * Accessors
   */
  const_reference top() const { return *m_top_it; }
  dirty_reference top_dirty() { return *m_top_it; }
  const_reference front() const { return *m_subcontainer.begin(); }
  dirty_reference front_dirty() { return *m_subcontainer.begin(); }
  const_reference back() const { return *(--m_subcontainer.end()); }
  dirty_reference back_dirty() { return *(--m_subcontainer.end()); }
protected:
  subcontainer_type m_subcontainer;
  dirty_iterator m_top_it;
};

template <class Compare = std::less<>> struct queued_set_compare
{
  using element_comparator = Compare;
  explicit queued_set_compare(element_comparator ec = element_comparator())
      : m_compare(std::move(ec))
  {
  }
  template <class OrderedSet>
  bool operator()(OrderedSet const &lhs, OrderedSet const &rhs) const
  {
    if(lhs.empty())
      return true;
    if(rhs.empty())
      return false;
    return m_compare(lhs.top(), rhs.top());
  }

protected:
  element_comparator m_compare;
};

END_NAMESPACE_CORE

#endif
