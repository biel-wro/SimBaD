#include "coordinates/scale_coord_tiler.hpp"
#include "space/space.hpp"
#include "space/space_impl.hpp"

#include <boost/test/auto_unit_test.hpp>

using namespace simbad::core;
namespace
{
struct my_config
{
  using board_coord_scalar = int;
  using space_coord_scalar = float;

  using dimension_type = size_t;
  static constexpr size_t dimension = 2;

  using chunk_sz_type = uint16_t;
  static constexpr uint16_t chunk_size = 1000;

  using space_coords = coordinates<space_coord_scalar, dimension>;
  using board_coords = coordinates<board_coord_scalar, dimension>;

  using coord_tiler =
      scale_coord_tiler<space_coord_scalar, board_coord_scalar, dimension>;
};
}

using my_space_impl = space_impl<std::string, my_config>;

BOOST_AUTO_TEST_CASE(space_instantiation)
{

  my_space_impl si;
  //    space_impl si;
  //    space s;
}
