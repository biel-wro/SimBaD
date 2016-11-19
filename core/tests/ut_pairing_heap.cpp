#include <boost/test/auto_unit_test.hpp>

#include <algorithm>
#include <numeric>
#include <random>

#include "heap/pairing_heap_algorithms.hpp"

struct MyNode
{
  MyNode(double t = 0) : t(t) {}
  MyNode *left = nullptr;
  MyNode *right = nullptr;
  bool has_left;
  double t;
};

struct MyCompare
{
  bool operator()(MyNode const &n1, MyNode const &n2) const
  {
    return n1.t > n2.t;
  }
};

struct MyNodeTraits
{
  using node = MyNode;
  using node_ptr = MyNode *;
  using const_node_ptr = MyNode const *;

  static node_ptr get_left(const_node_ptr n) { return n->left; }
  static void set_left(node_ptr n, node_ptr c) { n->left = c; }
  static node_ptr get_right(const_node_ptr n) { return n->right; }
  static void set_right(node_ptr n, node_ptr l) { n->right = l; }
  static bool get_has_left(const_node_ptr n) { return n->has_left; }
  static void set_has_left(node_ptr n, bool r) { n->has_left = r; }
};
using algo = simbad::core::pairing_heap_algorithms<MyNodeTraits>;

BOOST_AUTO_TEST_SUITE(pairing_heap_algorithms)

BOOST_AUTO_TEST_CASE(top_access)
{
  MyNode n;
  algo::init(&n);
  BOOST_REQUIRE_EQUAL(algo::top(&n), &n);
}

BOOST_AUTO_TEST_CASE(null_non_null_merge)

{
  MyNode n;
  algo::init(&n);
  BOOST_REQUIRE_EQUAL(algo::merge<MyCompare>(nullptr, &n), &n);
  BOOST_REQUIRE(algo::check_heap_property<MyCompare>(&n));
  BOOST_REQUIRE_EQUAL(algo::count_nodes(&n), 1);
}
BOOST_AUTO_TEST_CASE(non_null_null_merge)
{
  MyNode n;
  algo::init(&n);
  BOOST_REQUIRE_EQUAL(algo::merge<MyCompare>(&n, nullptr), &n);
  BOOST_REQUIRE(algo::check_heap_property<MyCompare>(&n));
  BOOST_REQUIRE_EQUAL(algo::count_nodes(&n), 1);
}
BOOST_AUTO_TEST_CASE(smaller_greater_merge)
{
  MyNode n1(1), n2(2);
  algo::init(&n1);
  algo::init(&n2);
  BOOST_REQUIRE_EQUAL(algo::merge<MyCompare>(&n1, &n2), &n1);
  BOOST_REQUIRE_EQUAL(n1.left, &n2);
  BOOST_REQUIRE(algo::check_heap_property<MyCompare>(&n1));
  BOOST_REQUIRE_EQUAL(algo::count_nodes(&n1), 2);
}

BOOST_AUTO_TEST_CASE(greater_smaller_merge)
{
  MyNode n1(2), n2(1);
  algo::init(&n1);
  algo::init(&n2);
  MyNode *merged = algo::merge<MyCompare>(&n1, &n2);
  BOOST_REQUIRE_EQUAL(merged, &n2);
  BOOST_REQUIRE_EQUAL(n2.left, &n1);
  BOOST_REQUIRE(algo::check_heap_property<MyCompare>(&n2));
  BOOST_REQUIRE_EQUAL(algo::count_nodes(&n2), 2);
}

BOOST_AUTO_TEST_CASE(merge_few_single)
{
  int NNODES = 10;
  std::vector<MyNode> nodes(NNODES);
  std::for_each(nodes.begin(), nodes.end(), [](MyNode &n) { algo::init(&n); });

  MyNode *root = nullptr;
  double mint = 100;
  for(int i = 0; i < NNODES; ++i)
  {
    double t = std::rand() % 100;
    mint = std::min(t, mint);
    nodes[i].t = t;

    BOOST_TEST_CHECKPOINT("before merge i=" << i << " t=" << t);
    root = algo::merge<MyCompare>(root, &nodes[i]);
    BOOST_TEST_CHECKPOINT("after merge i=" << i << " t=" << t);
    std::size_t count = algo::count_nodes(root);
    BOOST_REQUIRE_EQUAL(count, i + 1);
  }

  BOOST_REQUIRE_LE(root->t, root->left->t);
  BOOST_REQUIRE_EQUAL(root->t, mint);

  BOOST_REQUIRE(algo::check_heap_property<MyCompare>(root));
}

BOOST_AUTO_TEST_CASE(merge_trees)
{
  MyCompare cmp;
  MyNode *tree1 = nullptr;
  MyNode n1(4), n2(8), n3(10);
  algo::init(&n1);
  algo::init(&n2);
  algo::init(&n3);
  tree1 = algo::merge(tree1, &n1, cmp);
  tree1 = algo::merge(tree1, &n2, cmp);
  tree1 = algo::merge(tree1, &n3, cmp);
  BOOST_REQUIRE(algo::check_heap_property(tree1, cmp));
  BOOST_REQUIRE_EQUAL(algo::count_nodes(tree1), 3);

  MyNode *tree2 = nullptr;
  MyNode n4(2), n5(3), n6(6);
  algo::init(&n4);
  algo::init(&n5);
  algo::init(&n6);
  tree2 = algo::merge(tree2, &n4, cmp);
  tree2 = algo::merge(tree2, &n5, cmp);
  tree2 = algo::merge(tree2, &n6, cmp);
  BOOST_REQUIRE(algo::check_heap_property(tree2, cmp));
  BOOST_REQUIRE_EQUAL(algo::count_nodes(tree2), 3);

  MyNode *root = algo::merge(tree1, tree2, cmp);
  BOOST_REQUIRE(algo::check_heap_property(root, cmp));

  BOOST_REQUIRE_EQUAL(algo::count_nodes(root), 6);
}

BOOST_AUTO_TEST_CASE(sort)
{
  int NNODES = 1000;
  std::vector<MyNode> nodes(NNODES);

  for(int i = 0; i < NNODES; ++i)
    nodes[i].t = i, algo::init(&nodes[i]);

  std::mt19937_64 rng;
  MyNode *root = nullptr;
  std::shuffle(nodes.begin(), nodes.end(), rng);

  for(int i = 0; i < NNODES; ++i)
  {
    root = algo::merge<MyCompare>(root, &nodes[i]);
    BOOST_REQUIRE(algo::check_heap_property<MyCompare>(root));
    BOOST_REQUIRE_EQUAL(algo::count_nodes(root), i + 1);
  }

  std::vector<MyNode> output;
  for(int i = 0; i < NNODES; ++i)
  {
    output.push_back(*algo::top(root));
    BOOST_REQUIRE_EQUAL(algo::count_nodes(root), NNODES - i);

    root = algo::pop<MyCompare>(root);

    BOOST_REQUIRE_EQUAL(algo::count_nodes(root), NNODES - i - 1);
    assert(algo::check_heap_property<MyCompare>(root));
    BOOST_REQUIRE(algo::check_heap_property<MyCompare>(root));
  }

  BOOST_REQUIRE(std::is_sorted(output.rbegin(), output.rend(), MyCompare()));
}

BOOST_AUTO_TEST_CASE(remove_all)
{
  int NNODES = 10;
  std::vector<MyNode> nodes(NNODES);

  for(int i = 0; i < NNODES; ++i)
    nodes[i].t = i, algo::init(&nodes[i]);

  std::mt19937_64 rng;
  MyNode *root = nullptr;
  std::shuffle(nodes.begin(), nodes.end(), rng);

  for(MyNode &n : nodes)
    root = algo::merge<MyCompare>(root, &n);

  for(int i = 0; i < NNODES; ++i)
  {
    root = algo::remove(root, &nodes[i], MyCompare());
    BOOST_REQUIRE_EQUAL(algo::count_nodes(root), NNODES - i - 1);
    BOOST_REQUIRE(algo::check_heap_property<MyCompare>(root));
  }
}

BOOST_AUTO_TEST_CASE(full_update)
{
  int NNODES = 10;
  std::vector<MyNode> nodes(NNODES);

  for(int i = 0; i < NNODES; ++i)
    nodes[i].t = i, algo::init(&nodes[i]);

  std::mt19937_64 rng;
  MyNode *root = nullptr;
  std::shuffle(nodes.begin(), nodes.end(), rng);

  for(MyNode &n : nodes)
    root = algo::merge<MyCompare>(root, &n);

  std::vector<int> updates(NNODES);
  std::iota(updates.begin(), updates.end(), 0);
  std::shuffle(updates.begin(), updates.end(), rng);

  for(int i = 0; i < NNODES; ++i)
  {
    nodes[i].t = updates[i];
    root = algo::update(root, &nodes[i], MyCompare());
    BOOST_REQUIRE_EQUAL(algo::count_nodes(root), NNODES);
    BOOST_REQUIRE(algo::check_heap_property<MyCompare>(root));
  }

  std::vector<MyNode> output;
  for(int i = 0; i < NNODES; ++i)
  {
    output.push_back(*algo::top(root));
    BOOST_REQUIRE_EQUAL(algo::count_nodes(root), NNODES - i);

    root = algo::pop<MyCompare>(root);

    BOOST_REQUIRE_EQUAL(algo::count_nodes(root), NNODES - i - 1);
    BOOST_REQUIRE(algo::check_heap_property<MyCompare>(root));
  }

  BOOST_REQUIRE(std::is_sorted(output.rbegin(), output.rend(), MyCompare()));
}

BOOST_AUTO_TEST_SUITE_END()
