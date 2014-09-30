#define BOOST_TEST_MAIN

#include <boost/test/included/unit_test.hpp>
#include "fixed/fixed.h"


BOOST_AUTO_TEST_CASE( bases )
{
    typename core::fixed::int_base<32>::type i32;
    typename core::fixed::int_base<32,false>::type ui32;
    (void) i32;
    (void) ui32;

    typename core::fixed::int_base<64>::type i64;
    typename core::fixed::int_base<64,false>::type ui64;
    (void) i64;
    (void) ui64;

    typename core::fixed::int_base<128,true>::type i128;
    (void) i128;
}

typedef core::fixed::Fixed<32,16> fixed32_16;
typedef core::fixed::Fixed<64,32> fixed64_32;

BOOST_AUTO_TEST_CASE( instantiation )
{
    const int val1=16;
    const double val2=16;

    fixed32_16 f1(val1);
    fixed64_32 f2(val2);

    BOOST_CHECK_EQUAL( f1.to<int>(), val1 );
    BOOST_CHECK_EQUAL( f2.to<double>(), val2 );

    f1 = 3.0;
    f2 = 5;
    BOOST_CHECK_EQUAL( f1.to<double>(), 3.0 );
    BOOST_CHECK_EQUAL( f2.to<int>(),    5 );
}

BOOST_AUTO_TEST_CASE( comparison )
{
    fixed32_16 f1(3);
    fixed32_16 f2(5);

    BOOST_CHECK_EQUAL( f1==f2, false );
    BOOST_CHECK_EQUAL( f1!=f2, true );
    BOOST_CHECK_EQUAL( f1<=f2, true );
    BOOST_CHECK_EQUAL( f1 >f2, false );
    BOOST_CHECK_EQUAL( f1>=f2, false );
    BOOST_CHECK_EQUAL( f1 <f2, true );
}

BOOST_AUTO_TEST_CASE( addsub )
{
    fixed32_16 f1(13);
    fixed32_16 f2(11);

    BOOST_CHECK_EQUAL( (f1 + f2).to<int>(), 24 );
    BOOST_CHECK( f1 + f2 - f1 == f2 );

    fixed32_16 f3 = 13 + 5;
    BOOST_CHECK( f1 + 5 == f3 );
    BOOST_CHECK( f3 - 5 == f1 );

}

BOOST_AUTO_TEST_CASE( multiplication )
{
    fixed32_16 f1(8.0);
    fixed32_16 f2(16.0);

    BOOST_CHECK_EQUAL( (f1*f2).to<int>(), 8*16 );
    BOOST_CHECK_EQUAL( (f1*3).to<int>(), 8*3 );
    BOOST_CHECK_EQUAL( (f1*5.0).to<int>(), 8*5 );
}

BOOST_AUTO_TEST_CASE( division )
{
    fixed32_16 f1(8.0);
    fixed32_16 f2(4.0);
    fixed32_16 f3(2.0);
    fixed32_16 f4(0.5);

   BOOST_CHECK_EQUAL( (f1/f2).to<double>(), f3.to<double>() );
   BOOST_CHECK_EQUAL( (f2/f1).to<double>(), f4.to<double>() );
}

BOOST_AUTO_TEST_CASE( unary )
{
    fixed32_16 f1(46);
    fixed32_16 f2(72);

    BOOST_CHECK( +f1 - f2 == -f2 + f1 );
    BOOST_CHECK( +f2 - f1 == -f1 + f2 );
}
