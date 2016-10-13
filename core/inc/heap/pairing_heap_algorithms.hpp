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
 *
 * Each node stores a pointer to its child (or nullptr if it doesn't have
 * children of its own.
 *  - get_first_child(const_node_ptr) and
 *  - set_first_child_ptr(const_node_ptr,const_node_ptr)
 * should provide those operations.
 *
 * The second pointer can be
 *  - a nullptr - means the node is a root node and has no siblings
 *  - a pointer to its next sibling iff get_is_last_child(n) == false
 *  - a pointer to its parent iff get_is_last_child(n) == true
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
        to_be_linked = link_older_sibling(to_be_linked, older_sibling);
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

    remove_non_root(node);
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

/*
template <class NodeTraits> struct pairing_heap_algorithms
{
  using node_traits = NodeTraits;
  using node = typename NodeTraits::node;
  using node_ptr = typename NodeTraits::node_ptr;
  using const_node_ptr = typename NodeTraits::const_node_ptr;


  template <class Compare>
  static bool check_heap_property(node_ptr r, Compare cmp = Compare())
  {
    if(nullptr == r)
      return true;

    node_ptr first_child = node_traits::get_first_child(r);
    if(nullptr == first_child)
      return true;

    node_ptr child = first_child;
    while(true)
    {
      if(cmp(*r, *child))
        return false;
      if(!check_heap_property(child, cmp))
        return false;
      if(node_traits::get_is_last_child(child))
        break;
      child = node_traits::get_next(child);
    }

    return true;
  }

  static node_ptr top(node_ptr root)
  {
    assert(nullptr != root);
    assert(nullptr == node_traits::get_next(root));
    return root;
  }

  static void init(node_ptr n)
  {
    node_traits::set_first_child(n, nullptr);
    node_traits::set_next(n, nullptr);
  }

  static bool inited(node_ptr n)
  {
    return node_traits::get_first_child(n) == nullptr &&
           node_traits::get_next(n) == nullptr;
  }

  static void init_leaf(node_ptr n, node_ptr parent)
  {
    node_traits::set_first_child(n, nullptr);
    node_traits::set_parent(n, parent);
  }

  template <class Compare>
  static node_ptr merge(node_ptr r1, node_ptr r2, Compare cmp = Compare())
  {
    if(nullptr == r1)
      return r2;
    if(nullptr == r2)
      return r1;

    // assert(!is_root(r1));
    assert(check_heap_property(r1, cmp));

    // assert(!is_root(r2));
    assert(check_heap_property(r2, cmp));

    node_ptr made_root, made_child;
    if(cmp(*r1, *r2))
    {
      made_root = r2;
      made_child = r1;
    }
    else
    {
      made_root = r1;
      made_child = r2;
    }

    add_child(made_root, made_child);

    return made_root;
  }

  template <class Compare>
  static node_ptr merge_pairs(node_ptr first_child, Compare cmp = Compare())
  {
    // if there is only one element on the list return it
    if(node_traits::get_is_last_child(first_child))
      return first_child;

    // otherwise get next one and disconnect the first
    node_ptr second_child = node_traits::get_next(first_child);
    node_traits::set_next(first_child, nullptr);

    // check if there still are more elements on the list
    // and disconnect the second
    node_ptr next = node_traits::get_next(second_child);
    bool has_more_siblings = !node_traits::get_is_last_child(second_child);
    node_traits::set_next(second_child, nullptr);

    // now we merge the two first elements
    node_ptr pair = merge(first_child, second_child, cmp);

    // if there was no more elements we are done
    if(!has_more_siblings)
      return pair;

    // otherwise get the result from the remaining elements
    node_ptr merged_pairs = merge_pairs(next, cmp);

    // and merge it with our pair
    return merge(pair, merged_pairs, cmp);
  }

  template <class Compare>
  static node_ptr pop(node_ptr root, Compare cmp = Compare())
  {
    if(nullptr == root)
      throw std::logic_error("attempted to pop an empty heap");

    node_ptr first_child = node_traits::get_first_child(root);

    if(nullptr == first_child)
      return nullptr;

    node_ptr new_root = merge_pairs<Compare>(first_child, cmp);
    node_traits::set_next(new_root, nullptr);
    return new_root;
  }

  template <class Compare>
  static node_ptr unlink(node_ptr root, node_ptr n, Compare cmp = Compare())
  {
    if(root == n)
      return pop(root, cmp);

    node_ptr parent = get_parent_quick(root, n);
    node_ptr first_sibling = node_traits::get_first_child(parent);

    // if node to be unlinked is a first child
    if(first_sibling == n)
    {
      pop_child(parent);
      node_traits::set_next(n, nullptr);
      return root;
    }

    // if node is not connected to root and is not root's first child
    if(parent != root)
    {
      // then it must be the second and the last child
      assert(node_traits::get_next(first_sibling) == n);
      assert(node_traits::get_is_last_child(n));
      assert(node_traits::get_next(n) == parent);

      set_parent(first_sibling, parent);

      node_traits::set_next(n, nullptr);
      return n;
    }

    // node must be second or further child of the root node
    node_ptr prev_sibling = first_sibling;
    node_ptr sibling = node_traits::get_next(prev_sibling);

    assert(prev_sibling != n);
    assert(sibling != n);

    while(!node_traits::get_is_last_child(sibling))
    {
      if(n != sibling)
      {
        prev_sibling = sibling;
        sibling = node_traits::get_next(sibling);
        continue;
      }

      node_ptr next_sibling = node_traits::get_next(sibling);

      set_sibling(next_sibling, prev_sibling);
      node_traits::set_next(n, nullptr);
      return;
    }

    assert(false); // node is not in the heap or heap corrupted
    return root;
  }

  template <class Compare>
  static node_ptr update(node_ptr root, node_ptr n, Compare cmp = Compare())
  {
    root = unlink(root, n, cmp);
    root = merge(root, n, cmp);
    return root;
  }
};
*/
END_NAMESPACE_CORE
#endif
