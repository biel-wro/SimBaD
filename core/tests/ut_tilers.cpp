#include "scale_coord_tiler.hpp"

#include <boost/test/auto_unit_test.hpp>
#include <iostream>

using namespace simbad::core;

BOOST_AUTO_TEST_SUITE(tilers)

using board_coord_type = int;
using space_coord_type = float;
constexpr std::size_t dim = 2;

using my_board_coordinates = coordinates<board_coord_type, dim>;
using my_space_coordinates = coordinates<space_coord_type, dim>;
using my_tiler = scale_coord_tiler<space_coord_type, board_coord_type, dim>;

BOOST_AUTO_TEST_CASE(tiler_simple_test)
{
  my_space_coordinates sc{123.4, -98.76};
  my_tiler tiler(10);
  my_board_coordinates bc = tiler(sc);
  my_board_coordinates expected_result{12, -10};
  BOOST_REQUIRE(bc == expected_result);
}

BOOST_AUTO_TEST_CASE(tiler_small_coords)
{
  my_space_coordinates sc{0.4, -0.76};
  my_tiler tiler(10);
  my_board_coordinates bc = tiler(sc);
  my_board_coordinates expected_result{0, -1};
  BOOST_REQUIRE(bc == expected_result);
}

BOOST_AUTO_TEST_SUITE_END()
