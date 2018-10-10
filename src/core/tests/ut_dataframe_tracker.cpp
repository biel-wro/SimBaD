#include <boost/test/auto_unit_test.hpp>

#include "io/csv_printer.hpp"
#include "io/csv_reader.hpp"
#include "processors/dataframe_tracker.hpp"

#include <boost/optional.hpp>
#include <boost/pool/pool.hpp>

#include <iostream>
#include <sstream>
#include <string>
#include <type_traits>

using namespace simbad::core;

BOOST_AUTO_TEST_SUITE(test_dataframe_tracker)

BOOST_AUTO_TEST_CASE(allocation_test)
{
  BOOST_CHECK_EQUAL(tracker_record::attribute_offset(16, 8), 16);
  BOOST_CHECK_EQUAL(tracker_record::attribute_offset(8, 8), 8);
  BOOST_CHECK_EQUAL(tracker_record::attribute_offset(20, 8), 24);
  BOOST_CHECK_EQUAL(tracker_record::attribute_offset(8, 16), 16);
  BOOST_CHECK_EQUAL(tracker_record::attribute_offset(24, 8), 24);
  BOOST_CHECK_EQUAL(tracker_record::attribute_offset(24, 24), 24);
  BOOST_CHECK_EQUAL(tracker_record::attribute_offset(24, 32), 32);
  BOOST_CHECK_EQUAL(tracker_record::attribute_offset(16, 24), 24);

  // std::cout << "sizeof(record):" << sizeof(new_record) << std::endl;
  // std::cout << "sizeof(attribute):" << sizeof(attribute) << std::endl;
  // std::cout << "alignof(attribute):" << alignof(attribute) << std::endl;

  constexpr std::size_t record_length = 7;
  boost::pool<boost::default_user_allocator_malloc_free> pool(
      tracker_record::record_bytes(record_length));

  char *memory_ptr = reinterpret_cast<char *>(pool.malloc());

  tracker_record *record_ptr =
      tracker_record::initialize(memory_ptr, record_length);

  for(std::size_t i = 0; i < record_length; ++i)
  {
    attribute &attr = record_ptr->get(i);
    attr = std::to_string(i);
  }

  for(std::size_t i = 0; i < record_length; ++i)
  {
    attribute const &attr = record_ptr->get(i);
    BOOST_CHECK_EQUAL(attr, std::to_string(i));
  }

  tracker_record::deinitialize(record_ptr, record_length);

  pool.free(memory_ptr);
}

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

  dataframe_tracker tracker(key_names.size() + observable_names.size());

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
