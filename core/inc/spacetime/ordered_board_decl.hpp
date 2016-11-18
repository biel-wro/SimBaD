#ifndef CORE_ORDERED_BOARD_DECL_HPP
#define CORE_ORDERED_BOARD_DECL_HPP

#include <cstddef>
#include <memory>

#include <boost/intrusive/list.hpp>
#include <boost/intrusive/unordered_set.hpp>
#include <boost/optional.hpp>

#include "core_def.hpp"
#include "heap/pairing_heap_algorithms.hpp"
#include "ordered_tile.hpp"

BEGIN_NAMESPACE_CORE
template <class OrderedBoardTraits> class ordered_board
{
public:
  using ordered_board_traits = OrderedBoardTraits;
  using key_type = typename ordered_board_traits::key_type;
  using data_type = typename ordered_board_traits::data_type;
  using data_reference = data_type &;
  using const_data_reference = data_type const &;

  using node_type = ordered_board_node<key_type, data_type>;

  using node_reference = node_type &;
  using const_node_reference = const node_type &;
  using node_pointer = node_type *;
  using const_node_pointer = node_type const *;

  using key_equal_pred = typename ordered_board_traits::key_equal_pred;
  using key_hash_pred = typename ordered_board_traits::key_hash_pred;
  using data_order_pred = typename ordered_board_traits::data_order_pred;
  using rehash_watchdog = typename ordered_board_traits::rehash_watchdog;

  using node_hash_pred = ordered_board_node_hasher<key_hash_pred>;
  using node_equal_pred = ordered_board_node_equal<key_type, key_equal_pred>;
  using node_order_pred = ordered_board_node_ordering<data_order_pred>;

  using deleter_type = std::default_delete<node_type>;

  using size_type = std::size_t;
  static constexpr const size_t MINIMAL_BUCKET_COUNT = 1024;

  using dirty_handle_type = ordered_board_node_handle<node_type>;
  using const_handle_type = ordered_board_node_handle<const node_type>;

  using tile_list_type = boost::intrusive::list<node_type>;
  using list_iterator = typename tile_list_type::iterator;
  using list_const_iterator = typename tile_list_type::const_iterator;

  using tile_set_type =
      boost::intrusive::unordered_set<node_type,
                                      boost::intrusive::equal<node_equal_pred>,
                                      boost::intrusive::hash<node_hash_pred>>;
  using set_iterator = typename tile_set_type::iterator;
  using set_const_iterator = typename tile_set_type::const_iterator;

  using bucket_type = typename tile_set_type::bucket_type;
  using bucket_traits = typename tile_set_type::bucket_traits;

  using pairing_heap_algo = pairing_heap_algorithms<
      ordered_board_node_pairing_heap_traits<node_type>>;

  // Constructors
  explicit ordered_board(size_type nbuckets = MINIMAL_BUCKET_COUNT,
                         key_equal_pred equal = key_equal_pred(),
                         key_hash_pred hash = key_hash_pred(),
                         data_order_pred order = data_order_pred(),
                         rehash_watchdog ar = rehash_watchdog());

  void rehash(size_type sz);
  size_type bucket_count() const;
  size_type size() const;
  bool empty() const;

  template <class... Args> const_handle_type emplace(Args &&... args);
  template <class... Args> dirty_handle_type emplace_dirty(Args &&... args);
  void pop();
  void pop_dirty();
  void remove(const_handle_type v);
  void remove_dirty(const_handle_type v);
  void clear();

  const_data_reference top() const;
  data_reference top_dirty();
  const_handle_type first() const;
  dirty_handle_type first_dirty();

  const_handle_type find(key_type const &key) const;
  dirty_handle_type find_dirty(key_type const &key);

  template <class Visitor> void visit(Visitor v = Visitor()) const;
  template <class Visitor> void visit_dirty(Visitor v = Visitor());
  template <class Visitor> void visit_guarded_order(Visitor v = Visitor());

  template <class KeyGenerator, class Visitor>
  void visit_region(KeyGenerator generator = KeyGenerator(),
                    Visitor v = Visitor()) const;
  template <class KeyGenerator, class Visitor>
  void visit_region_dirty(KeyGenerator generator = KeyGenerator(),
                          Visitor v = Visitor());
  template <class KeyGenerator, class Visitor>
  void visit_region_guarded_order(KeyGenerator generator = KeyGenerator(),
                                  Visitor v = Visitor());

  void repair_order();
  void repair_order(const_handle_type v);
  template<class KeyGenerator>
  void repair_region_order(KeyGenerator gen);

  dirty_handle_type get_dirty_handle(const_handle_type h);

protected:
  node_reference get_node_reference(data_reference vr);
  const_node_reference get_node_reference(const_data_reference vr) const;

  tile_list_type m_tile_list;

  size_type m_nbuckets;
  std::unique_ptr<bucket_type[]> m_buckets;
  node_pointer m_heap_root;
  tile_set_type m_tile_set;

  key_equal_pred m_key_equal_pred;
  key_hash_pred m_key_hash_pred;
  data_order_pred m_data_order_pred;
  rehash_watchdog m_auto_rehash;
  node_order_pred m_order_pred;
};

END_NAMESPACE_CORE

#endif // ORDERED_BOARD_DECL_HPP
