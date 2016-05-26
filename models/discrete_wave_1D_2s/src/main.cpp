#include <iostream>

#include "event.hpp"

#include "discrete_wave_1d_2s.hpp"

using namespace simbad::models::discrete_wave_1d_2s;


int main(int argc, char **argv)
{
  discrete_wave_1d_2s m(3.5,1.0,10000,1.0,0);

  auto event_visitor = [&](CORE_NAMESPACE::event const &e) {
    e.raw_text_print(std::cout);
    std::cout << std::endl;
  };
  m.generate_events(event_visitor, 10);

}
