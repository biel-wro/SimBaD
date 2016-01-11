#ifndef FRONT_WAVE_1D_HPP
#define FRONT_WAVE_1D_HPP

#include <random>
#include <utility>

#include <boost/intrusive/set.hpp>

#include "handle_trackee.hpp"
#include "lazy_set.hpp"
#include "simple_event_queue.hpp"


#include "particle_1d.hpp"

namespace simbad
{
namespace models
{
class front_wave_1d{
public:
  front_wave_1d();
  void set_configuration(void);
  void get_next_event();
  void get_current_configuration();

};


}
}
#endif // FRONT_WAVE_1D_HPP
