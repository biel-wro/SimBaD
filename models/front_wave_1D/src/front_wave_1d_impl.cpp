#include "front_wave_1d_impl.hpp"

#include <tuple>
#include <utility>

#include "birth_rate_accumulator.hpp"
#include "death_rate_accumulator.hpp"

#include "sort_on_1D.hpp"
#include "spatial_neighbourhood.hpp"

namespace simbad
{
namespace models
{

front_wave_1d_impl::front_wave_1d_impl() : t(0.0), placer(1, 10)
{
    reinitialize();
}

front_wave_1d_impl::~front_wave_1d_impl() { space.clear(); }

void front_wave_1d_impl::clear() {}

void front_wave_1d_impl::reinitialize()
{

    init_configuration();
    init_event_queue();
}

front_wave_1d_impl::Event front_wave_1d_impl::next_event()
{
    return execute_event();
}

front_wave_1d_impl::Queue::ordered_iterator
front_wave_1d_impl::begin_queue() const
{
    return queue.ordered_begin();
}
front_wave_1d_impl::Queue::ordered_iterator
front_wave_1d_impl::end_queue() const
{
    return queue.ordered_end();
}

std::size_t front_wave_1d_impl::size() const { return space.size(); }

double front_wave_1d_impl::simulation_time() const { return t; }
front_wave_1d_impl::Space::const_iterator front_wave_1d_impl::begin() const
{
    return space.begin();
}

front_wave_1d_impl::Space::const_iterator front_wave_1d_impl::end() const
{
    return space.end();
}

void front_wave_1d_impl::resample_event(particle_1D &p)
{
    EventHandle h = p.get_handle();
    std::pair<float, EVENT_KIND> e = p.sample_event(rnd);

    (*h).set_time(e.first + simulation_time());
    (*h).set_event_kind(e.second);
    (*h).set_particle_ptr(&p);

    queue.update(h);
}

void front_wave_1d_impl::full_update(particle_1D &p)
{
    double center = p.coordinate(0);

    for (particle_1D const &neighbour : get_neighbourhood(center))
        p.update_accumulators(neighbour);

    resample_event(p);
}

void front_wave_1d_impl::update_simulation_time(double new_time)
{
    this->t = new_time;
}

void front_wave_1d_impl::init_configuration()
{
    std::unique_ptr<particle_1D> p(new particle_1D(0.0f));
    space.insert(*p);
    p.release();
}

void front_wave_1d_impl::init_event_queue()
{

    for (particle_1D &p : space)
    {
        EventHandle h = queue.emplace(&p);
        p.set_handle(h);
        full_update(p);
    }
}

void front_wave_1d_impl::update_neighbourhood(const Event &e)
{
    double center = e.coordinate(0);
    spatial_neighbourhood neighbourhood = get_neighbourhood(center);
    for (particle_1D &particle : neighbourhood)
    {
        particle.update_accumulators(e);
        resample_event(particle);
    }
}

front_wave_1d_impl::Event front_wave_1d_impl::execute_event()
{
    double new_time = queue.top().get_time();
    update_simulation_time(new_time);

    EVENT_KIND const event_kind = queue.top().get_event_kind();

    Event retval;
    if (event_kind == EVENT_KIND::DEATH)
        retval = execute_death();
    else if (event_kind == EVENT_KIND::BIRTH)
        retval = execute_birth();

    return retval;
}

front_wave_1d_impl::Event front_wave_1d_impl::execute_birth()
{
    EventSchedule const &es = queue.top();
    Event event(es.get_time(), simbad::core::EVENT_KIND::BIRTH);

    particle_1D &parent = *es.get_particle_ptr_as_nonconst<particle_1D>();

    double parent_x = parent.get_coordinate<0>();
    double offspring_x = placer(rnd, parent_x);

    event.set_coordinate(0, offspring_x);

    resample_event(parent);
    update_neighbourhood(event);

    EventHandle offspring_event_handle = queue.emplace();

    auto offspring_ptr =
        std::make_unique<particle_1D>(offspring_x, offspring_event_handle);

    particle_1D &offspring = *offspring_ptr.release();

    space.insert(offspring);

    full_update(offspring);

    return event;
}

front_wave_1d_impl::Event front_wave_1d_impl::execute_death()
{
    EventSchedule const &es = queue.top();

    Event event(es.get_time(), simbad::core::EVENT_KIND::DEATH);

    particle_1D const &p = *es.get_particle_ptr_as<particle_1D>();
    float position = p.coordinate(0);
    event.set_coordinate(0, position);

    space.erase_and_dispose(p, std::default_delete<particle_1D>());
    queue.pop();

    update_neighbourhood(event);
    return event;
}

spatial_neighbourhood front_wave_1d_impl::get_neighbourhood(float center)
{
    double range = particle_1D::interaction_range();
    return spatial_neighbourhood(space, center, range);
}
}
}
