#include "front_wave_1d_impl.hpp"

#include <tuple>
#include <utility>

#include "birth_rate_accumulator.hpp"
#include "death_rate_accumulator.hpp"
//#include "front_wave_1d_algorithms.hpp"
#include "sort_on_1D.hpp"

namespace simbad
{
namespace models
{

front_wave_1d_impl::front_wave_1d_impl() : t(0.0), op(1, 3) { reinitialize(); }

front_wave_1d_impl::~front_wave_1d_impl() { space.clear(); }

void front_wave_1d_impl::reinitialize()
{
    range = default_interation_range();

    space = initial_configuration();
    queue = init_event_queue();
}

front_wave_1d_impl::Event front_wave_1d_impl::next_event()
{
    return execute_event();
}

double front_wave_1d_impl::default_interation_range()
{
    double r1 = death_rate_accumulator().get_range();
    double r2 = birth_rate_accumulator().get_range();

    return std::max(r1, r2);
}

front_wave_1d_impl::Space front_wave_1d_impl::initial_configuration()
{
    Space space;
    std::unique_ptr<particle_1D> p(new particle_1D(0.0f));
    space.insert(*p);
    p.release();

    return space;
}

front_wave_1d_impl::Queue front_wave_1d_impl::init_event_queue()
{
    Queue event_queue;

    for (particle_1D &particle : space)
    {
        EventSchedule event = compute_event(particle);

        EventHandle h = event_queue.emplace(std::move(event));

        particle.set_handle(h);
        (*h).set_particle_ptr(&particle);
    }

    return event_queue;
}

void front_wave_1d_impl::update_neighbourhood(double center)
{
    double lb = center - range;
    double ub = center + range;

    Space::const_iterator neigh_beg = space.lower_bound(lb, sort_on_1D());
    Space::const_iterator neigh_end = space.upper_bound(ub, sort_on_1D());

    for (Space::const_iterator it = neigh_beg; it != neigh_end; ++it)
    {
        const particle_1D &particle = *it;

        EventSchedule event = compute_event(particle);
        event.increase_time(t);

        EventHandle h = particle.get_handle();
        *h = std::move(event);
        (*h).set_particle_ptr(const_cast<particle_1D *>(&*it));

        queue.update_lazy(h);
    }
}

front_wave_1d_impl::Event front_wave_1d_impl::execute_event()
{
    Event e;
    particle_1D const &point = *queue.top().get_particle_ptr_as<particle_1D>();

    double event_center = point.get_coordinate<0>();

    t = queue.top().get_time();
    e.set_time(t);

    EVENT_KIND const event_kind = queue.top().get_event_kind();
    e.set_event_kind(event_kind);
    if (event_kind == EVENT_KIND::DEATH)
    {
        execute_death();
        e.set_coordinate(0,event_center);
    }
    else if (event_kind == EVENT_KIND::BIRTH)
    {
        particle_1D &offspring = execute_birth();
        double offspring_pos = offspring.get_coordinate<0>();
        e.set_coordinate(0,offspring_pos);
        update_neighbourhood(offspring_pos);
    }

    update_neighbourhood(event_center);



    return e;
}

particle_1D &front_wave_1d_impl::execute_birth()
{
    particle_1D const &parent = *queue.top().get_particle_ptr_as<particle_1D>();

    double parent_x = parent.get_coordinate<0>();
    double offspring_x = op(rnd,parent_x);

    EventHandle offspring_event_handle = queue.emplace();

    std::unique_ptr<particle_1D> offspring_ptr(
        new particle_1D(offspring_x, offspring_event_handle));
    space.insert(*offspring_ptr);

    particle_1D &offspring = *offspring_ptr;
    offspring_ptr.release();

    return offspring;
}

void front_wave_1d_impl::execute_death()
{
    particle_1D const &p = *queue.top().get_particle_ptr_as<particle_1D>();

    space.erase_and_dispose(p, std::default_delete<particle_1D>());
    queue.pop();
}

front_wave_1d_impl::EventSchedule
front_wave_1d_impl::compute_event(particle_1D const &p)
{
    death_rate_accumulator death_acc;
    double death_intrange = death_acc.get_range();

    birth_rate_accumulator birth_acc;
    double birth_intrange = birth_acc.get_range();

    double range = std::max(death_intrange, birth_intrange);
    double center = p.get_coordinate<0>();

    Space::const_iterator neight_beg =
        space.lower_bound<float, sort_on_1D>(center - range, sort_on_1D());
    Space::const_iterator neight_end =
        space.upper_bound<float, sort_on_1D>(center + range, sort_on_1D());

    for (Space::const_iterator cit = neight_beg; cit != neight_end; ++cit)
    {
        death_acc.accumulate(p, *cit);
        birth_acc.accumulate(p, *cit);
    }

    double death_intensity = death_acc.get_intensity();
    double birth_intensity = birth_acc.get_intensity();

    double death_time = std::exponential_distribution<>(death_intensity)(rnd);
    double birth_time = std::exponential_distribution<>(birth_intensity)(rnd);

    EventSchedule event;

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
}
}
