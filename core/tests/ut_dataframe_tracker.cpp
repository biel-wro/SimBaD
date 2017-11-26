#include <boost/test/auto_unit_test.hpp>

#include "io/csv_printer.hpp"
#include "io/csv_reader.hpp"
#include "processors/dataframe_tracker.hpp"

#include <iostream>
#include <sstream>
#include <string>

using namespace simbad::core;

BOOST_AUTO_TEST_SUITE(test_dataframe_tracker)

BOOST_AUTO_TEST_CASE(instantiation)
{
  std::string test_input =
      R"TESTINPUT("position_0",  "position_1",   "position_2",   "density"
         1, 2, 3, 1.23
         6, 7, 8, 5.32
)TESTINPUT";

    std::stringstream stream(test_input);

    csv_reader reader(&stream);
    csv_printer writer(&std::cerr);

    attribute_description description = reader.read_header();
    writer.write_header(description);

    std::vector<std::string> key_names = {"position"};
    std::vector<std::string> observable_names = {"density"};

    
    dataframe_tracker tracker(description, key_names, observable_names);

    std::cerr << test_input << std::endl;

}

BOOST_AUTO_TEST_SUITE_END()
