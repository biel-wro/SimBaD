#ifndef CORE_SPACETIME_HPP
#define CORE_SPACETIME_HPP

#include "spacetime_decl.hpp"

BEGIN_NAMESPACE_CORE

template <class SpacetimeTraits>
spacetime<SpacetimeTraits>::spacetime(order_pred po, tiler tiler,
                                      tile_key_equal tke, tile_key_hash tkh)
    : m_order_pred(po),
      m_tiler(tiler),
      m_nparticles(0),
      m_board(1024, std::move(tke), std::move(tkh),
              typename ordered_board_traits::data_order_pred(po),
              typename ordered_board_traits::auto_rehash())
{
}
template <class Traits>
template <class... Args>
typename spacetime<Traits>::const_handle_type
spacetime<Traits>::emplace(tile_key const &tk, Args... args)
{
  dirty_handle_type h = emplace_dirty(tk, std::forward<Args>(args)...);
  dirty_subset_handle sh = h.get_subset_handle();
  dirty_tile_handle th = h.get_tile_handle();
  th->repair_order(sh, m_order_pred);
  //  h.get_tile_handle()->repair_order(h.get_subset_handle());
  m_board.repair_order(h.get_tile_handle());

  return h;
}

template <class Traits>
template <class... Args>
typename spacetime<Traits>::dirty_handle_type
spacetime<Traits>::emplace_dirty(tile_key const &tk, Args... args)
{
  dirty_tile_handle tile_handle = m_board.find_dirty(tk);

  if(!tile_handle)
    tile_handle = m_board.emplace_dirty(tk);

  dirty_subset_handle set_handle =
      tile_handle->emplace_dirty(std::forward<Args>(args)...);
  m_nparticles++;
  return dirty_handle_type(tile_handle, set_handle);
}

template <class Traits> void spacetime<Traits>::pop()
{
  dirty_tile_handle th = m_board.first_dirty();
  th->pop(m_order_pred);
  if(th->empty())
    m_board.pop();
  m_nparticles--;
}
template <class Traits> void spacetime<Traits>::pop_dirty() { pop(); }
template <class Traits> void spacetime<Traits>::remove(const_handle_type h)
{
  remove_dirty(h);
  m_board.repair_order(h.get_tile_handle());
}
template <class Trait> void spacetime<Trait>::remove_dirty(const_handle_type h)
{
  const_tile_handle th = h.get_tile_handle();
  th->remove(h.get_subset_handle());
  if(th->empty())
    m_board.remove(th);

  m_nparticles--;
}

template <class Traits>
typename spacetime<Traits>::const_reference spacetime<Traits>::top() const
{
  return m_board.top().top();
}
template <class Traits>
typename spacetime<Traits>::reference spacetime<Traits>::top_dirty()
{
  return m_board.top_dirty().top_dirty();
}

template <class Traits>
typename spacetime<Traits>::const_handle_type spacetime<Traits>::first() const
{
  const_tile_handle th = m_board.first();
  const_subset_handle sh = th->first();
  return const_handle_type(th, sh);
}
template <class Traits>
typename spacetime<Traits>::dirty_handle_type spacetime<Traits>::first_dirty()
{
  return get_dirty_handle(first());
}

template <class Traits>
typename spacetime<Traits>::size_type spacetime<Traits>::size() const
{
  return m_nparticles;
}
template <class Traits> bool spacetime<Traits>::empty() const
{
  return 0 == m_nparticles;
}

template <class Traits>
void spacetime<Traits>::repair_order(const_handle_type h)
{
  dirty_handle_type handle = get_dirty_handle(h);
  handle.get_tile_handle()->repair_time(handle.get_subset_handle());
  m_board.repair_order(handle.get_tile_handle());
}
template <class Traits>
template <class KeyGenerator>
void spacetime<Traits>::repair_region_order(KeyGenerator gen)
{
  for(; gen; ++gen)
  {
    tile_key key(*gen);
    dirty_handle_type tile_handle = m_board.find(key);
    if(!tile_handle)
      continue;
    tile_handle->repair_order(m_order_pred);
    m_board.repair_order(tile_handle);
  }
}

template <class Traits>
template <class Visitor>
void spacetime<Traits>::visit(Visitor v) const
{
  m_board.visit([v](subset_type const &ss) { ss.visit(v); });
}
template <class Traits>
template <class Visitor>
void spacetime<Traits>::visit_dirty(Visitor v)
{
  m_board.visit_dirty([v](subset_type &ss) { ss.visit_dirty(v); });
}
template <class Traits>
template <class Visitor>
void spacetime<Traits>::visit_guarded_order(Visitor v)
{
  m_board.visit_guarded_order([v](subset_type &ss) { ss.visit_guarded_order(v); });
}

template <class Traits>
template <class KeyGenerator, class Visitor>
void spacetime<Traits>::visit_region(KeyGenerator gen, Visitor v) const
{
  m_board.visit_region(gen, [v](subset_type &ss) { ss.visit(v); });
}
template <class Traits>
template <class KeyGenerator, class Visitor>
void spacetime<Traits>::visit_region_dirty(KeyGenerator gen, Visitor v)
{
  m_board.visit_region_dirty(gen, [v](subset_type &ss) { ss.visit_dirty(v); });
}
template <class Traits>
template <class KeyGenerator, class Visitor>
void spacetime<Traits>::visit_region_guarded_order(KeyGenerator gen, Visitor v)
{
  m_board.visit_region_guarded_order(
      gen, [v](subset_type &ss) { ss.visit_guarded_order(v); });
}

template <class Traits>
typename spacetime<Traits>::dirty_handle_type
spacetime<Traits>::get_dirty_handle(const_handle_type ch)
{
  dirty_tile_handle th = m_board.get_dirty_handle(ch.get_tile_handle());
  dirty_subset_handle sh = th->get_dirty(ch.get_subset_handle());
  return dirty_handle_type(th, sh);
}

END_NAMESPACE_CORE

#endif // SPACETIME_HPP
