#include <cstdlib>
#include <iostream>
#include <tuple>
#include <utility>

#include "discrete_wave_1d_1s_fwd.hpp"

#include "event.hpp"

#include "discrete_wave_1d_1s.hpp"


USING_NAMEPSPACE_DISCRETE_WAVE_1D_1S;

int main(int argc, char **argv)
{
  discrete_wave_1d_1s m(3.5, 1.0, 10000, 1.0, 1.0, 0);

  auto event_visitor = [&](CORE_NAMESPACE::event const &e) {
    e.raw_text_print(std::cout);
    std::cout << std::endl;
  };

  for (size_t genruns = 0; genruns < 10000; ++genruns)
    m.generate_events(event_visitor, 1000);
}
