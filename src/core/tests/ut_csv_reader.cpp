#define BOOST_SPIRIT_USE_PHOENIX_V3
#define BOOST_SPIRIT_DEBUG
#define BOOST_SPIRIT_DEBUG_TRACENODE 1

#include "interface/attribute.hpp"
#include "interface/attribute_description.hpp"
#include "interface/attribute_list.hpp"
#include "io/csv_reader.hpp"

#include <boost/optional.hpp>
#include <boost/test/auto_unit_test.hpp>

#include <sstream>

using attribute = simbad::core::attribute;
using attribute_description = simbad::core::attribute_description;
using attribute_list = simbad::core::attribute_list;

using csv_reader = simbad::core::csv_reader;

using opt_descriptor =
    boost::optional<simbad::core::attribute_descriptor const &>;
BOOST_AUTO_TEST_SUITE(csv_reader_test)
BOOST_AUTO_TEST_CASE(read_header)
{
  std::string test_value = "aaa_0; aaa_1; aaa_2; bb_0; bb_1; c";
  std::istringstream test_stream(test_value);

  simbad::core::csv_reader reader(&test_stream, ";");
  attribute_description description = reader.read_header();

  opt_descriptor desc_pos = description.get_descriptor("aaa");
  BOOST_REQUIRE(desc_pos);
  BOOST_REQUIRE_EQUAL(desc_pos->attribute_name(), "aaa");
  BOOST_REQUIRE_EQUAL(desc_pos->attribute_idx(), 0);
  BOOST_REQUIRE_EQUAL(desc_pos->attribute_dimension(), 3);

  opt_descriptor desc_id = description.get_descriptor("bb");
  BOOST_REQUIRE(desc_id);
  BOOST_REQUIRE_EQUAL(desc_id->attribute_name(), "bb");
  BOOST_REQUIRE_EQUAL(desc_id->attribute_idx(), 1);
  BOOST_REQUIRE_EQUAL(desc_id->attribute_dimension(), 2);

  opt_descriptor desc_speed = description.get_descriptor("c");
  BOOST_REQUIRE(desc_speed);
  BOOST_REQUIRE_EQUAL(desc_speed->attribute_name(), "c");
  BOOST_REQUIRE_EQUAL(desc_speed->attribute_idx(), 2);
  BOOST_REQUIRE_EQUAL(desc_speed->attribute_dimension(), 1);
}
BOOST_AUTO_TEST_CASE(read_quoted_header)
{
  std::string test_value = R"("aaa_0"; "aaa_1"; "aaa_2"; "bb_0"; "bb_1"; "c")";
  std::istringstream test_stream(test_value);

  simbad::core::csv_reader reader(&test_stream, ";");
  attribute_description description = reader.read_header();

  opt_descriptor desc_pos = description.get_descriptor("aaa");
  BOOST_REQUIRE(desc_pos);
  BOOST_REQUIRE_EQUAL(desc_pos->attribute_name(), "aaa");
  BOOST_REQUIRE_EQUAL(desc_pos->attribute_idx(), 0);
  BOOST_REQUIRE_EQUAL(desc_pos->attribute_dimension(), 3);

  opt_descriptor desc_id = description.get_descriptor("bb");
  BOOST_REQUIRE(desc_id);
  BOOST_REQUIRE_EQUAL(desc_id->attribute_name(), "bb");
  BOOST_REQUIRE_EQUAL(desc_id->attribute_idx(), 1);
  BOOST_REQUIRE_EQUAL(desc_id->attribute_dimension(), 2);

  opt_descriptor desc_speed = description.get_descriptor("c");
  BOOST_REQUIRE(desc_speed);
  BOOST_REQUIRE_EQUAL(desc_speed->attribute_name(), "c");
  BOOST_REQUIRE_EQUAL(desc_speed->attribute_idx(), 2);
  BOOST_REQUIRE_EQUAL(desc_speed->attribute_dimension(), 1);
}
BOOST_AUTO_TEST_CASE(read_records)
{
  std::string test_string =
      "pos_0; pos_1; pos_2; id; speed_0; speed_1; speed_2 \n"
      "11.0; 11.1; 11.2; 111; -1.0; -1.1; -1.2\n"
      "22.0; 22.1; 22.2; 222; -2.0; -2.1; -2.2\n";
  std::istringstream test_stream(test_string);

  simbad::core::csv_reader reader(&test_stream, ";");
  attribute_description description = reader.read_header();

  opt_descriptor desc_pos = description.get_descriptor("pos");
  BOOST_REQUIRE(desc_pos);
  BOOST_REQUIRE_EQUAL(desc_pos->attribute_name(), "pos");
  BOOST_REQUIRE_EQUAL(desc_pos->attribute_idx(), 0);
  BOOST_REQUIRE_EQUAL(desc_pos->attribute_dimension(), 3);

  opt_descriptor desc_id = description.get_descriptor("id");
  BOOST_REQUIRE(desc_id);
  BOOST_REQUIRE_EQUAL(desc_id->attribute_name(), "id");
  BOOST_REQUIRE_EQUAL(desc_id->attribute_idx(), 1);
  BOOST_REQUIRE_EQUAL(desc_id->attribute_dimension(), 1);

  opt_descriptor desc_speed = description.get_descriptor("speed");
  BOOST_REQUIRE(desc_speed);
  BOOST_REQUIRE_EQUAL(desc_speed->attribute_name(), "speed");
  BOOST_REQUIRE_EQUAL(desc_speed->attribute_idx(), 2);
  BOOST_REQUIRE_EQUAL(desc_speed->attribute_dimension(), 3);

  std::size_t read_cnt = 0;
  reader.visit_entries([&read_cnt](attribute_list const &attributes) {
    if(0 == read_cnt)
    {
      BOOST_REQUIRE_EQUAL(attributes[0].get_real_val(0), 11.0);
      BOOST_REQUIRE_EQUAL(attributes[0].get_real_val(1), 11.1);
      BOOST_REQUIRE_EQUAL(attributes[0].get_real_val(2), 11.2);

      BOOST_REQUIRE_EQUAL(attributes[1].get_int_val(0), 111);

      BOOST_REQUIRE_EQUAL(attributes[2].get_real_val(0), -1.0);
      BOOST_REQUIRE_EQUAL(attributes[2].get_real_val(1), -1.1);
      BOOST_REQUIRE_EQUAL(attributes[2].get_real_val(2), -1.2);
    }
    else if(1 == read_cnt)
    {
      BOOST_REQUIRE_EQUAL(attributes[0].get_real_val(0), 22.0);
      BOOST_REQUIRE_EQUAL(attributes[0].get_real_val(1), 22.1);
      BOOST_REQUIRE_EQUAL(attributes[0].get_real_val(2), 22.2);

      BOOST_REQUIRE_EQUAL(attributes[1].get_int_val(0), 222);

      BOOST_REQUIRE_EQUAL(attributes[2].get_real_val(0), -2.0);
      BOOST_REQUIRE_EQUAL(attributes[2].get_real_val(1), -2.1);
      BOOST_REQUIRE_EQUAL(attributes[2].get_real_val(2), -2.2);
    }
    else
      BOOST_REQUIRE(false);

    read_cnt++;
  });
}

BOOST_AUTO_TEST_CASE(csv_reader_some_bad_record)
{
  std::string data =
      R"("position_0";"position_1";"position_2";"density"
            -14.6503;   -74.2607;     -12.9507;    1e10)";

  std::istringstream test_stream(data);
  simbad::core::csv_reader reader(&test_stream, ";");
  simbad::core::attribute_description description = reader.read_header();
  reader.visit_entries([](attribute_list const &v){},1);
}

BOOST_AUTO_TEST_SUITE_END()
