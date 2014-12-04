#define BOOST_TEST_MAIN

#include <boost/test/included/unit_test.hpp>

#include <containters/board_impl.h>
#include <containters/board.h>

#include <type_traits>
#include <random>

#include <model.h>

typedef std::integral_constant<int,2> _2D;
typedef simbad::core::board_coordinates<int,_2D> tile_coord_type;

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

struct my_config{
  typedef int                                 board_coord;
  typedef std::integral_constant<int,2>       int_dimension;
  typedef std::integral_constant<int,1000>    int_chunk_size;
};

typedef simbad::core::board_tile<int,my_config> tile_type;

BOOST_AUTO_TEST_CASE( board_tile_fill )
{
  std::array<int,2> coords = {1,2};
  tile_type tile(coords);
  const int TEST_SIZE = 1024*7;

  for( int i=0; i<TEST_SIZE; ++i)
  {
    tile.emplace_back(i);
  }

  auto it = tile.begin();
  tile_type const &ctile = tile;
  auto cit = tile.begin();

  for( int i=0; i<TEST_SIZE; ++i)
  {
    BOOST_REQUIRE_EQUAL( *it, i );
    BOOST_REQUIRE_EQUAL( *cit, i );
    ++it;
    ++cit;
  }
  BOOST_REQUIRE( it == tile.end() );
  BOOST_REQUIRE( cit == tile.end() );
}

typedef simbad::core::board<int,2> int_board;
typedef simbad::core::board<std::string,2> string_board;
typedef int_board::coordinates_type int_coords;

BOOST_AUTO_TEST_CASE( board_init )
{


  int_board ib;
  string_board sb;

  ib.begin();
}

BOOST_AUTO_TEST_CASE( board_emplace_and_sweep )
{
  int_board ib;

  const unsigned TEST_SIZE = 1024;

  std::srand(0);
  for( unsigned i=0; i<TEST_SIZE; ++i )
  {
    int x = std::rand() % 10;
    int y = std::rand() % 10;
    typename int_board::coordinates_type c;
    c[0]=x; c[1]=y;
    //std::cerr<< c[0]<<" "<< c[1]<< std::endl;
    ib.emplace(c,i);
  }

  std::vector<bool> visited;
  visited.assign(TEST_SIZE,false);

  int_board const &cib(ib);

  auto it = cib.begin();
  for( unsigned i=0; i<TEST_SIZE; ++i )
  {
    unsigned idx = *it;
    BOOST_REQUIRE( !visited[idx] );
    visited[idx] = true;
    ++it;
  }

  for( bool b: visited )
    BOOST_REQUIRE(b);
}

BOOST_AUTO_TEST_CASE( board_remove )
{
  const unsigned int TEST_SIZE = 1024 * 16;
  const unsigned AREA = 10;

  int_board ib;


  for( unsigned i=0; i < TEST_SIZE; ++i )
  {
    std::srand(i);
    int x = std::rand() % AREA;
    int y = std::rand() % AREA;

    int_coords c = {x,y};
    ib.emplace(c,i);
  }

  std::vector<bool> visited;
  visited.assign(TEST_SIZE,false);

  for( unsigned i=0; i<TEST_SIZE; ++i )
  {
    int &v = *ib.begin();
    BOOST_REQUIRE( !visited[v] );
    visited[v] = true;

    srand(v);
    int x = std::rand() % AREA;
    int y = std::rand() % AREA;

    int_coords c{ {x,y} };

    auto it = ib.iterator_to(c,v);
    BOOST_REQUIRE( *it == v );

    ib.remove(c,v);
  }

  for( bool b: visited)
    BOOST_REQUIRE( b );

}


