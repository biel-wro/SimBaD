#define BOOST_TEST_MAIN

#include <iostream>
#include <vector>

#include <boost/test/auto_unit_test.hpp>
#include <boost/test/included/unit_test.hpp>

#include <front_wave_1d_algorithms.hpp>
using namespace simbad::models;

/*using Space = front_wave_1d_algorithms::Space;
using Queue = front_wave_1d_algorithms::Queue;
using Event = front_wave_1d_algorithms::Event;
using EventHandle = front_wave_1d_algorithms::EventHandle;

BOOST_AUTO_TEST_CASE(first_iteration)
{
    std::mt19937_64 random_engine;
    Space space = front_wave_1d_algorithms::initial_configuration();
    Queue event_queue =
        front_wave_1d_algorithms::init_event_queue(random_engine, space);

    double time = 0.0;
    front_wave_1d_algorithms::execute_event(time, random_engine, space,
                                            event_queue);

    BOOST_CHECK_EQUAL(space.size(), 2);
    BOOST_CHECK_EQUAL(event_queue.size(), 2);

    for (particle_1D const &p : space)
    {

        EventHandle h = p.get_handle();
        Event &e = *h;
        particle_1D *ptr = e.get_particle_ptr_as<particle_1D>();

        BOOST_CHECK_EQUAL(ptr, &p);
    }
}

BOOST_AUTO_TEST_CASE(more_iterations)
{
    std::mt19937_64 random_engine;
    Space space = front_wave_1d_algorithms::initial_configuration();
    Queue event_queue =
        front_wave_1d_algorithms::init_event_queue(random_engine, space);

    double time = 0.0;
    for (size_t iter = 0; iter < 100; iter++)
    {
        BOOST_TEST_CHECKPOINT("iteration = " << iter);

        front_wave_1d_algorithms::execute_event(time, random_engine, space,
                                                event_queue);
        for (particle_1D const &p : space)
        {

            EventHandle h = p.get_handle();
            Event &e = *h;
            particle_1D *ptr = e.get_particle_ptr_as<particle_1D>();

            BOOST_CHECK_EQUAL(ptr, &p);
        }
    }
}
*/
