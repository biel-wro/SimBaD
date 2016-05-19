#include <cstdlib>
#include <iostream>
#include <tuple>
#include <utility>

#include "discrete_wave_1d_fwd.hpp"

#include "discrete_wave_1d.hpp"
#include "event_1d.hpp"

USING_NAMEPSPACE_DISCRETE_WAVE_1D;

void plain_text_print( discrete_wave_1d::Event e){
  std::cout<< e.time() << " " << e.coordinate(0) << std::endl;
}


int main(int argc, char **argv)
{
  discrete_wave_1d model(4.5, 1, 10000);

  std::size_t niters = 110000;

  discrete_wave_1d::Event e = model.initial_event();
  plain_text_print(e);

  for (std::size_t iter = 1; iter < niters; ++iter)
  {
    discrete_wave_1d::Event e = model.next_event();
    plain_text_print(e);
  }

  return 0;
}
