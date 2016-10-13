#ifndef BINARY_TREE_ALGORITHMS_HPP
#define BINARY_TREE_ALGORITHMS_HPP

#include <algorithm>
#include <assert.h>
#include <cstddef>

#include "core_def.hpp"

BEGIN_NAMESPACE_CORE

template <class NodeTraits> struct binary_tree_algorithms
{
  using node_traits = NodeTraits;
  using node = typename NodeTraits::node;
  using node_ptr = typename NodeTraits::node_ptr;
  using const_node_ptr = typename NodeTraits::const_node_ptr;

  static node_ptr get_left(node_ptr n) { return node_traits::get_left(n); }
  static void set_left(node_ptr n, node_ptr l) { node_traits::set_left(n, l); }
  static node_ptr get_right(node_ptr n) { return node_traits::get_right(n); }
  static void set_right(node_ptr n, node_ptr r)
  {
    node_traits::set_right(n, r);
  }
  static bool get_has_left(node_ptr n) { return node_traits::get_has_left(n); }
  static void set_has_left(node_ptr n, bool v)
  {
    node_traits::set_has_left(n, v);
  }

  static void init(node_ptr n)
  {
    set_left(n, nullptr);
    set_right(n, nullptr);
    set_has_left(n, false);
  }



  static node_ptr get_parent(node_ptr n)
  {
    if(nullptr == n)
      return nullptr;

    node_ptr right = get_right(n);
    if(nullptr == right)
      return nullptr;

    node_ptr right_left = get_left(right);

    if(n == right_left) // n is the only child
      return right;

    if(nullptr != right_left)
    {
      node_ptr right_left_right = get_right(right_left);
      if(n == right_left_right) // n is the right child
        return right;
    }

    node_ptr right_right = get_right(right);
    assert(nullptr != right_right);
    assert(n == get_left(right_right));
    return right_right;
  }
  static node_ptr get_left_child(node_ptr n)
  {
    node_ptr left = get_left(n);
    if(nullptr == left)
    {
      assert(!get_has_left(n));
      return nullptr;
    }

    if(!get_has_left(n))
    {
      return nullptr;
    }

    assert(n == get_right(left) || n == get_right(get_right(left)));
    return left;
  }
  static node_ptr get_right_child(node_ptr n)
  {
    node_ptr left = get_left(n);
    if(nullptr == left)
      return nullptr;

    node_ptr left_right = get_right(left);
    if(n == left_right) // n has only one child
      return get_has_left(n) ? nullptr : left;

    assert(n == get_right(left_right));
    return left_right;
  }

  static void link_left_child(node_ptr parent, node_ptr child)
  {
    assert(nullptr == get_right(child));
    node_ptr old_child = get_left(parent);

    if(nullptr == old_child) // parent does not have children yet
    {
      set_left(parent, child);
      set_has_left(parent, true);
      set_right(child, parent);
      return;
    }

    // parent has a child but it has to be a right child
    assert(!get_has_left(parent));
    assert(get_right(old_child) == parent);

    set_left(parent, child);
    set_has_left(parent, true);
    set_right(child, old_child);
  }
  static void link_right_child(node_ptr parent, node_ptr child)
  {
    assert(nullptr == get_right(child));
    node_ptr old_child = get_left(parent);
    if(nullptr == old_child) // parent doesn't have children yet
    {
      set_left(parent, child);
      set_has_left(parent, false);
      set_right(child, parent);
      return;
    }

    // parent has a child but it has to be left child
    assert(get_has_left(parent));
    assert(get_right(old_child) == parent);

    set_right(old_child, child);
    set_right(child, parent);
  }
  static void unlink_subtree(node_ptr n)
  {
    node_ptr right = get_right(n);

    // we don't support removal of the root node here
    assert(nullptr != right);

    node_ptr right_left = get_left(right);

    if(n == right_left) // n is the only child of it parent
    {
      node_ptr parent = right;
      set_left(parent, nullptr);
      set_has_left(parent, false);
      set_right(n, nullptr);
      return;
    }

    if(nullptr != right_left)
    {
      node_ptr right_left_right = get_right(right_left);
      if(n == right_left_right) // n is the right child of its parent
      {
        node_ptr parent = right;
        node_ptr left_sibling = right_left;

        assert(get_has_left(parent));

        set_right(left_sibling, parent);
        set_right(n, nullptr);
        return;
      }
    }

    // n is the left child of its parent
    node_ptr right_right = get_right(right);

    assert(nullptr != right_right);
    assert(n == get_left(right_right));

    node_ptr parent = right_right;
    node_ptr right_sibling = right;

    assert(get_has_left(parent));

    set_left(parent, right_sibling);
    set_has_left(parent, false);
    set_right(n, nullptr);
  }

  template <class Visitor> static void visit(node_ptr n, Visitor v = Visitor())
  {
    if(nullptr == n)
      return;
    v(*n);
    visit(get_left_child(n), v);
    visit(get_right_child(n), v);
  }

  static std::size_t count_nodes(node_ptr n)
  {
    size_t sz(0);
    visit(n, [&sz](node &) { ++sz; });
    return sz;
  }


};

END_NAMESPACE_CORE

#endif // BINARY_TREE_ALGORITHMS_HPP
