#include <boost/test/auto_unit_test.hpp>

#include <algorithm>
#include <random>

#include "heap/binary_tree_algorithms.hpp"
struct MyProperNode
{
  MyProperNode *left;
  MyProperNode *right;
  bool has_left;
};

struct MyProperNodeTraits
{
  using node = MyProperNode;
  using node_ptr = MyProperNode *;
  using const_node_ptr = MyProperNode const *;
  static node_ptr get_left(node_ptr n) { return n->left; }
  static void set_left(node_ptr n, node_ptr l) { n->left = l; }
  static node_ptr get_right(node_ptr n) { return n->right; }
  static void set_right(node_ptr n, node_ptr r) { n->right = r; }
  static bool get_has_left(node_ptr n) { return n->has_left; }
  static void set_has_left(node_ptr n, bool v) { n->has_left = v; }
};

struct MyHackedNode
{
  MyHackedNode *left;
  MyHackedNode *right;
};

struct MyHackedNodeTraits
{
  using node = MyHackedNode;
  using node_ptr = MyHackedNode *;
  using const_node_ptr = MyHackedNode const *;
  using intptr = std::uintptr_t;

  static node_ptr get_left(node_ptr n)
  {
    std::uintptr_t intrepr = reinterpret_cast<intptr>(n->left);
    intrepr &= ~intptr(1);
    return reinterpret_cast<node_ptr>(intrepr);
  }
  static void set_left(node_ptr n, node_ptr l)
  {
    std::uintptr_t intrepr = reinterpret_cast<intptr>(l);
    intrepr |= reinterpret_cast<intptr>(n->left) & 1;
    n->left = reinterpret_cast<node_ptr>(intrepr);
  }
  static node_ptr get_right(node_ptr n) { return n->right; }
  static void set_right(node_ptr n, node_ptr r) { n->right = r; }
  static bool get_has_left(node_ptr n)
  {
    return reinterpret_cast<intptr>(n->left) & 1;
  }
  static void set_has_left(node_ptr n, bool v)
  {
    intptr intrepr = reinterpret_cast<intptr>(n->left);
    intrepr &= ~intptr(1);
    intrepr |= v ? 1 : 0;
    n->left = reinterpret_cast<node_ptr>(intrepr);
  }
};

using MyTreeNode = MyHackedNode;
using algo = simbad::core::binary_tree_algorithms<MyHackedNodeTraits>;

static bool check_ok(MyTreeNode *root)
{
  bool global_ok = true;
  auto checker = [&global_ok](MyTreeNode &n) {
    bool ok = true;
    MyTreeNode *parent = algo::get_parent(&n);
    if(nullptr != parent)
      ok &= &n == algo::get_left_child(parent) ||
            &n == algo::get_right_child(parent);

    MyTreeNode *left_child = algo::get_left_child(&n);
    if(nullptr != left_child)
      ok &= &n == algo::get_parent(left_child);

    MyTreeNode *right_child = algo::get_right_child(&n);
    if(nullptr != right_child)
      ok &= &n == algo::get_parent(right_child);
    global_ok &= ok;
  };

  algo::visit(root, checker);
  return global_ok;
}

BOOST_AUTO_TEST_SUITE(binary_tree)

BOOST_AUTO_TEST_CASE(empty_tree)
{
  MyTreeNode *root = nullptr;
  BOOST_REQUIRE_EQUAL(algo::count_nodes(root), 0);
  BOOST_REQUIRE(check_ok(root));
}

BOOST_AUTO_TEST_CASE(single_node)
{
  MyTreeNode n;
  algo::init(&n);

  BOOST_REQUIRE(nullptr == algo::get_left(&n));
  BOOST_REQUIRE(nullptr == algo::get_right(&n));
  BOOST_REQUIRE(!algo::get_has_left(&n));
  BOOST_REQUIRE_EQUAL(algo::count_nodes(&n), 1);
  BOOST_REQUIRE(check_ok(&n));
}

BOOST_AUTO_TEST_CASE(left_child)
{
  MyTreeNode p, c;
  algo::init(&p);
  algo::init(&c);

  algo::link_left_child(&p, &c);

  BOOST_REQUIRE(nullptr == algo::get_parent(&p));
  BOOST_REQUIRE(&c == algo::get_left_child(&p));
  BOOST_REQUIRE(nullptr == algo::get_right_child(&p));

  BOOST_REQUIRE(&p == algo::get_parent(&c));
  BOOST_REQUIRE(nullptr == algo::get_left_child(&c));
  BOOST_REQUIRE(nullptr == algo::get_right_child(&c));

  BOOST_REQUIRE_EQUAL(algo::count_nodes(&p), 2);
  BOOST_REQUIRE(check_ok(&p));
}

BOOST_AUTO_TEST_CASE(right_child)
{
  MyTreeNode p, c;
  algo::init(&p);
  algo::init(&c);

  algo::link_right_child(&p, &c);

  BOOST_REQUIRE(nullptr == algo::get_parent(&p));
  BOOST_REQUIRE(nullptr == algo::get_left_child(&p));
  BOOST_REQUIRE(&c == algo::get_right_child(&p));

  BOOST_REQUIRE(&p == algo::get_parent(&c));
  BOOST_REQUIRE(nullptr == algo::get_left_child(&c));
  BOOST_REQUIRE(nullptr == algo::get_right_child(&c));

  BOOST_REQUIRE_EQUAL(algo::count_nodes(&p), 2);
  BOOST_REQUIRE(check_ok(&p));
}

BOOST_AUTO_TEST_CASE(both_children)
{
  MyTreeNode p, l, r;
  algo::init(&p);
  algo::init(&l);
  algo::init(&r);

  algo::link_left_child(&p, &l);
  algo::link_right_child(&p, &r);

  BOOST_REQUIRE(nullptr == algo::get_parent(&p));
  BOOST_REQUIRE(&l == algo::get_left_child(&p));
  BOOST_REQUIRE(&r == algo::get_right_child(&p));

  BOOST_REQUIRE(&p == algo::get_parent(&l));
  BOOST_REQUIRE(nullptr == algo::get_left_child(&l));
  BOOST_REQUIRE(nullptr == algo::get_right_child(&l));

  BOOST_REQUIRE(&p == algo::get_parent(&r));
  BOOST_REQUIRE(nullptr == algo::get_left_child(&r));
  BOOST_REQUIRE(nullptr == algo::get_right_child(&r));

  BOOST_REQUIRE_EQUAL(algo::count_nodes(&p), 3);
  BOOST_REQUIRE(check_ok(&p));
}

static void link_at_random(std::mt19937_64 &rng, MyTreeNode *root,
                           MyTreeNode *child)
{
  MyTreeNode *parent = root;
  size_t rand_bit;

  while(true)
  {
    MyTreeNode *spot = parent;
    rand_bit = rng() & 1;
    spot =
        rand_bit ? algo::get_left_child(parent) : algo::get_right_child(parent);
    if(nullptr == spot)
      break;
    parent = spot;
  }

  if(rand_bit)
    algo::link_left_child(parent, child);
  else
    algo::link_right_child(parent, child);
}

BOOST_AUTO_TEST_CASE(dynamic_tree)
{
  std::mt19937_64 rand;

  std::size_t NNODES = 173;
  std::vector<MyTreeNode> nodes(NNODES);
  std::for_each(nodes.begin(), nodes.end(),
                [](MyTreeNode &n) { algo::init(&n); });

  MyTreeNode *root = &nodes[0];

  for(size_t i = 1; i < NNODES; ++i)
  {
    link_at_random(rand, root, &nodes[i]);
    BOOST_REQUIRE_EQUAL(algo::count_nodes(root), i + 1);
    BOOST_REQUIRE(check_ok(root));
  }

  for(size_t i = NNODES / 2; i < NNODES; ++i)
  {
    MyTreeNode *node = &nodes[i];
    algo::unlink_subtree(node);

    std::size_t root_sz = algo::count_nodes(root);
    std::size_t sub_sz = algo::count_nodes(node);
    BOOST_REQUIRE_EQUAL(root_sz + sub_sz, NNODES);
    BOOST_REQUIRE(check_ok(root));
    BOOST_REQUIRE(check_ok(node));
    link_at_random(rand, root, node);
    BOOST_REQUIRE(check_ok(root));
  }
}

BOOST_AUTO_TEST_SUITE_END()
