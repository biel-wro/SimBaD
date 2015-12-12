#define BOOST_TEST_MAIN

#include <boost/test/included/unit_test.hpp>

#include <trackee_base.h>

struct ptr_updater
{
    ptr_updater() : listener(nullptr) {}
    explicit ptr_updater(void *&ref_listener) : listener(&ref_listener) {}

    ptr_updater(ptr_updater &&) = default;
    ptr_updater(ptr_updater const &) = delete;

    ptr_updater &operator=(ptr_updater &&) = default;
    ptr_updater &operator=(ptr_updater const &) = delete;

    void notify_constructed(void *ptr) { notify(ptr); }
    void notify_copy_constructed(void *ptr) { notify(ptr); }
    void notify_move_constructed(void *ptr) { notify(ptr); }
    void notify_copy_assigned(void *ptr) { notify(ptr); }
    void notify_move_assigned(void *ptr) { notify(ptr); }
    void notify_destroyed(void *) { notify(nullptr); }

    void notify(void *ptr)
    {
        if (nullptr != listener)
            *listener = ptr;
    }

    void **listener;
};

struct my_trackee : public trackee<ptr_updater>
{
    my_trackee() = delete;
    explicit my_trackee(ptr_updater &&pu) : trackee<ptr_updater>(std::move(pu))
    {
    }
};

BOOST_AUTO_TEST_CASE(static_asserts)
{
    static_assert(sizeof(ptr_updater) == sizeof(void *), "sizes must match");
    static_assert(sizeof(trackee<ptr_updater>) == sizeof(void *),
                  "sizes must match");
}

BOOST_AUTO_TEST_CASE(tracking_simple)
{
    void *tracker1;
    ptr_updater updater1(tracker1);

    my_trackee t1(std::move(updater1));
    BOOST_REQUIRE_EQUAL(tracker1, &t1);

    my_trackee t2(std::move(t1));
    BOOST_REQUIRE_EQUAL(tracker1, &t2);

    std::swap(t1, t2);
    BOOST_REQUIRE_EQUAL(tracker1, &t1);

    std::swap(t1, t2);
    BOOST_REQUIRE_EQUAL(tracker1, &t2);
}
