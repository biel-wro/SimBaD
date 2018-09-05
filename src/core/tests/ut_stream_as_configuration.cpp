#include "utils/stream_as_configuration.hpp"

#include "interface/attribute_description.hpp"
#include "interface/attribute_list.hpp"
#include "io/csv_reader.hpp"

#include <boost/test/auto_unit_test.hpp>
#include <boost/numeric/conversion/cast.hpp>

#include <sstream>

char const configuration_stream_csv[] = //
    R"("my id", "position", "numeric property", "text property"
        0, -2, -2.2, "here"
        1, -1, -1.1, "are"
        2,  0,  0.0, "some"
        3, +1,  1.1, "example"
        4, +2,  2.2, "particles"
)";

namespace core = simbad::core;

BOOST_AUTO_TEST_CASE(test_stream_as_configuration)
{
  std::stringstream stream(configuration_stream_csv);
  std::unique_ptr<core::stream_reader> stream_reader_ptr(
      new core::csv_reader(stream, ","));

  core::stream_as_configuration configuration(std::move(stream_reader_ptr));

  BOOST_CHECK_EQUAL(5, configuration.size());

  core::attribute_description const &description = configuration.description();
  std::size_t position_idx = description.get_attribute_idx("position");
  std::size_t numeric_idx = description.get_attribute_idx("numeric property");
  std::size_t text_idx = description.get_attribute_idx("text property");
  std::size_t id_idx = description.get_attribute_idx("my id");

  std::size_t visited = 0;
  auto visitor = [&visited, position_idx, numeric_idx, text_idx,
                  id_idx](core::attribute_list const &particle) {
    visited++;
    std::size_t id =
        boost::numeric_cast<std::size_t>(particle[id_idx].get_int_val());

    double position = particle[position_idx].get_real_val();
    double numeric_property = particle[numeric_idx].get_real_val();
    std::string string_property = particle[text_idx].get_string_val();

    switch(id)
    {
    case 0:
      BOOST_CHECK_EQUAL(-2, position);
      BOOST_CHECK_EQUAL(-2.2, numeric_property);
      BOOST_CHECK_EQUAL("here", string_property);
      break;

    case 1:
      BOOST_CHECK_EQUAL(-1, position);
      BOOST_CHECK_EQUAL(-1.1, numeric_property);
      BOOST_CHECK_EQUAL("are", string_property);
      break;

    case 2:
      BOOST_CHECK_EQUAL(0, position);
      BOOST_CHECK_EQUAL(0, numeric_property);
      BOOST_CHECK_EQUAL("some", string_property);
      break;

    case 3:
      BOOST_CHECK_EQUAL(1, position);
      BOOST_CHECK_EQUAL(1.1, numeric_property);
      BOOST_CHECK_EQUAL("example", string_property);
      break;

    case 4:
      BOOST_CHECK_EQUAL(2, position);
      BOOST_CHECK_EQUAL(2.2, numeric_property);
      BOOST_CHECK_EQUAL("particles", string_property);
      break;

    default: BOOST_ERROR("unexpected position");
    }

  };
  configuration.visit_records(visitor);

  BOOST_CHECK_EQUAL(5, visited);
}