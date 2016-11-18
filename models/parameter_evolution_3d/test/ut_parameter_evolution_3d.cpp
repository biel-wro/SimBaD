#include <iostream>

#include <boost/property_tree/ptree.hpp>
#include <boost/test/auto_unit_test.hpp>

#include "particle.hpp"

//using namespace simbad::core;
using namespace  simbad::models::parameter_evolution_3d;

BOOST_AUTO_TEST_SUITE(parameter_evolution_3d)

BOOST_AUTO_TEST_CASE(empty)
{

}




BOOST_AUTO_TEST_CASE(particle_size_check)
{
  BOOST_REQUIRE_EQUAL( sizeof(cell), 4*sizeof(float) + 4*sizeof(float) );
}

BOOST_AUTO_TEST_SUITE_END()
