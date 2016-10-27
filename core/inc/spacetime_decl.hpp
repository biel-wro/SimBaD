#ifndef SPACETIME_DECL_HPP
#define SPACETIME_DECL_HPP

#include "core_def.hpp"
#include "ordered_board.hpp"
#include "queued_set.hpp"

#include <cstddef>

BEGIN_NAMESPACE_CORE
template <class TileHandle, class SetHandle> class spacetime_handle
{
  using tile_handle_type = TileHandle;
  using subset_handle_type = SetHandle;
  using particle_type = typename subset_handle_type::value_type;
  struct enabler
  {
  };

public:
  template <class, class> friend class spacetime_handle;
  spacetime_handle(tile_handle_type th, subset_handle_type sh)
      : m_tile_handle(th), m_subset_handle(sh)
  {
  }
  template <class OtherTH, class OtherSH>
  spacetime_handle(spacetime_handle<OtherTH, OtherSH> const &other,
                   typename std::enable_if<
                       std::is_convertible<OtherTH, TileHandle>::value &&
                           std::is_convertible<OtherSH, SetHandle>::value,
                       enabler>::type = enabler())
      : m_tile_handle(other.m_tile_handle),
        m_subset_handle(other.m_subset_handle)
  {
  }
  tile_handle_type get_tile_handle() const { return m_tile_handle; }
  subset_handle_type get_subset_handle() const { return m_subset_handle; }
  particle_type &operator*() const { return *m_subset_handle; }
  particle_type *operator->() const { return m_subset_handle; }
private:
  tile_handle_type m_tile_handle;
  subset_handle_type m_subset_handle;
};

template <class ParticleTraits> class spacetime
{
public:
  using particle_traits = ParticleTraits;
  using particle = typename particle_traits::particle_type;
  using reference = particle &;
  using const_reference = particle const &;

  using order_pred = typename particle_traits::particle_ordering;

  using subset_type = queued_set<particle>;
  using dirty_subset_handle = typename subset_type::dirty_handle_type;
  using const_subset_handle = typename subset_type::const_handle_type;

  using tiler = typename particle_traits::tiler;
  using tile_key = typename particle_traits::tile_key;
  using tile_key_equal = typename particle_traits::tile_key_equal;
  using tile_key_hash = typename particle_traits::tile_key_hash;

  using size_type = std::size_t;

  struct ordered_board_traits
  {
    using key_type = tile_key;
    using data_type = subset_type;
    using data_order_pred = queued_set_compare<order_pred>;

    using key_hash_pred = tile_key_hash;
    using key_equal_pred = tile_key_equal;
    using auto_rehash = auto_rehash<float>;
  };

  using board_type = ordered_board<ordered_board_traits>;

  using dirty_tile_handle = typename board_type::dirty_handle_type;
  using const_tile_handle = typename board_type::const_handle_type;

  using dirty_handle_type =
      spacetime_handle<dirty_tile_handle, dirty_subset_handle>;
  using const_handle_type =
      spacetime_handle<const_tile_handle, const_subset_handle>;

  spacetime(order_pred po = order_pred(), tiler t = tiler(),
            tile_key_equal tke = tile_key_equal(),
            tile_key_hash tkh = tile_key_hash());

  template <class... Args>
  const_handle_type emplace(tile_key const &tk, Args... args);
  template <class... Args>
  dirty_handle_type emplace_dirty(tile_key const &tk, Args... args);

  void pop();
  void pop_dirty();

  void remove(const_handle_type handle);
  void remove_dirty(const_handle_type handle);

  const_reference top() const;
  reference top_dirty();

  const_handle_type first() const;
  dirty_handle_type first_dirty();

  size_type size() const;
  bool empty() const;

  void repair_order(const_handle_type h);
  template <class KeyGenerator> void repair_region_order(KeyGenerator gen);

  template <class Visitor> void visit(Visitor v = Visitor()) const;
  template <class Visitor> void visit_dirty(Visitor v = Visitor());
  template <class Visitor> void visit_guarded_order(Visitor v = Visitor());

  template <class KeyGenerator, class Visitor>
  void visit_region(KeyGenerator gen = KeyGenerator(),
                    Visitor v = Visitor()) const;
  template <class KeyGenerator, class Visitor>
  void visit_region_dirty(KeyGenerator gen = KeyGenerator(),
                          Visitor v = Visitor());
  template <class KeyGenerator, class Visitor>
  void visit_region_guarded_order(KeyGenerator gen = KeyGenerator(),
                                  Visitor v = Visitor());

  dirty_handle_type get_dirty_handle(const_handle_type ch);

protected:
  order_pred m_order_pred;
  tiler m_tiler;
  size_type m_nparticles;
  board_type m_board;
};

END_NAMESPACE_CORE

#endif // SPACETIME_DECL_HPP
