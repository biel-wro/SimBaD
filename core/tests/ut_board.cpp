#include "board.hpp"
#include "board_impl.hpp"
#include "board_tile.hpp"
#include "coordinates.hpp"

#include <boost/test/auto_unit_test.hpp>

#include <random>
#include <type_traits>

//#include <model.h>

using int_board = simbad::core::board<int, 2>;
using string_board = simbad::core::board<std::string, 2>;

using int_coords = int_board::coordinates_type;

BOOST_AUTO_TEST_CASE(board_init)
{

  int_board ib;
  string_board sb;

  string_board sb2(std::move(sb));
  ib.begin();
}

BOOST_AUTO_TEST_CASE(board_emplace_and_sweep)
{
  int_board ib;

  const unsigned TEST_SIZE = 1024;

  std::srand(0);
  for (unsigned i = 0; i < TEST_SIZE; ++i)
  {
    int x = std::rand() % 10;
    int y = std::rand() % 10;
    typename int_board::coordinates_type c;
    c[0] = x;
    c[1] = y;
    // std::cerr<< c[0]<<" "<< c[1]<< std::endl;
    ib.emplace(c, i);
  }

  std::vector<bool> visited;
  visited.assign(TEST_SIZE, false);

  int_board const &cib(ib);

  auto it = cib.begin();
  for (unsigned i = 0; i < TEST_SIZE; ++i)
  {
    unsigned idx = *it;
    BOOST_REQUIRE(!visited[idx]);
    visited[idx] = true;
    ++it;
  }

  for (bool b : visited)
    BOOST_REQUIRE(b);
}

BOOST_AUTO_TEST_CASE(board_remove)
{
  const unsigned int TEST_SIZE = 1024 * 16;
  const unsigned AREA = 10;

  int_board ib;

  for (unsigned i = 0; i < TEST_SIZE; ++i)
  {
    std::srand(i);
    int x = std::rand() % AREA;
    int y = std::rand() % AREA;

    int_coords c = {x, y};
    ib.emplace(c, i);
  }

  std::vector<bool> visited;
  visited.assign(TEST_SIZE, false);

  for (unsigned i = 0; i < TEST_SIZE; ++i)
  {
    int &v = *ib.begin();
    BOOST_REQUIRE(!visited[v]);
    visited[v] = true;

    srand(v);
    int x = std::rand() % AREA;
    int y = std::rand() % AREA;

    int_coords c{{x, y}};

    auto it = ib.iterator_to(c, v);
    BOOST_REQUIRE(*it == v);

    ib.remove(c, v);
  }

  for (bool b : visited)
    BOOST_REQUIRE(b);
}

BOOST_AUTO_TEST_CASE(board_box_iteration)
{
  const unsigned int TEST_SIZE = 1024;
  const unsigned AREA = 10;

  int_board ib;
  int_coords min{{2, 3}}, max{{5, 6}};

  std::vector<int_coords> assigned_tile(TEST_SIZE);
  std::vector<bool> within_box(TEST_SIZE, false);
  std::vector<bool> visited(TEST_SIZE, false);

  for (unsigned i = 0; i < TEST_SIZE; ++i)
  {
    std::srand(i);
    int x = std::rand() % AREA;
    int y = std::rand() % AREA;

    if (min[0] <= x && x <= max[0] && min[1] <= y && y <= max[1])
      within_box[i] = true;

    int_coords c = {x, y};
    assigned_tile[i] = c;
    ib.emplace(c, i);
  }

  for (int idx : ib.get_box(min, max))
  {
    BOOST_REQUIRE(!visited[idx]);
    BOOST_REQUIRE(within_box[idx]);
    visited[idx] = true;
  }

  for (unsigned i = 0; i < TEST_SIZE; ++i)
    BOOST_REQUIRE(visited[i] == within_box[i]);
}

BOOST_AUTO_TEST_CASE(board_box_visitation)
{
  const unsigned int TEST_SIZE = 1024;
  const unsigned AREA = 10;

  int_board ib;
  int_coords min{{2, 3}}, max{{5, 6}};

  std::vector<int_coords> assigned_tile(TEST_SIZE);
  std::vector<bool> within_box(TEST_SIZE, false);

  for (unsigned i = 0; i < TEST_SIZE; ++i)
  {
    std::srand(i);
    int x = std::rand() % AREA;
    int y = std::rand() % AREA;

    if (min[0] <= x && x <= max[0] && min[1] <= y && y <= max[1])
      within_box[i] = true;

    int_coords c = {x, y};
    assigned_tile[i] = c;
    ib.emplace(c, i);
  }

  const int_board &cib(ib);
  simbad::core::coord_incrementer_box<int_coords> ci(min, max);

  std::vector<bool> const_visited(TEST_SIZE, false);

  cib.visit_region(ci, [&const_visited, &within_box](int const &v) {
    BOOST_REQUIRE(!const_visited[v]);
    BOOST_REQUIRE(within_box[v]);
    const_visited[v] = true;
  });

  std::vector<bool> visited(TEST_SIZE, false);

  ib.visit_region(ci, [&visited, &within_box](int const &v) {
    BOOST_REQUIRE(!visited[v]);
    BOOST_REQUIRE(within_box[v]);
    visited[v] = true;
  });

  for (unsigned i = 0; i < TEST_SIZE; ++i)
    BOOST_REQUIRE(visited[i] == within_box[i]);
}
