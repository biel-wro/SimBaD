#include <boost/test/auto_unit_test.hpp>

#include <functional>
#include <iostream>
#include <random>
#include <type_traits>

#include "coordinates/coord_generators.hpp"
#include "coordinates/coordinates.hpp"

typedef simbad::core::coordinates<int, 2> tile_coord_type;
using float_coord_type = simbad::core::coordinates<float, 3>;

using wrapped_incrementer =
    simbad::core::coord_incrementer_wrapped<tile_coord_type>;
using infinite_incrementer =
    simbad::core::coord_incrementer_box<tile_coord_type>;

using box_generator = simbad::core::coord_generator_box<tile_coord_type>;
BOOST_AUTO_TEST_SUITE(coords)

BOOST_AUTO_TEST_CASE(instantiate)
{
  tile_coord_type tc;
  (void)tc;
}

BOOST_AUTO_TEST_CASE(coord_wrapped_simple_pass)
{
  tile_coord_type tc;
  tc.fill(0);
  std::array<int, 36> arr;
  arr.fill(false);
  tile_coord_type rmin, rmax, bmin, bmax;

  rmin.fill(0);
  rmax.fill(5);
  bmin.fill(-10);
  bmax.fill(+10);

  bool r = true;

  wrapped_incrementer incrementer(rmin, rmax, bmin, bmax);

  BOOST_TEST_CHECKPOINT("entering the loop");
  for(int i = 0; i < 36; i++)
  {
    // std::cout<<tc[0] <<","<<tc[1]<<":"<<r<<std::endl;

    int &v = arr[tc[0] * 6 + tc[1]];

    BOOST_REQUIRE_EQUAL(v, false);
    v = true;

    BOOST_REQUIRE(r);
    r = incrementer.next(tc);
  }
  BOOST_TEST_CHECKPOINT("leaving the loop");
  BOOST_REQUIRE(!r);
}

BOOST_AUTO_TEST_CASE(coord_infinite_simple_pass)
{
  tile_coord_type tc;
  tc.fill(0);
  std::array<int, 36> arr;
  arr.fill(false);
  tile_coord_type rmin, rmax;

  rmin.fill(0);
  rmax.fill(5);

  bool r = true;

  infinite_incrementer incrementer(rmin, rmax);

  BOOST_TEST_CHECKPOINT("entering the loop");
  for(int i = 0; i < 36; i++)
  {
    // std::cout<<tc[0] <<","<<tc[1]<<":"<<r<<std::endl;

    int &v = arr[tc[0] * 6 + tc[1]];

    BOOST_REQUIRE_EQUAL(v, false);
    v = true;

    BOOST_REQUIRE(r);
    r = incrementer.next(tc);
  }
  BOOST_TEST_CHECKPOINT("leaving the loop");
  BOOST_REQUIRE(!r);
}

BOOST_AUTO_TEST_CASE(coord_wrap_pass)
{
  tile_coord_type tc;
  tc.fill(2);

  tile_coord_type rmin, rmax, bmin, bmax;

  rmin.fill(+2);
  rmax.fill(-2);
  bmin.fill(-4);
  bmax.fill(+4);

  wrapped_incrementer incrementer(rmin, rmax, bmin, bmax);
  bool r = true;
  BOOST_TEST_CHECKPOINT("entering the loop");
  for(int i = 0; i < 36; i++)
  {
    BOOST_REQUIRE(r);
    r = incrementer.next(tc);
    // std::cout<<tc[0] <<","<<tc[1]<<":"<<r<<std::endl;
  }
  BOOST_TEST_CHECKPOINT("leaving the loop");
  BOOST_REQUIRE(!r);
}

BOOST_AUTO_TEST_CASE(coord_go_and_back)
{
  std::vector<tile_coord_type> stack;
  tile_coord_type tc;

  tile_coord_type rmin, rmax, bmin, bmax;

  rmin.fill(+2);
  rmax.fill(-2);
  bmin.fill(-4);
  bmax.fill(+4);

  wrapped_incrementer incrementer(rmin, rmax, bmin, bmax);
  tc = rmin;
  bool r = true;
  BOOST_TEST_CHECKPOINT("entering the loop");
  for(int i = 0; i < 36; i++)
  {
    BOOST_REQUIRE(r);
    stack.push_back(tc);
    r = incrementer.next(tc);
  }
  BOOST_TEST_CHECKPOINT("leaving the loop");
  BOOST_REQUIRE(!r);

  tc = rmax;
  r = true;
  BOOST_TEST_CHECKPOINT("entering the loop");
  for(int i = 0; i < 36; i++)
  {
    BOOST_REQUIRE(r);
    BOOST_REQUIRE(tc == stack.back());
    stack.pop_back();

    r = incrementer.prev(tc);
  }
  BOOST_TEST_CHECKPOINT("leaving the loop");
  BOOST_REQUIRE(!r);
}

BOOST_AUTO_TEST_CASE(box_coord_generator)
{
  tile_coord_type rmin{0, 0};
  tile_coord_type rmax{4, 4};

  box_generator gen(rmin, rmax);
  std::vector<bool> visited(25, false);

  while(!gen.is_end())
  {
    tile_coord_type c = *gen;
    ++gen;
    size_t tileno = c[0] + 5 * c[1];
    BOOST_REQUIRE(!visited[tileno]);
    visited[tileno] = true;
  }

  BOOST_REQUIRE(
      std::all_of(visited.begin(), visited.end(), [](bool b) { return b; }));
}

BOOST_AUTO_TEST_CASE(box_coord_generator_bound_by_ref)
{
  tile_coord_type rmin{0, 0};
  tile_coord_type rmax{4, 4};

  box_generator gen(std::cref(rmin), std::cref(rmax));
  std::vector<bool> visited(25, false);

  while(!gen.is_end())
  {
    tile_coord_type c = *gen;
    ++gen;
    size_t tileno = c[0] + 5 * c[1];
    BOOST_REQUIRE(!visited[tileno]);
    visited[tileno] = true;
  }

  BOOST_REQUIRE(
      std::all_of(visited.begin(), visited.end(), [](bool b) { return b; }));
}

BOOST_AUTO_TEST_CASE(wrapped_coord_generator)
{
  tile_coord_type rmin{4, 4};
  tile_coord_type rmax{1, 1};
  tile_coord_type bmin{0, 0};
  tile_coord_type bmax{5, 5};

  simbad::core::coord_generator_grid<tile_coord_type> gen(rmin, rmax, bmin,
                                                          bmax);
  std::vector<bool> visited(36, false);

  int visited_count = 0;
  while(!gen.is_end())
  {
    tile_coord_type c = *gen;
    ++gen;
    size_t tileno = c[0] + 6 * c[1];
    BOOST_REQUIRE(!visited[tileno]);
    visited[tileno] = true;
    visited_count++;
  }

  BOOST_CHECK(visited[0]);
  BOOST_CHECK(visited[1]);
  BOOST_CHECK(visited[6]);
  BOOST_CHECK(visited[7]);

  BOOST_CHECK(visited[4]);
  BOOST_CHECK(visited[5]);
  BOOST_CHECK(visited[10]);
  BOOST_CHECK(visited[11]);

  BOOST_CHECK(visited[24]);
  BOOST_CHECK(visited[25]);
  BOOST_CHECK(visited[30]);
  BOOST_CHECK(visited[31]);

  BOOST_CHECK(visited[28]);
  BOOST_CHECK(visited[29]);
  BOOST_CHECK(visited[34]);
  BOOST_CHECK(visited[35]);
  BOOST_REQUIRE_EQUAL(visited_count, 16);
}

BOOST_AUTO_TEST_CASE(distance)
{
  float_coord_type c1 = {1.0f, 2.0, 3};
  float_coord_type c2 = {3, 2.0, 1.0f};

  BOOST_REQUIRE_CLOSE(c1.distance_square_to(c2), 8.0f, 0.0001);
}
BOOST_AUTO_TEST_SUITE_END()
