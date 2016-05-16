#include "driver.hpp"

#include <algorithm>

#include "birth_pdf_computer.hpp"
#include "particle_1d.hpp"

BEGIN_NAMESPACE_INTEGRATED_WAVE_1D

using DIRECTION = intensity_accumulator::DIRECTION;

driver::driver() : simulation_time(0.0), computer(100) {}

driver::~driver() {}

driver::Event driver::next_event()
{
    double total_intensity = birth_acc.get_value();
    double dt = std::exponential_distribution<double>(total_intensity)(rnd);
    simulation_time += dt;

    double x; // sampled x

    throw std::runtime_error("not implemented yet");
}

void driver::init_configuration()
{
    storage = Storage();
    storage.emplace_back(0);

    particle_1d &p = storage.back();

    birth_acc.accumulate<DIRECTION::INCLUDE>(p, computer);
}

double driver::point_intensity(double x)
{
    intensity_accumulator acc;
    birth_pdf_computer cpu(x);

    auto beg = space.cbegin();
    auto end = space.cend();

    auto fun = [&acc, cpu](particle_1d const &p) {
        acc.accumulate<DIRECTION::INCLUDE>(p, cpu);
    };

    std::for_each(beg, end, fun);

    double intensity = acc.get_value();
    return intensity > 100 ? 100 : intensity;
}

END_NAMESPACE_INTEGRATED_WAVE_1D
