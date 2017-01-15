#include "configurations/cubic_crystal_configuration.hpp"
#include "configurations/default_attributed_configuration.hpp"
#include "configurations/selected_attributes_configuration.hpp"
#include "configurations/stacked_view_configuration.hpp"

#include "interface/particle.hpp"
#include "processors/configuration_builder.hpp"
#include "processors/text_configuration_printer.hpp"

#include <boost/test/auto_unit_test.hpp>
#include <iostream>

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
  //printer.set_configuration(conf);
}

BOOST_AUTO_TEST_CASE(load_static_configuration)
{
  /*
  cubic_crystal_configuration crystal(2, 2, 1);
  property_tree pt;
  pt.put("prop1", "abc");
  pt.put("prop2", "2");
  pt.put("prop3", "3.0");
  pt.put("prop4", 4.0);
  pt.put("prop5", 5);

  default_attributed_configuration initial_configuration(crystal,
                                                        pt);
  configuration_builder builder(true,
                                configuration_builder::ID_POLICY::REASSIGN);

  builder.set_configuration(initial_configuration);

  property_tree cpt;
  cpt.put("layers.l1.class", "selected_attributes");
  cpt.put("layers.l1.parameters.white_list.el1", "prop4");

  stacked_view_configuration stacked_view(builder, cpt);

  text_configuration_printer printer(&std::cout);

  //printer.set_configuration(stacked_view);
  */
}

BOOST_AUTO_TEST_SUITE_END()
