#include "configurations/cubic_crystal_configuration.hpp"
#include "configurations/default_attributed_configuration.hpp"
#include "configurations/selected_attributes_configuration.hpp"
#include "configurations/stacked_view_configuration.hpp"
#include "interface/attribute_descriptor.hpp"
#include "io/csv_printer.hpp"
#include "io/csv_reader.hpp"
#include "io/text_printer.hpp"
#include "processors/configuration_builder.hpp"

#include <boost/test/auto_unit_test.hpp>
#include <iostream>

char const *test_input =
    R"TESTINPUT("position_0","position_1","position_2","float_property", "int_property", "event_kind"
1, 1, 1, 1.1, 10, 1
2, 2, 2, 2.2, 20, 1
3, 3, 3, 3.3, 30, 1
1, 1, 1, 1.0, 10, 3
3, 3, 3, 3.3, 30, 2
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
  text_printer printer(&std::cout);

  property_tree attrs;
  attrs.put("abc", 123);

  default_attributed_configuration conf(initial_configuration, attrs);
  // printer.set_configuration(conf);
}

BOOST_AUTO_TEST_CASE(read_stream)
{
  std::stringstream stream(test_input);
  csv_reader reader(&stream);
  attribute_description description = reader.read_header();

  description.standardize_record(ATTRIBUTE_KIND::EVENT_KIND, "event_kind");

  std::vector<std::string> values_to_track = {"float_property",
                                              "int_property"};

  configuration_builder builder(description, "position", values_to_track);

  reader.visit_entries(
      [&builder](attribute_list const &event) { builder.push_event(event); });

  std::cerr << builder.description();

  csv_printer writer(&std::cout);
  writer.write_dataframe(builder);
}

BOOST_AUTO_TEST_SUITE_END()
