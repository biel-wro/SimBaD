#include <boost/test/auto_unit_test.hpp>

#include "io/csv_printer.hpp"
#include "io/csv_reader.hpp"
#include "processors/dataframe_tracker.hpp"

#include <boost/optional.hpp>

#include <iostream>
#include <sstream>
#include <string>

using namespace simbad::core;

BOOST_AUTO_TEST_SUITE(test_dataframe_tracker)

BOOST_AUTO_TEST_CASE(instantiation)
{
  std::string test_input =
      R"TESTINPUT("position_0","position_1","position_2","density"
         1, 2, 3, 1.23
         6, 7, 8, 5.32
)TESTINPUT";

  std::stringstream input_stream(test_input);

  csv_reader reader(&input_stream);

  attribute_description const description = reader.read_header();

  std::vector<std::string> key_names = {"position"};
  std::vector<std::string> observable_names = {"density"};

  std::size_t position_idx =
      description.get_descriptor("position").get().attribute_idx();
  std::size_t observable_idx =
      description.get_descriptor("density").get().attribute_idx();

  dataframe_tracker tracker(key_names.size() + observable_names.size(),
                            key_names.size());

  reader.visit_entries(
      [&tracker, position_idx, observable_idx](attribute_list const &record) {
        attribute const &key = record[position_idx];
        dataframe_tracker::iterator it;
        bool inserted;
        std::tie(it, inserted) = tracker.insert(key);
        it->get(1) = record[observable_idx];
      });

  tracker.visit_records([](attribute_list const &record) {
    if(record[0] == attribute{1, 2, 3})
      BOOST_REQUIRE_EQUAL(record[1], attribute(1.23));
    if(record[1] == attribute{6, 7, 8})
      BOOST_REQUIRE_EQUAL(record[1], attribute(5.32));
  });
}

BOOST_AUTO_TEST_SUITE_END()
