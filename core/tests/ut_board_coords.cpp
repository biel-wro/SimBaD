#include <boost/test/auto_unit_test.hpp>


#include <random>
#include <type_traits>

#include "coordinates.hpp"

typedef simbad::core::coordinates<int, 2> tile_coord_type;

using wrapped_incrementer =
    simbad::core::coord_incrementer_wrapped<tile_coord_type>;
using infinite_incrementer =
    simbad::core::coord_incrementer_box<tile_coord_type>;

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
  for (int i = 0; i < 36; i++)
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
  for (int i = 0; i < 36; i++)
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
  for (int i = 0; i < 36; i++)
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
  for (int i = 0; i < 36; i++)
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
  for (int i = 0; i < 36; i++)
  {
    BOOST_REQUIRE(r);
    BOOST_REQUIRE(tc == stack.back());
    stack.pop_back();

    r = incrementer.prev(tc);
  }
  BOOST_TEST_CHECKPOINT("leaving the loop");
  BOOST_REQUIRE(!r);
}
