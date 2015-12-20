#include "uni_trackee.hpp"
#include "ptr_updater.hpp"

#include <boost/test/auto_unit_test.hpp>

using namespace simbad::core;
namespace
{
struct my_trackee : public uni_trackee<ptr_updater<my_trackee>, my_trackee>
{
    my_trackee() = delete;
    explicit my_trackee(ptr_updater<my_trackee> pu)
        : uni_trackee<ptr_updater<my_trackee>, my_trackee>(std::move(pu))
    {
    }
};
}
BOOST_AUTO_TEST_CASE(static_asserts)
{
    static_assert(sizeof(my_trackee) == sizeof(void *), "sizes must match");
}

BOOST_AUTO_TEST_CASE(tracking_simple)
{
    my_trackee *tracker1;

    ptr_updater<my_trackee> callback(tracker1);

    my_trackee t1(std::move(callback));
    BOOST_REQUIRE_EQUAL(tracker1, &t1);

    my_trackee t2(std::move(t1));
    BOOST_REQUIRE_EQUAL(tracker1, &t2);

    std::swap(t1, t2);
    BOOST_REQUIRE_EQUAL(tracker1, &t1);

    std::swap(t1, t2);
    BOOST_REQUIRE_EQUAL(tracker1, &t2);
}
