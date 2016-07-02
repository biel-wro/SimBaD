#include <iostream>
#include <vector>

#include <boost/test/auto_unit_test.hpp>

#include "fixed/fixed.hpp"

#include "integrated_wave_1d_fwd.hpp"
#include "integrated_wave_1d.hpp"
#include "particle_1d.hpp"
#include "space_1d.hpp"

using namespace simbad::models::integrated_wave_1d;


BOOST_AUTO_TEST_CASE(empty) {}

BOOST_AUTO_TEST_CASE(space_instantiation) { space_1d space; }

BOOST_AUTO_TEST_CASE(space_filling)
{
    const int N = 123;
    space_1d space;

    std::vector<particle_1d> values;
    values.reserve(N);
    for (size_t i = 0; i < N; ++i)
        values.emplace_back(float(i));

    space.insert(values.begin(), values.end());

    std::vector<bool> visited(N, false);
    for (particle_1d const &p : space)
    {
        size_t idx = size_t(p.get_position());
        BOOST_REQUIRE(!visited[idx]);
        visited[idx] = true;
    }

    for (particle_1d const &p : space)
    {
        size_t idx = std::size_t(p.get_position());
        BOOST_REQUIRE(visited[idx]);
    }

    space.clear();

}

