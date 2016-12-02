#ifndef LAZY_SET_H
#define LAZY_SET_H
#include "core_fwd.hpp"

#include "lazy_set_chunk.hpp"
#include "lazy_set_impl.hpp"

namespace simbad
{
namespace core
{

template <class T, size_t chunk_size = DEFAULT_CHUNK_SIZE>
class LazySet
{
public:
  /*
   * types
   */
  using chunk_type = LazySetChunk<T, chunk_size>;
  using value_type = T;
  using impl = LazySet_impl<chunk_type>;
  using reference_type = typename impl::reference_type;
  using const_reference_type = typename impl::const_reference_type;
  using iterator = typename impl::iterator;
  using const_iterator = typename impl::const_iterator;
  using size_type = typename impl::size_type;

public:
  /*
   * Constructors and assignments
   */
  LazySet() = default;
  LazySet(LazySet const &) = delete;
  LazySet(LazySet &&) = default;

  LazySet &operator=(LazySet const &) = delete;
  LazySet &operator=(LazySet &&) = default;

  /*
   * Accessors
   */
  reference_type front() { return *m_impl.begin(); }
  const_reference_type front() const { return *m_impl.begin(); }

  reference_type back()
  {
    iterator it = m_impl.end();
    return *(--it);
  }

  const_reference_type back() const
  {
    const_iterator it = m_impl.end();
    return *(--it);
  }

  /*
   * Modifiers
   */
  template <class... Args>
  iterator emplace_back(Args &&... args)
  {
    return m_impl.emplace_back(std::forward<Args>(args)...);
  }

  void pop_back() { m_impl.pop_back(); }

  void swap_and_delete(const_reference_type cref)
  {
    std::swap(const_cast<reference_type>(cref), back());
    pop_back();
  }

  void clear()
  {
    m_impl.clear();
  }

  /*
   * Status
   */
  size_t size() const { return m_impl.size(); }
  bool empty() const { return 0 == m_impl.size(); }

  /*
   * Visitors
   */

  template <class Visitor>
  void visit(Visitor v = Visitor())
  {
    m_impl.visit(v);
  }

  template <class ConstVisitor>
  void visit(ConstVisitor v = ConstVisitor()) const
  {
    m_impl.visit(v);
  }

  /*
   * Iterators
   */
  iterator begin() { return m_impl.begin(); }

  iterator end() { return m_impl.end(); }

  const_iterator begin() const { return m_impl.begin(); }

  const_iterator end() const { return m_impl.end(); }

  iterator find(value_type const &v) { return m_impl.find(v); }

  const_iterator find(value_type const &v) const { return m_impl.find(v); }

private:
  impl m_impl;
};
}
}
#endif
