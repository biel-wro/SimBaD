#include <boost/test/auto_unit_test.hpp>

#include "config.hpp"
#include "particle.hpp"
#include "simple_event_queue.hpp"

using namespace simbad::models::simple_exp_2d;

BOOST_AUTO_TEST_SUITE(simple_exp_2d)

BOOST_AUTO_TEST_CASE(particle_tracking)
{
  simbad::models::simple_exp_2d::particle p1, p2;
  simbad::core::simple_event_queue queue;

  simbad::core::simple_event_handle h1 =
      queue.emplace(1.0f, simbad::core::EVENT_KIND::NONE, &p1);
  p1.set_handle(h1);

  simbad::core::simple_event_handle h2 =
      queue.emplace(2.0f, simbad::core::EVENT_KIND::NONE, &p2);
  p2.set_handle(h2);

  BOOST_REQUIRE(p1.get_handle()==h1);
  BOOST_REQUIRE(p2.get_handle()==h2);

  BOOST_REQUIRE((*h1).get_particle_ptr_as<particle>() == &p1 );
  BOOST_REQUIRE((*h2).get_particle_ptr_as<particle>() == &p2 );

  std::swap(p1,p2);

  BOOST_REQUIRE(p2.get_handle()==h1);
  BOOST_REQUIRE(p1.get_handle()==h2);

  BOOST_REQUIRE((*h1).get_particle_ptr_as<particle>() == &p2 );
  BOOST_REQUIRE((*h2).get_particle_ptr_as<particle>() == &p1 );
}

BOOST_AUTO_TEST_SUITE_END()
