#include "configurations/cubic_crystal_configuration.hpp"
#include "configurations/enriched_configuration.hpp"
#include "interface/attribute_descriptor.hpp"
#include "interface/particle.hpp"
#include "processors/configuration_builder.hpp"
#include "processors/text_configuration_printer.hpp"

#include <boost/test/auto_unit_test.hpp>
#include <iostream>

using namespace simbad::core;
BOOST_AUTO_TEST_SUITE(test_configuration_builder)
BOOST_AUTO_TEST_CASE(load_static_configuration)
{
  cubic_crystal_configuration initial_configuration(2, 2, 1);
  property_tree pt;
  pt.put("prop1", "abc");
  pt.put("prop2", "2");
  pt.put("prop3", "3.0");
  pt.put("prop4", 4.0);
  pt.put("prop5", 5);

  enriched_configuration enriched_initial_configuration(initial_configuration,
                                                        pt);

  configuration_builder builder(true,
                                configuration_builder::ID_POLICY::REASSIGN);

  builder.set_configuration(enriched_initial_configuration);

  text_configuration_printer printer(std::cout);

  printer.set_configuration(builder);
}

BOOST_AUTO_TEST_SUITE_END()
