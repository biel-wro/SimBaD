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
      m_nbuckets(nbuckets),
      m_buckets(new bucket_type[m_nbuckets]),
      m_heap_root(nullptr),
      m_tile_set(bucket_traits(m_buckets.get(), m_nbuckets),
                 node_hash_pred(hash), node_equal_pred(equal)),
      m_key_equal_pred(equal),
      m_key_hash_pred(hash),
      m_data_order_pred(order),
      m_auto_rehash(ar),
      m_order_pred(order)

{
}
template <class Traits>
void ordered_board<Traits>::ordered_board::rehash(ordered_board::size_type sz)
{
  throw std::runtime_error("not implemented yet");
}

template <class Traits>
typename ordered_board<Traits>::size_type
ordered_board<Traits>::bucket_count() const
{
  return m_nbuckets;
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
  return emplace_dirty(std::forward<Args>(args)...);
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
  m_auto_rehash(*this);
  m_tile_list.push_back(*rptr);
  return dirty_handle_type(rptr);
}

template <class Traits> void ordered_board<Traits>::pop() { pop_dirty(); }
template <class Traits> void ordered_board<Traits>::pop_dirty()
{
  node_pointer to_delete = m_heap_root;
  m_tile_set.erase(*m_heap_root);
  m_tile_list.erase(m_tile_list.iterator_to(*m_heap_root));
  m_heap_root = pairing_heap_algo::pop(m_heap_root, m_order_pred);
  delete to_delete;
}

template <class Traits> void ordered_board<Traits>::remove(const_handle_type d)
{
  remove_dirty(d);
}
template <class Traits>
void ordered_board<Traits>::remove_dirty(const_handle_type d)
{
  const_node_pointer ptr = &get_node_reference(d);
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
  return m_heap_root->get_data();
}
template <class Traits>
typename ordered_board<Traits>::data_reference
ordered_board<Traits>::top_dirty()
{
  return m_heap_root->get_data();
}

template <class Traits>
typename ordered_board<Traits>::const_handle_type
ordered_board<Traits>::first() const
{
  return const_handle_type(m_heap_root);
}
template <class Traits>
typename ordered_board<Traits>::dirty_handle_type
ordered_board<Traits>::first_dirty()
{
  return dirty_handle_type(m_heap_root);
}

template <class Traits>
typename ordered_board<Traits>::const_handle_type
ordered_board<Traits>::find(key_type const &key) const
{
  set_const_iterator it = m_tile_set.find(key, m_key_hash_pred,
                          node_equal_pred(m_key_equal_pred));
  if(m_tile_set.end() == it)
    return const_handle_type();
  return const_handle_type(&*it);
}
template <class Traits>
typename ordered_board<Traits>::dirty_handle_type
ordered_board<Traits>::find_dirty(const key_type &key)
{
  // using return_type = boost::optional<data_reference>;
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
    const_handle_type handle = find(*gen);
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
    dirty_handle_type handle = get_dirty_handle(find(*gen));
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
  for(node_reference ref : m_tile_set)
  {
    pairing_heap_algo::init(&ref);
    m_heap_root =
        pairing_heap_algo::merge(m_heap_root, &ref, m_order_pred);
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

template <class Traits>
typename ordered_board<Traits>::node_reference
ordered_board<Traits>::get_node_reference(data_reference vr)
{
  return node_type::node_from_data(vr);
}
template <class Traits>
typename ordered_board<Traits>::const_node_reference
ordered_board<Traits>::get_node_reference(const_data_reference vr) const
{
  return node_type::node_from_data(vr);
}

END_NAMESPACE_CORE

#endif
