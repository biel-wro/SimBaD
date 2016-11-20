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

BOOST_AUTO_TEST_CASE(tracking_empty)
{
  BOOST_TEST_CHECKPOINT("before queue construction");

  simple_event_queue eq;
  BOOST_TEST_CHECKPOINT("after queue construction");

  Point p0;
  BOOST_TEST_CHECKPOINT("after point construction");

  simple_event_handle empty_event_handle = p0.get_handle();
  BOOST_TEST_CHECKPOINT("after get");
  BOOST_REQUIRE(*empty_event_handle != nullptr);

  simple_event_handle null_handle(nullptr);
  BOOST_TEST_CHECKPOINT("before check");
  BOOST_REQUIRE(empty_event_handle == null_handle);

  simple_event_handle event_handle = eq.emplace();

  simple_event_handle event_handle_copy = event_handle;
  BOOST_REQUIRE_EQUAL(std::addressof(*event_handle),
                      std::addressof(*event_handle_copy));

  simple_event_handle handle_moved = std::move(event_handle_copy);

  BOOST_REQUIRE((*handle_moved).get_particle_ptr_as<Point>() == nullptr);
  BOOST_REQUIRE((*event_handle_copy).get_particle_ptr_as<Point>() == nullptr);
}

BOOST_AUTO_TEST_CASE(tracking_particle_move)
{
  simple_event_queue eq;
  simple_event_handle event_handle = eq.emplace();
  BOOST_REQUIRE(std::addressof(*event_handle) != nullptr);

  Point p1(event_handle);
  BOOST_REQUIRE_EQUAL(std::addressof(*event_handle),
                      std::addressof(*p1.get_handle()));

  BOOST_REQUIRE_EQUAL((*event_handle).get_particle_ptr_as<Point>(), &p1);
  BOOST_REQUIRE_EQUAL((*p1.get_handle()).get_particle_ptr_as<Point>(), &p1);
  BOOST_REQUIRE_EQUAL((*event_handle).get_particle_ptr_as<Point>(), &p1);

  Point p2(std::move(p1));
  BOOST_REQUIRE_EQUAL((*event_handle).get_particle_ptr_as<Point>(), &p2);
}

BOOST_AUTO_TEST_CASE(tracking_particle_swap)
{
  simple_event_queue eq;
  simple_event_handle h1 = eq.emplace();
  simple_event_handle h2 = eq.emplace();

  Point p1(h1);
  BOOST_REQUIRE_EQUAL(static_cast<Point *>((*h1).get_particle_ptr()), &p1);

  Point p2(h2);
  BOOST_REQUIRE_EQUAL(static_cast<Point *>((*h2).get_particle_ptr()), &p2);

  std::swap(p1, p2);
  BOOST_REQUIRE_EQUAL(static_cast<Point *>((*h2).get_particle_ptr()), &p1);
  BOOST_REQUIRE_EQUAL(static_cast<Point *>((*h1).get_particle_ptr()), &p2);

  std::swap(p1, p2);
  BOOST_REQUIRE_EQUAL(static_cast<Point *>((*h1).get_particle_ptr()), &p1);
  BOOST_REQUIRE_EQUAL(static_cast<Point *>((*h2).get_particle_ptr()), &p2);
}
