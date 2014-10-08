#define BOOST_TEST_MAIN

#include <boost/test/included/unit_test.hpp>

#include <containters/tile.h>
#include <type_traits>


typedef std::integral_constant<int,2> _2D;
typedef simbad::core::tile_coordinates<int,_2D> tile_coord_type;

BOOST_AUTO_TEST_CASE( instantiate )
{
    tile_coord_type tc;

}

BOOST_AUTO_TEST_CASE( coord_simple_pass )
{
    tile_coord_type tc;
    tc.fill(0);
    std::array<int,36> arr;
    arr.fill(false);

    tile_coord_type rmin,rmax,bmin,bmax;

    rmin.fill(0);
    rmax.fill(5);
    bmin.fill(-10);
    bmax.fill(+10);

    bool r=true;
    BOOST_CHECKPOINT("entering the loop");
    for(int i=0; i<36; i++ )
    {
        //std::cout<<tc[0] <<","<<tc[1]<<":"<<r<<std::endl;

        int &v = arr[tc[0]*6+tc[1]];

        BOOST_REQUIRE_EQUAL(v, false);
        v = true;

        BOOST_REQUIRE(r);
        r = tile_coord_type::next(tc,rmin,rmax,bmin,bmax);

    }
    BOOST_CHECKPOINT("leaving the loop");
    BOOST_REQUIRE(!r);
}


BOOST_AUTO_TEST_CASE( coord_wrap_pass)
{
    tile_coord_type tc;
    tc.fill(2);

    tile_coord_type rmin,rmax,bmin,bmax;

    rmin.fill(+2);
    rmax.fill(-2);
    bmin.fill(-4);
    bmax.fill(+4);

    bool r=true;
    BOOST_CHECKPOINT("entering the loop");
    for(int i=0; i<36; i++ )
    {
        BOOST_REQUIRE(r);
        r = tile_coord_type::next(tc,rmin,rmax,bmin,bmax);
        //std::cout<<tc[0] <<","<<tc[1]<<":"<<r<<std::endl;

    }
    BOOST_CHECKPOINT("leaving the loop");
    BOOST_REQUIRE(!r);
}

BOOST_AUTO_TEST_CASE( coord_go_and_back )
{
    std::vector<tile_coord_type> stack;
    tile_coord_type tc;


    tile_coord_type rmin,rmax,bmin,bmax;

    rmin.fill(+2);
    rmax.fill(-2);
    bmin.fill(-4);
    bmax.fill(+4);

    tc = rmin;
    bool r=true;
    BOOST_CHECKPOINT("entering the loop");
    for(int i=0; i<36; i++ )
    {
        BOOST_REQUIRE(r);
        stack.push_back(tc);
        r = tile_coord_type::next(tc,rmin,rmax,bmin,bmax);


    }
    BOOST_CHECKPOINT("leaving the loop");
    BOOST_REQUIRE(!r);

    tc = rmax;
    r = true;
    BOOST_CHECKPOINT("entering the loop");
    for(int i=0; i<36; i++ )
    {
        BOOST_REQUIRE(r);
        BOOST_REQUIRE( tc == stack.back() );
        stack.pop_back();

        r = tile_coord_type::prev(tc,rmin,rmax,bmin,bmax);


    }
    BOOST_CHECKPOINT("leaving the loop");
    BOOST_REQUIRE(!r);
}
