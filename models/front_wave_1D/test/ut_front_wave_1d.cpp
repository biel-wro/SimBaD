#define BOOST_TEST_MAIN

#include <iostream>

#include <boost/test/auto_unit_test.hpp>
#include <boost/test/included/unit_test.hpp>

#include <front_wave_1d_algorithms.hpp>

BOOST_AUTO_TEST_CASE(first_iteration)
{
    using namespace simbad::models;
    using Storage = front_wave_1d_algorithms::Storage;
    using Space = front_wave_1d_algorithms::Space;
    using Queue = front_wave_1d_algorithms::Queue;
    using Event = front_wave_1d_algorithms::Event;
    using EventHandle = front_wave_1d_algorithms::EventHandle;

    std::mt19937_64 random_engine;
    std::pair<Storage, Space> state =
        front_wave_1d_algorithms::initial_configuration();

    Storage &storage = state.first;
    Space &space = state.second;
    Queue event_queue =
        front_wave_1d_algorithms::init_event_queue(random_engine, space);

    double time = 0.0;
    front_wave_1d_algorithms::execute_event(time, random_engine, storage, space,
                                            event_queue);

    BOOST_CHECK_EQUAL(storage.size(), 2);
    BOOST_CHECK_EQUAL(space.size(), 2);
    BOOST_CHECK_EQUAL(event_queue.size(), 2);

    for (particle_1D const &p : storage)
    {

        EventHandle h = p.get_handle();
        Event &e = *h;
        particle_1D *ptr = e.get_particle_ptr_as<particle_1D>();

        BOOST_CHECK_EQUAL(ptr, &p);

    }
}
