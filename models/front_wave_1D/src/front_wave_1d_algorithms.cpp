#include "front_wave_1d_algorithms.hpp"

#include <cmath>
#include <iostream>
#include <memory>

#include "birth_rate_accumulator.hpp"
#include "death_rate_accumulator.hpp"
#include "sort_on_1D.hpp"

using std::mt19937_64;

using simbad::core::simple_event_handle;
using simbad::core::EVENT_KIND;
using simbad::core::simple_event;
using handle_type = simbad::core::simple_event_queue::handle_type;

namespace simbad
{
namespace models
{
front_wave_1d_algorithms::Space
front_wave_1d_algorithms::initial_configuration()
{
    Space space;
    std::unique_ptr<particle_1D> p(new particle_1D(0.0f));
    space.insert(*p);
    p.release();

    return space;
}

core::simple_event
front_wave_1d_algorithms::compute_event(Random &rnd, const particle_1D &p,
                                        const Space &space)
{
    death_rate_accumulator death_acc;
    double death_intrange = death_acc.get_range();

    birth_rate_accumulator birth_acc;
    double birth_intrange = birth_acc.get_range();

    double range = std::max(death_intrange, birth_intrange);
    double center = p.get_coordinate<0>();

    const_spatial_iterator neight_beg =
        space.lower_bound<float, sort_on_1D>(center - range, sort_on_1D());
    const_spatial_iterator neight_end =
        space.upper_bound<float, sort_on_1D>(center + range, sort_on_1D());

    for (const_spatial_iterator cit = neight_beg; cit != neight_end; ++cit)
    {
        death_acc.accumulate(p, *cit);
        birth_acc.accumulate(p, *cit);
    }

    double death_intensity = death_acc.get_intensity();
    double birth_intensity = birth_acc.get_intensity();

    double death_time = std::exponential_distribution<>(death_intensity)(rnd);
    double birth_time = std::exponential_distribution<>(birth_intensity)(rnd);

    Event event;

    if (death_time < birth_time)
    {
        event.set_time(death_time);
        event.set_event_kind(EVENT_KIND::DEATH);
    }
    else
    {
        event.set_time(birth_time);
        event.set_event_kind(EVENT_KIND::BIRTH);
    }

    return event;
}

core::simple_event_queue
front_wave_1d_algorithms::init_event_queue(std::mt19937_64 &rnd, Space &space)
{
    Queue event_queue;

    for (particle_1D &particle : space)
    {
        Event event = compute_event(rnd, particle, space);

        EventHandle h = event_queue.emplace(std::move(event));

        particle.set_handle(h);
        (*h).set_particle_ptr(&particle);
    }

    return event_queue;
}

void front_wave_1d_algorithms::update_neighbourhood(Random &rnd, Queue &eq,
                                                    double center, double range,
                                                    const Space &space,
                                                    double time_offset)
{
    auto neigh_beg = space.lower_bound(center - range, sort_on_1D());
    auto neigh_end = space.upper_bound(center + range, sort_on_1D());

    for (const_spatial_iterator it = neigh_beg; it != neigh_end; ++it)
    {
        const particle_1D &particle = *it;

        Event event = compute_event(rnd, particle, space);
        event.increase_time(time_offset);

        EventHandle h = particle.get_handle();
        *h = std::move(event);
        (*h).set_particle_ptr(const_cast<particle_1D *>(&*it));

        eq.update_lazy(h);
    }
}

void front_wave_1d_algorithms::execute_death(Space &space, Queue &eq)
{
    particle_1D const &p = *eq.top().get_particle_ptr_as<particle_1D>();

    space.erase_and_dispose(p, std::default_delete<particle_1D>());
    eq.pop();
}

particle_1D &front_wave_1d_algorithms::execute_birth(std::mt19937_64 &rnd,
                                                     Space &space, Queue &eq)
{
    particle_1D const &parent = *eq.top().get_particle_ptr_as<particle_1D>();

    double parent_x = parent.get_coordinate<0>();
    double offspring_x = std::normal_distribution<>(parent_x, 1)(rnd);

    EventHandle offspring_event_handle = eq.emplace();

    std::unique_ptr<particle_1D> offspring_ptr(
        new particle_1D(offspring_x, offspring_event_handle));
    space.insert(*offspring_ptr);

    particle_1D &offspring = *offspring_ptr;
    offspring_ptr.release();

    return offspring;
}

EVENT_KIND front_wave_1d_algorithms::execute_event(double &t,
                                                   std::mt19937_64 &rnd,
                                                   Space &space, Queue &eq)
{
    particle_1D const &point = *eq.top().get_particle_ptr_as<particle_1D>();

    double event_center = point.get_coordinate<0>();
    double update_range = 1;

    t = eq.top().get_time();

    EVENT_KIND const event_kind = eq.top().get_event_kind();

    if (event_kind == EVENT_KIND::DEATH)
    {
        execute_death(space, eq);
    }
    else if (event_kind == EVENT_KIND::BIRTH)
    {
        particle_1D &offspring = execute_birth(rnd, space, eq);
        double offspring_pos = offspring.get_coordinate<0>();

        update_neighbourhood(rnd, eq, offspring_pos, update_range, space, t);
    }

    update_neighbourhood(rnd, eq, event_center, update_range, space, t);

    return event_kind;
}
/*
std::pair<Storage,Space> front_wave_1d_algorithms::run()
{
    size_t niters = 100;

    std::mt19937_64 random_engine;
    std::pair<Storage, Space> state = initial_configuration();

    Storage &storage = state.first;
    Space &space = state.second;
    Queue event_queue = init_event_queue(random_engine, space);

    double time = 0.0;
    for (size_t iter = 0; iter < niters; ++iter)
        execute_event(time, random_engine, storage, space, event_queue);

    for(particle_1D const &p: storage )
    {
        std::cout<<p.get_coordinate<0>() << p.get_coordinate<1>() << std::endl;
    }

    return 0;
}
*/
}
}
