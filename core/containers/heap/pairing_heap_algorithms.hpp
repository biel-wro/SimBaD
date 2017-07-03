#ifndef CORE_PAIRING_ALGORITHMS
#define CORE_PAIRING_ALGORITHMS
#include "core_def.hpp"
#include <algorithm>
#include <assert.h>
#include <stdexcept>

#include <boost/intrusive/circular_list_algorithms.hpp>

#include "binary_tree_algorithms.hpp"
#include "pairing_hook.hpp"
BEGIN_NAMESPACE_CORE

/**
 * This pairing heap implementation is based on a binary tree implemented with
 * only two pointers and a bool value per node. Left child in the binary tree is
 * a first child in the pairing heap. Right child is the next (younger) sibling.
 */

template <class NodeTraits> struct pairing_heap_algorithms
{
  using node_traits = NodeTraits;
  using node = typename NodeTraits::node;
  using node_ptr = typename NodeTraits::node_ptr;
  using const_node_ptr = typename NodeTraits::const_node_ptr;

  using bt_algs = binary_tree_algorithms<NodeTraits>;

  static void init(node_ptr n) { bt_algs::init(n); }
  static node_ptr get_parent(node_ptr n) { return bt_algs::get_parent(n); }
  static node_ptr get_last_child(node_ptr n)
  {
    return bt_algs::get_left_child(n);
  }
  static node_ptr get_older_sibling(node_ptr n)
  {
    return bt_algs::get_right_child(n);
  }
  static void link_last_child(node_ptr n, node_ptr c)
  {
    bt_algs::link_left_child(n, c);
  }
  static void link_older_sibling(node_ptr n, node_ptr c)
  {
    bt_algs::link_right_child(n, c);
  }
  static void unlink_with_older_siblings(node_ptr n)
  {
    bt_algs::unlink_subtree(n);
  }

  static void add_child(node_ptr parent, node_ptr child)
  {
    node_ptr first_child = get_last_child(parent);
    if(nullptr == first_child)
    {
      link_last_child(parent, child);
      return;
    }

    unlink_with_older_siblings(first_child);
    link_last_child(parent, child);
    link_older_sibling(child, first_child);
  }
  static node_ptr pop_child(node_ptr parent)
  {
    node_ptr first_child = get_last_child(parent);
    assert(nullptr != first_child);

    node_ptr second_child = get_older_sibling(first_child);
    unlink_with_older_siblings(first_child);

    if(nullptr == second_child)
      return;

    unlink_with_older_siblings(second_child);
    link_last_child(parent, second_child);
  }

  static node_ptr top(node_ptr root) { return root; }
  template <class Compare>
  static node_ptr merge(node_ptr r1, node_ptr r2, Compare cmp = Compare())
  {
    if(nullptr == r1)
      return r2;
    if(nullptr == r2)
      return r1;

    if(cmp(*r1, *r2))
    {
      add_child(r2, r1);
      return r2;
    }

    add_child(r1, r2);
    return r1;
  }

  template <class Compare>
  static node_ptr merge_pairs(node_ptr first_child, Compare cmp = Compare())
  {
    assert(nullptr != first_child);
    assert(nullptr == get_parent(first_child));

    node_ptr second_child = get_older_sibling(first_child);
    if(nullptr == second_child)
      return first_child;

    node_ptr third_child = get_older_sibling(second_child);

    unlink_with_older_siblings(second_child);
    if(nullptr == third_child)
      return merge(first_child, second_child, cmp);

    unlink_with_older_siblings(third_child);
    node_ptr paired = merge(first_child, second_child, cmp);
    node_ptr other_pairs = merge_pairs(third_child, cmp);
    return merge(paired, other_pairs, cmp);
  }

  template <class Compare>
  static node_ptr pop(node_ptr root, Compare cmp = Compare())
  {
    assert(nullptr != root);
    assert(nullptr == get_older_sibling(root));
    node_ptr first_child = get_last_child(root);
    if(nullptr == first_child)
      return nullptr;

    unlink_with_older_siblings(first_child);
    return merge_pairs(first_child, cmp);
  }

  template <class Compare>
  static void remove_non_root(node_ptr node, Compare cmp = Compare())
  {
    assert(nullptr != get_parent(node));
    node_ptr parent = get_parent(node);
    bool was_last_child = get_last_child(parent) == node;

    unlink_with_older_siblings(node);
    node_ptr older_sibling = get_older_sibling(node);

    // if node has older siblings they must be now disconnected
    if(nullptr != older_sibling)
      unlink_with_older_siblings(older_sibling);

    node_ptr to_be_linked = pop(node, cmp);

    if(nullptr != older_sibling)
    {
      if(nullptr != to_be_linked)
        link_older_sibling(to_be_linked, older_sibling);
      else
        to_be_linked = older_sibling;
    }

    if(nullptr == to_be_linked)
      return;

    if(was_last_child)
      link_last_child(parent, to_be_linked);
    else
      link_older_sibling(parent, to_be_linked);
  }

  template <class Compare>
  static node_ptr remove(node_ptr root, node_ptr node, Compare cmp = Compare())
  {
    if(root == node)
      return pop(root, cmp);

    remove_non_root(node, cmp);
    return root;
  }

  template <class Compare>
  static node_ptr update(node_ptr root, node_ptr node, Compare cmp = Compare())
  {
    root = remove(root, node, cmp);
    root = merge(root, node, cmp);
    return root;
  }

  template <class Visitor> static void visit(node_ptr r, Visitor v = Visitor())
  {
    bt_algs::visit(r, v);
  }
  static std::size_t count_nodes(node_ptr r) { return bt_algs::count_nodes(r); }
  template <class Compare>
  static bool check_heap_property(node_ptr r, Compare cmp = Compare())
  {
    bool ok = true;
    auto checker = [&ok, &cmp](node &n) {

      node_ptr child = get_last_child(&n);
      while(nullptr != child)
      {
        ok &= !cmp(n, *child);
        child = get_older_sibling(child);
      }
    };
    visit(r, checker);
    return ok;
  }
};

END_NAMESPACE_CORE
#endif
