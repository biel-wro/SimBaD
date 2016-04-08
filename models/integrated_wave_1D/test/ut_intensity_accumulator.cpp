#include <iostream>
#include <vector>

#include <boost/test/auto_unit_test.hpp>

#include "fixed/fixed.hpp"

#include "integrated_wave_1d.hpp"
#include "intensity_accumulator.hpp"
#include "particle_1d.hpp"

using simbad::models::integrated_wave_1d::intensity_accumulator;

BOOST_AUTO_TEST_CASE(accumulator_instantiation) { intensity_accumulator acc; }
