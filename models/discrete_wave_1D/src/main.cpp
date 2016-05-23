#include <cstdlib>
#include <iostream>
#include <tuple>
#include <utility>

#include "discrete_wave_1d_fwd.hpp"

#include "event.hpp"

#include "discrete_wave_1d.hpp"
//#include "discrete_wave_1d_impl.hpp"
//#include "event_1d.hpp"

USING_NAMEPSPACE_DISCRETE_WAVE_1D;
/*
void plain_text_print(discrete_wave_1d_impl::Event e)
{
  std::cout << e.time() << " " << e.coordinate(0) << std::endl;
}
*/
/*
int main(int argc, char **argv)
{
  discrete_wave_1d_impl model(4.5, 1, 10000);

  std::size_t niters = 110000;

  discrete_wave_1d_impl::Event e = model.initial_event();
  plain_text_print(e);

  for (std::size_t iter = 1; iter < niters; ++iter)
  {
    discrete_wave_1d_impl::Event e = model.next_event();
    plain_text_print(e);
  }

  return 0;
}
*/

int main(int argc, char **argv)
{
  discrete_wave_1d m(3.5,1.0,10000,1.0,0);

  auto event_visitor = [&](CORE_NAMESPACE::event const &e) {
    e.raw_text_print(std::cout);
    std::cout << std::endl;
  };
  m.generate_events(event_visitor, 10);

}
