#define BOOST_TEST_MAIN

#include <boost/test/included/unit_test.hpp>
#include <string>

#include "containters/lazy_set_chunk.h"

typedef simbad::core::LazySetChunk<std::string> string_chunk;
typedef simbad::core::LazySetChunk<unsigned> int_chunk;

BOOST_AUTO_TEST_CASE( construct )
{
   string_chunk schk;
   (void) schk;
   int_chunk ichk;
   (void) ichk;
}

BOOST_AUTO_TEST_CASE( fill_and_pop )
{
    string_chunk schk;
    int_chunk ichk;

    BOOST_CHECK( ichk.isEmpty());
    BOOST_CHECK( schk.isEmpty());

    for( unsigned i=0; i<ichk.length(); ++i )
    {
        BOOST_CHECK_EQUAL( ichk.occupancy(), i );

        ichk.emplace_back( i );
        BOOST_CHECK_EQUAL( ichk.back(), i );

        schk.emplace_back( std::to_string(i) );
        BOOST_CHECK_EQUAL( schk.back(), std::to_string(i) );

        BOOST_CHECK_EQUAL( ichk.occupancy(), i+1 );
    }

    BOOST_CHECK( ichk.isFull() );
    BOOST_CHECK( schk.isFull() );

    for( unsigned i=0; i<ichk.length(); ++i)
    {
        BOOST_REQUIRE_EQUAL(ichk.at(i), i);
        BOOST_REQUIRE_EQUAL(schk.at(i), std::to_string(i) );
    }

    for( unsigned i=ichk.length()-1; i > 0; --i)
    {
        BOOST_REQUIRE_EQUAL(ichk.back(), i);
        ichk.pop_back();

        BOOST_REQUIRE_EQUAL(schk.back(), std::to_string(i) );
        schk.pop_back();
    }
    ichk.pop_back();
    schk.pop_back();

    BOOST_CHECK( ichk.isEmpty() );
    BOOST_CHECK( schk.isEmpty() );
}

BOOST_AUTO_TEST_CASE( iterators )
{
    string_chunk chk;

    for( unsigned i=0; i<chk.length(); ++i )
    {
        chk.emplace_back( std::to_string(i) );
    }

    auto it = chk.begin();

    for( unsigned i=0; i<chk.length(); ++i, ++it)
    {
        BOOST_CHECK_NE( it, chk.end() );
        BOOST_CHECK_EQUAL( *it, std::to_string(i) );
    }
    BOOST_CHECK_EQUAL( it, chk.end() );
}

BOOST_AUTO_TEST_CASE( find )
{
    string_chunk chk;

    for( unsigned i=0; i<chk.length(); ++i )
    {
        chk.emplace_back( std::to_string(i) );
    }

    auto it = chk.begin();
    for( unsigned i=0; i<chk.length(); ++i, ++it)
    {
        BOOST_CHECKPOINT("i=" << i);

        std::string *ptr = &*it;
        BOOST_CHECK( chk.containsByAddress(ptr) );

        unsigned idx = chk.findIdx( *ptr );
        BOOST_CHECK_LT( idx, chk.occupancy() );
        BOOST_CHECK_EQUAL( idx, i);

        BOOST_CHECK_EQUAL( chk.begin() + idx, it );
    }

}
