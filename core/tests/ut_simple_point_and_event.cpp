#include "object_tracking/handle_trackee.hpp"
#include "object_tracking/trackee.hpp"

#include "simple_event_queue.hpp"
#include "simple_event_schedule.hpp"
#include "simple_tracked_particle.hpp"

#include <boost/heap/d_ary_heap.hpp>
#include <boost/heap/fibonacci_heap.hpp>
#include <boost/heap/pairing_heap.hpp>
#include <boost/test/auto_unit_test.hpp>

#include <array>
#include <type_traits>
using namespace simbad::core;

static_assert(sizeof(simple_event_handle) == sizeof(void *),
              "making sure handle is pointer-sized");

using Point = simple_tracked_particle<2, float>;

BOOST_AUTO_TEST_CASE(tracking_particle_move)
{
  simple_event_queue eq;

  simple_event_handle handle = eq.emplace();
  BOOST_REQUIRE((*handle).get_particle_ptr_as<Point>() == nullptr);

  Point p0(handle);
  BOOST_REQUIRE_EQUAL(std::addressof(*handle),
                      std::addressof(*p0.get_handle()));
  BOOST_REQUIRE_EQUAL((*p0.get_handle()).get_particle_ptr_as<Point>(), &p0);
  BOOST_REQUIRE_EQUAL((*handle).get_particle_ptr_as<Point>(), &p0);

  Point p1(std::move(p0));
  BOOST_REQUIRE_EQUAL((*handle).get_particle_ptr_as<Point>(), &p1);
}

BOOST_AUTO_TEST_CASE(tracking_particle_swap)
{
  simple_event_queue eq;
  simple_event_handle handle = eq.emplace();
  simple_event_handle h2 = eq.emplace();

  Point p1(handle);
  BOOST_REQUIRE_EQUAL(static_cast<Point *>((*handle).get_particle_ptr()), &p1);

  Point p2(h2);
  BOOST_REQUIRE_EQUAL(static_cast<Point *>((*h2).get_particle_ptr()), &p2);

  std::swap(p1, p2);
  BOOST_REQUIRE_EQUAL(static_cast<Point *>((*h2).get_particle_ptr()), &p1);
  BOOST_REQUIRE_EQUAL(static_cast<Point *>((*handle).get_particle_ptr()), &p2);

  std::swap(p1, p2);
  BOOST_REQUIRE_EQUAL(static_cast<Point *>((*handle).get_particle_ptr()), &p1);
  BOOST_REQUIRE_EQUAL(static_cast<Point *>((*h2).get_particle_ptr()), &p2);
}
