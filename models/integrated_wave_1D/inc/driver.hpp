#ifndef DRIVER_HPP
#define DRIVER_HPP
#include <random>

#include "lazy_set.hpp"

#include "integrated_wave_1d.hpp"
#include "integrated_wave_1d_fwd.hpp"

#include "intensity_accumulator.hpp"
#include "intensity_computer.hpp"
#include "space_1d.hpp"

BEGIN_NAMESPACE_INTEGRATED_WAVE_1D

class driver
{
  public:
    // types
    using Event = event_1d;
    using Random = std::mt19937_64;
    using Storage = ::simbad::core::LazySet<particle_1d>;
    // constructors
    driver();
    ~driver();

    Event next_event();

    // setters & getters
  protected:
    void init_configuration();
    double point_intensity(double x);


  private:
    double simulation_time;
    space_1d space;
    intensity_accumulator birth_acc;
    intensity_computer computer;
    Random rnd;
    Storage storage;

};

END_NAMESPACE_INTEGRATED_WAVE_1D

#endif // DRIVER_HPP
