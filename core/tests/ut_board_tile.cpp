#include "board_coordinates.hpp"
#include "board_tile.hpp"

#include <boost/test/auto_unit_test.hpp>

#include <random>
#include <type_traits>


using tile_coord_type = simbad::core::board_coordinates<int, 2>;
using tile_type = simbad::core::board_tile<int, tile_coord_type>;

BOOST_AUTO_TEST_CASE(board_tile_fill)
{
    std::array<int, 2> coords = {{1, 2}};
    tile_type tile(coords);
    const int TEST_SIZE = 1024 * 7;

    for (int i = 0; i < TEST_SIZE; ++i)
        tile.emplace_back(i);

    auto it = tile.begin();
    tile_type const &ctile = tile;
    auto cit = ctile.begin();

    for (int i = 0; i < TEST_SIZE; ++i)
    {
        BOOST_REQUIRE_EQUAL(*it, i);
        BOOST_REQUIRE_EQUAL(*cit, i);
        ++it;
        ++cit;
    }
    BOOST_REQUIRE(it == tile.end());
    BOOST_REQUIRE(cit == tile.end());
}
