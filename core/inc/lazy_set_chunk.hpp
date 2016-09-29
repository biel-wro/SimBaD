#ifndef LAZYSETCHUNK_H
#define LAZYSETCHUNK_H

#include <boost/iterator/iterator_adaptor.hpp>

#include "core_def.hpp"

#include <algorithm>
#include <algorithm>
#include <array>
#include <assert.h>
#include <cstddef>
#include <functional>
#include <inttypes.h>
#include <memory>

BEGIN_NAMESPACE_CORE

constexpr size_t DEFAULT_CHUNK_SIZE = 1000;

template <class D, size_t chunk_size = DEFAULT_CHUNK_SIZE>
class LazySetChunk
{

public:
  /*
   * Types
   */
  using value_type = D;
  using reference_type = D &;
  using const_reference_type = D const &;
  using array_type = std::array<char, sizeof(D) * chunk_size>;
  using emulated_array_type = std::array<D, chunk_size>;
  using size_type = size_t;

  using iterator = typename emulated_array_type::iterator;
  using const_iterator = typename emulated_array_type::const_iterator;

  /*
   * Constructors, destructors, assigments
   */

  LazySetChunk() noexcept : occupied(0) {}
  LazySetChunk(LazySetChunk const &) = delete;
  LazySetChunk(LazySetChunk &&) = default;

  LazySetChunk &operator=(LazySetChunk const &) = delete;
  LazySetChunk &operator=(LazySetChunk &&) = default;

  ~LazySetChunk()
  {
    while (occupancy() > 0)
    {
      pop_back();
    }
  }

  emulated_array_type &getEmulatedArray()
  {
    return *reinterpret_cast<emulated_array_type *>(&array);
  }

  emulated_array_type const &getEmulatedArray() const
  {
    return *reinterpret_cast<emulated_array_type const *>(&array);
  }

  /*
   * Modifiers
   */

  template <class... Args>
  iterator emplace_back(Args &&... args)
  {
    new (getPlace(occupied)) D(std::forward<Args>(args)...);
    iterator it = begin() + occupied;
    ++occupied;

    return it;
  }

  void pop_back()
  {
    assert(!isEmpty());
    at(occupied - 1).~D();
    --occupied;
  }

  void clear()
  {
    for (size_t occ = occupancy(); occ > 0; --occ)
      pop_back();
  }

  /*
   * status queries
   */
  size_type occupancy() const { return occupied; }
  constexpr static size_type length() { return chunk_size; }

  bool isFull() const { return occupied == chunk_size; }

  bool isEmpty() const { return occupied == 0; }

  bool containsByAddress(D const *ptr) const
  {
    if (std::greater_equal<D const *>()(ptr, getEmulatedArray().data()) &&
        std::less_equal<D const *>()(ptr, getEmulatedArray().data() +
                                              this->length() - 1))
      return true;
    else
      return false;
  }

  size_type findIdx(const_reference_type p) const
  {
    return &p - getEmulatedArray().data();
  }

  iterator find(const_reference_type p)
  {
    if (!containsByAddress(&p))
      return end();
    else
      return begin() + findIdx(p);
  }

  const_iterator find(const_reference_type p) const
  {
    if (!containsByAddress(&p))
      return end();
    else
      return begin() + findIdx(p);
  }

  /*
   * Element accessors
   */

  reference_type back()
  {
    assert(!isEmpty());
    return at(occupied - 1);
  }
  const_reference_type back() const
  {
    assert(!isEmpty());
    return at(occupied - 1);
  }

  reference_type at(size_type idx)
  {
    assert(idx < occupied);
    return *reinterpret_cast<D *>(getPlace(idx));
  }

  const_reference_type at(size_type idx) const
  {
    assert(idx < occupied);
    return *reinterpret_cast<const D *>(getPlace(idx));
  }

  /*
   * Visitors
   */
  template <class Self, class Visitor>
  static void s_visit(Self &self_ref, Visitor v)
  {
    std::for_each(self_ref.begin(), self_ref.end(), v);
  }

  template <typename Visitor>
  void visit(Visitor v = Visitor())
  {
    s_visit(*this, v);
  }

  template <typename ConstVisitor>
  void visit(ConstVisitor v = ConstVisitor()) const
  {
    s_visit(*this, v);
  }

  /*
   * Iterators retrievers
   */

  iterator begin() { return getEmulatedArray().begin(); }
  const_iterator begin() const { return getEmulatedArray().begin(); }

  iterator end() { return getEmulatedArray().begin() + occupancy(); }

  const_iterator end() const
  {
    return getEmulatedArray().begin() + occupancy();
  }

protected:
  char *getPlace(size_type idx) { return &array.at(idx * sizeof(D)); }

  const char *getPlace(size_type idx) const
  {
    return &array.at(idx * sizeof(D));
  }

private:
  size_type occupied;
  array_type array;
};

END_NAMESPACE_CORE

#endif
