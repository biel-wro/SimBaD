#include <iostream>
#include <vector>

#include <boost/numeric/conversion/cast.hpp>
#include <boost/test/auto_unit_test.hpp>

#include "interface/event_kind.hpp"
#include "front_wave_1d_impl.hpp"
using namespace simbad::models::front_wave_1d;
using namespace simbad::core;

BOOST_AUTO_TEST_CASE(void_test) {}

BOOST_AUTO_TEST_CASE(front_wave_1d_ordering)
{
    front_wave_1d_impl model(3,1);
    unsigned long niters = 1000;

    double t = 0.0;
    for (unsigned long iter = 0; iter < niters; ++iter)
    {
        if (iter == 6650)
            (void)0;
        auto event = model.next_event();
        //BOOST_TEST_MESSAGE("iter=" << iter );
        BOOST_REQUIRE_LE(t,event.time() );
        t = event.time();
    }

    for (auto it = model.begin_queue(); it != model.end_queue(); ++it)
    {
        //BOOST_TEST_MESSAGE(" " << it->get_time() << " ");
        BOOST_REQUIRE_LE(t, it->get_time() );
        t = it->get_time();
    }
}

BOOST_AUTO_TEST_CASE(front_wave_1d_size_check)
{
    front_wave_1d_impl model_pt(3,1);
    unsigned long niters = 1000;

    int64_t nparticles = boost::numeric_cast<int64_t>(model_pt.size());
    for (unsigned long iter = 0; iter < niters; ++iter)
    {
        auto event = model_pt.next_event();
        EVENT_KIND event_kind = event.event_kind();
        if (event_kind == EVENT_KIND::CREATED)
            nparticles++;
        if (event_kind == EVENT_KIND::REMOVED)
            nparticles--;
        BOOST_CHECK_GT(nparticles, 0);
        BOOST_REQUIRE_EQUAL(nparticles, model_pt.size());
    }
}
