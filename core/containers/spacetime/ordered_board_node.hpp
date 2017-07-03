#ifndef CORE_ORDERED_BOARD_NODE_HPP
#define CORE_ORDERED_BOARD_NODE_HPP
#include <type_traits>

#include <boost/intrusive/list_hook.hpp>
#include <boost/intrusive/unordered_set_hook.hpp>

#include "coordinates/coordinates.hpp"
#include "core_def.hpp"
#include "utils/parent_from_member.hpp"

BEGIN_NAMESPACE_CORE

template <class OrderedTile> struct ordered_board_node_pairing_heap_traits
{
  using node = OrderedTile;
  using node_ptr = node *;
  using const_node_ptr = node const *;

  static node_ptr get_left(const_node_ptr n) { return n->m_left; }
  static void set_left(node_ptr n, node_ptr c) { n->m_left = c; }
  static node_ptr get_right(const_node_ptr n) { return n->m_right; }
  static void set_right(node_ptr n, node_ptr l) { n->m_right = l; }
  static bool get_has_left(const_node_ptr n) { return n->m_has_left; }
  static void set_has_left(node_ptr n, bool r) { n->m_has_left = r; }
};

template <class Key, class Data>
struct ordered_board_node : public boost::intrusive::unordered_set_base_hook<>,
                            public boost::intrusive::list_base_hook<>
{
public:
  using key_type = Key;
  using key_reference = key_type &;
  using const_key_reference = key_type const &;

  using data_type = Data;
  using data_reference = data_type &;
  using const_data_reference = data_type const &;
  using data_pointer = Data *;
  using const_data_pointer = Data const *;

  using unordered_set_hook = boost::intrusive::unordered_set_base_hook<>;
  using list_hook = boost::intrusive::list_base_hook<>;

  using self_type = ordered_board_node<key_type, data_type>;
  using self_reference = self_type &;
  using const_self_reference = self_type const &;

  explicit ordered_board_node(key_type key = key_type())
      : m_key(std::move(key)), m_data()
  {
  }

  template <class... Args>
  explicit ordered_board_node(key_type key, Args &&... args)
      : m_key(std::move(key)), m_data(std::forward<Args>(args)...)
  {
  }
  friend struct ordered_board_node_pairing_heap_traits<ordered_board_node>;

  const_key_reference get_key() const { return m_key; }
  key_reference get_key() { return m_key; }
  const_data_reference get_data() const { return m_data; }
  data_reference get_data() { return m_data; }
  /*
  static const_self_reference node_from_data(const_data_reference cref)
  {
    return *parent_from_member(&cref, &self_type::m_data);
  }
  static self_reference node_from_data(data_reference ref)
  {
    return *parent_from_member(&ref, &self_type::m_data);
  }*/

private:
  key_type m_key;
  data_type m_data;

  bool m_has_left;
  ordered_board_node *m_left, *m_right;
};

template <class Node> class ordered_board_node_handle
{
  struct enabler
  {
  };

public:
  using tile_type = Node;
  using data_type = typename tile_type::data_type;
  using data_reference =
      typename std::conditional<std::is_const<tile_type>::value,
                                typename tile_type::const_data_reference,
                                typename tile_type::data_reference>::type;
  using data_pointer =
      typename std::conditional<std::is_const<tile_type>::value,
                                typename tile_type::const_data_pointer,
                                typename tile_type::data_pointer>::type;
  using key_type = typename tile_type::key_type;
  using key_reference =
      typename std::conditional<std::is_const<tile_type>::value,
                                typename tile_type::const_key_reference,
                                typename tile_type::key_reference>::type;

  explicit ordered_board_node_handle(tile_type *ptr = nullptr) : m_ptr(ptr) {}
  template <class OtherNode>
  ordered_board_node_handle(
      ordered_board_node_handle<OtherNode> const &ot,
      typename std::enable_if<std::is_convertible<OtherNode *, Node *>::value,
                              enabler>::type = enabler())
      : ordered_board_node_handle(ot.get_object_ptr())
  {
  }
  Node *get_object_ptr() const { return m_ptr; }
  data_reference get_data() const { return m_ptr->get_data(); }
  key_reference get_key() const { return m_ptr->get_key(); }
  data_pointer operator->() const { return &m_ptr->get_data(); }
  data_reference &operator*() const { return m_ptr->get_data(); }
  operator bool() const { return nullptr != m_ptr; }

private:
  Node *m_ptr;
};

template <class CoordHasher = simbad::core::coord_hasher>
struct ordered_board_node_hasher
{
  using coord_hasher = CoordHasher;
  explicit ordered_board_node_hasher(coord_hasher ch = coord_hasher())
      : m_coord_hasher(std::move(ch))
  {
  }
  template <class OrderedTile>
  std::size_t operator()(OrderedTile const &tile) const
  {
    return m_coord_hasher(tile.get_key());
  }

protected:
  coord_hasher m_coord_hasher;
};

template <class Coords, class CoordPred = std::equal_to<>>
struct ordered_board_node_equal
{
  using coord_pred = CoordPred;
  explicit ordered_board_node_equal(coord_pred cp = coord_pred())
      : m_coord_pred(std::move(cp))
  {
  }

  template <class OrderedSet>
  bool operator()(ordered_board_node<Coords, OrderedSet> const &t1,
                  ordered_board_node<Coords, OrderedSet> const &t2) const
  {
    return m_coord_pred(t1.get_key(), t2.get_key());
  }
  template <class OrderedSet>
  bool operator()(ordered_board_node<Coords, OrderedSet> const &t,
                  Coords const &c) const
  {
    return m_coord_pred(t.get_key(), c);
  }
  template <class OrderedSet>
  bool operator()(Coords const &c,
                  ordered_board_node<Coords, OrderedSet> const &t) const
  {
    return m_coord_pred(c, t.get_key());
  }

protected:
  coord_pred m_coord_pred;
};

template <class OrderedSetCompare = std::less<>>
struct ordered_board_node_ordering
{
  using tile_ordering = OrderedSetCompare;
  explicit ordered_board_node_ordering(tile_ordering to = tile_ordering())
      : m_tile_ordering(to)
  {
  }
  template <class OrderedTile>
  bool operator()(OrderedTile const &t1, OrderedTile const &t2) const
  {
    return m_tile_ordering(t1.get_data(), t2.get_data());
  }

protected:
  tile_ordering m_tile_ordering;
};
END_NAMESPACE_CORE
#endif
