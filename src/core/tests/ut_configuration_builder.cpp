#include "configurations/cubic_crystal_configuration.hpp"
#include "configurations/default_attributed_configuration.hpp"
#include "configurations/selected_attributes_configuration.hpp"
#include "configurations/stacked_view_configuration.hpp"
#include "io/text_configuration_printer.hpp"
#include "processors/configuration_builder.hpp"

#include <boost/test/auto_unit_test.hpp>
#include <iostream>

char const *test_input =
    R"TESTINPUT("position_0","position_1","position_2","density","event.time","event.kind","birth.efficiency","birth.resistance","lifespan.efficiency","lifespan.resistance","success.efficiency","success.resistance","mutation.id","birth.rate","death.rate","success.probability","lifespan","time","delta time","event"
-2,-1,-1,7.27422,0.00916802,2,0.1,0.5,0.1,0.5,0.9,0.5,1,0.0145068,0.0288142,0.0130561,34.7051,0.00916802,0,2
2,2,1,5.11779,0.116703,1,0.1,0.5,0.1,0.5,0.9,0.5,1,0.123029,0.0168849,0.110726,59.2245,0.116703,0,2
2,-2,1,5.11779,0.259171,1,0.1,0.5,0.1,0.5,0.9,0.5,1,0.123029,0.0168849,0.110726,59.2245,0.259171,0,2
-2,-2,-2,3.18449,0.501877,1,0.1,0.5,0.1,0.5,0.9,0.5,1,0.444291,0.0122485,0.399861,81.6426,0.501877,0,2
-2,-2,2,3.55237,0.514303,1,0.1,0.5,0.1,0.5,0.9,0.5,1,0.364385,0.0128709,0.327946,77.6945,0.514303,0,2
1,-2,2,4.74991,0.690842,1,0.1,0.5,0.1,0.5,0.9,0.5,1,0.164486,0.0157025,0.148038,63.6843,0.690842,0,2
2,2,-1,4.98245,0.781674,1,0.1,0.5,0.1,0.5,0.9,0.5,1,0.137245,0.0164295,0.123521,60.8659,0.781674,0,2
-2,2,2,3.55237,0.787124,2,0.1,0.5,0.1,0.5,0.9,0.5,1,0.364385,0.0128709,0.327946,77.6945,0.787124,0,2
-1,-2,-2,4.28813,0.874215,1,0.1,0.5,0.1,0.5,0.9,0.5,1,0.229686,0.014445,0.206718,69.2283,0.874215,0,2
1,-1,-1,10.2448,0.88711,2,0.1,0.5,0.1,0.5,0.9,0.5,1,0.000225667,0.0815892,0.000203101,12.2565,0.88711,0,2
)TESTINPUT";

using namespace simbad::core;
BOOST_AUTO_TEST_SUITE(test_configuration_builder)
BOOST_AUTO_TEST_CASE(set_default_attributes)
{
  property_tree pt;
  pt.put("dimension", 3);
  pt.put("radius", 2);
  pt.put("spacing", 1);

  cubic_crystal_configuration initial_configuration(pt);
  text_configuration_printer printer(&std::cout);

  property_tree attrs;
  attrs.put("abc", 123);

  default_attributed_configuration conf(initial_configuration, attrs);
  // printer.set_configuration(conf);
}

BOOST_AUTO_TEST_CASE(read_stream)
{
  std::stringstream stream(test_input);


}

BOOST_AUTO_TEST_SUITE_END()
