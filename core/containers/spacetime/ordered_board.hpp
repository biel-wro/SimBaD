#ifndef CORE_ORDERED_BOARD_HPP
#define CORE_ORDERED_BOARD_HPP

#include <memory>

#include "ordered_board_decl.hpp"

BEGIN_NAMESPACE_CORE
template <class Traits>
ordered_board<Traits>::ordered_board(size_type nbuckets, key_equal_pred equal,
                                     key_hash_pred hash, data_order_pred order,
                                     rehash_watchdog ar)
    : m_tile_list(),
      m_buckets(new bucket_type[nbuckets]),
      m_heap_root(nullptr),
      m_tile_set(bucket_traits(m_buckets.get(), nbuckets),
                 node_hash_pred(hash), node_equal_pred(equal)),
      m_key_equal_pred(equal),
      m_key_hash_pred(hash),
      m_data_order_pred(order),
      m_auto_rehash(ar),
      m_order_pred(order)

{
}

template <class Traits> ordered_board<Traits>::~ordered_board() { clear(); }
template <class Traits>
void ordered_board<Traits>::ordered_board::rehash(size_type new_size)
{
  std::unique_ptr<bucket_type[]> new_buckets( new bucket_type[new_size]);
  m_tile_set.rehash(bucket_traits(new_buckets.get(), new_size));
  m_buckets.swap(new_buckets);
}

template <class Traits>
typename ordered_board<Traits>::size_type
ordered_board<Traits>::bucket_count() const
{
  return m_tile_set.bucket_count();
}
template <class Traits>
typename ordered_board<Traits>::size_type ordered_board<Traits>::size() const
{
  return m_tile_set.size();
}

template <class Traits> bool ordered_board<Traits>::empty() const
{
  return m_tile_set.empty();
}

template <class Traits>
template <class... Args>
typename ordered_board<Traits>::const_handle_type
ordered_board<Traits>::emplace(Args &&... args)
{
  const_handle_type h = emplace_dirty(std::forward<Args>(args)...);
  return h;
}
template <class Traits>
template <class... Args>
typename ordered_board<Traits>::dirty_handle_type
ordered_board<Traits>::emplace_dirty(Args &&... args)
{
  std::unique_ptr<node_type> ptr(new node_type(std::forward<Args>(args)...));
  std::pair<set_iterator, bool> result = m_tile_set.insert(*ptr);
  if(!result.second)
  {
    throw std::invalid_argument(
        "attempted to insert a tile which already exist");
  }
  node_pointer rptr = ptr.release();

  pairing_heap_algo::init(rptr);
  m_heap_root = pairing_heap_algo::merge(m_heap_root, rptr, m_order_pred);
  m_auto_rehash(this->m_tile_set,m_buckets);
  m_tile_list.push_back(*rptr);
  return dirty_handle_type(rptr);
}

template <class Traits> void ordered_board<Traits>::pop()
{
  assert(nullptr != m_heap_root);
  assert(m_tile_list.size() > 0);
  node_pointer to_delete = m_heap_root;
  m_heap_root = pairing_heap_algo::pop(m_heap_root, m_order_pred);

  m_tile_set.erase(*to_delete);
  m_tile_list.erase(m_tile_list.iterator_to(*to_delete));

  delete to_delete;
}
template <class Traits>
typename ordered_board<Traits>::data_type ordered_board<Traits>::pop_value()
{
  assert(nullptr != m_heap_root);
  assert(m_tile_list.size() > 0);

  node_pointer to_delete = m_heap_root;
  m_heap_root = pairing_heap_algo::pop(m_heap_root, m_order_pred);
  m_tile_set.erase(*to_delete);
  m_tile_list.erase(m_tile_list.iterator_to(*to_delete));

  data_type val = std::move(to_delete->get_data());

  delete to_delete;
  return val;
}

template <class Traits> void ordered_board<Traits>::pop_dirty() { pop(); }
template <class Traits> void ordered_board<Traits>::remove(const_handle_type d)
{
  remove_dirty(d);
}
template <class Traits>
void ordered_board<Traits>::remove_dirty(const_handle_type d)
{
  const_node_pointer ptr = d.get_object_ptr(); //&get_node_reference(d);
  m_tile_set.erase(ptr);
  m_tile_list.erase(ptr);
  m_heap_root = pairing_heap_algo::remove(m_heap_root, ptr, m_order_pred);
  delete ptr;
}

template <class Traits> void ordered_board<Traits>::clear()
{
  m_heap_root = nullptr;
  m_tile_list.clear();
  m_tile_set.clear_and_dispose(deleter_type());
}

template <class Traits>
typename ordered_board<Traits>::const_data_reference
ordered_board<Traits>::top() const
{
  assert(nullptr != m_heap_root);
  assert(!m_tile_list.empty());
  return m_heap_root->get_data();
}
template <class Traits>
typename ordered_board<Traits>::data_reference
ordered_board<Traits>::top_dirty()
{
  assert(nullptr != m_heap_root);
  assert(!m_tile_list.empty());
  return m_heap_root->get_data();
}

template <class Traits>
typename ordered_board<Traits>::const_handle_type
ordered_board<Traits>::first() const
{
  assert(nullptr != m_heap_root);
  assert(!m_tile_list.empty());
  return const_handle_type(m_heap_root);
}
template <class Traits>
typename ordered_board<Traits>::dirty_handle_type
ordered_board<Traits>::first_dirty()
{
  assert(nullptr != m_heap_root);
  assert(!m_tile_list.empty());
  return dirty_handle_type(m_heap_root);
}

template <class Traits>
typename ordered_board<Traits>::const_handle_type
ordered_board<Traits>::find(key_type const &key) const
{
  set_const_iterator it =
      m_tile_set.find(key, m_key_hash_pred, node_equal_pred(m_key_equal_pred));
  if(m_tile_set.end() == it)
    return const_handle_type();
  return const_handle_type(&*it);
}
template <class Traits>
typename ordered_board<Traits>::dirty_handle_type
ordered_board<Traits>::find_dirty(const key_type &key)
{
  set_iterator it =
      m_tile_set.find(key, m_key_hash_pred, node_equal_pred(m_key_equal_pred));

  if(m_tile_set.end() == it)
    return dirty_handle_type();
  return dirty_handle_type(&*it);
}

template <class Traits>
template <class Visitor>
void ordered_board<Traits>::visit(Visitor v) const
{
  for(const_node_reference tile : m_tile_set)
    v(tile.get_data());
}
template <class Traits>
template <class Visitor>
void ordered_board<Traits>::visit_dirty(Visitor v)
{
  for(node_reference tile : m_tile_set)
    v(tile.get_data());
}
template <class Traits>
template <class Visitor>
void ordered_board<Traits>::visit_guarded_order(Visitor v)
{
  for(list_iterator it = m_tile_list.begin(), end = m_tile_list.end();
      it != end; ++it)
  {
    dirty_handle_type h(&*it);
    v(*h);
    repair_order(h);
  }
}

template <class Traits>
template <class KeyGen, class Visitor>
void ordered_board<Traits>::visit_region(KeyGen gen, Visitor visitor) const
{
  for(; gen; ++gen)
  {
    key_type key = *gen;
    const_handle_type handle = find(key);
    if(handle)
      visitor(handle.get_data());
  }
}
template <class Traits>
template <class KeyGen, class Visitor>
void ordered_board<Traits>::visit_region_dirty(KeyGen gen, Visitor visitor)
{
  for(; gen; ++gen)
  {
    key_type key = *gen;
    const_handle_type ch = find(key);
    dirty_handle_type handle = get_dirty_handle(ch);
    if(handle)
      visitor(handle.get_data());
  }
}
template <class Traits>
template <class KeyGen, class Visitor>
void ordered_board<Traits>::visit_region_guarded_order(KeyGen gen,
                                                       Visitor visitor)
{
  for(; gen; ++gen)
  {
    dirty_handle_type handle = get_dirty_handle(find(*gen));
    if(!handle)
      continue;
    visitor(handle.get_data());
    repair_order(handle);
  }
}

template <class Traits> void ordered_board<Traits>::repair_order()
{
  m_heap_root = nullptr;
  for(node_reference ref : m_tile_list)
  {
    pairing_heap_algo::init(&ref);
    m_heap_root = pairing_heap_algo::merge(m_heap_root, &ref, m_order_pred);
  }
}

template <class Traits>
void ordered_board<Traits>::repair_order(const_handle_type v)
{
  m_heap_root = pairing_heap_algo::update(
      m_heap_root, get_dirty_handle(v).get_object_ptr(), m_order_pred);
}

template <class Traits>
template <class KeyGenerator>
void ordered_board<Traits>::repair_region_order(KeyGenerator gen)
{
  for(; gen; ++gen)
  {
    key_type key(*gen);
    dirty_handle_type handle = find(key);
    if(handle)
      repair_order(handle);
  }
}

template <class Traits>
typename ordered_board<Traits>::dirty_handle_type
ordered_board<Traits>::get_dirty_handle(const_handle_type ch)
{
  return dirty_handle_type(const_cast<node_pointer>(ch.get_object_ptr()));
}

template <class Traits> bool ordered_board<Traits>::check_order()
{
  return pairing_heap_algo::check_heap_property(m_heap_root, m_order_pred);
}

END_NAMESPACE_CORE

#endif
