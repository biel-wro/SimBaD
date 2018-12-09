#include <boost/test/auto_unit_test.hpp>

#include "interface/attribute_description.hpp"
#include "interface/attribute_list.hpp"
#include "io/csv_printer.hpp"
#include "io/csv_reader.hpp"
#include "processors/chronicles_builder.hpp"
#include "processors/printing_chronicle_builder.hpp"
#include "processors/tracker_record.hpp"
#include "utils/stream_as_configuration.hpp"

#include <sstream>

BOOST_AUTO_TEST_SUITE(test_chronicle_builder)

// clang-format off
constexpr char initial_configuration_csv[] =
R"( position, property
    1.0,      1.1
    2.0,      2.1
    3.0,      3.1
)";

constexpr char event_stream_csv[] =
R"( position, property, event.time, event.delta_time, event.kind
    1.0,      1.1,      0.1,        0,                2
    2.0,      2.2,      0.2,        1,                4
    2.1,      2.3,      0.2,        0,                1
    3.0,      3.3,      0.3,        0,                2
    2.2,      2.4,      0.4,        1,                1
    2.1,      2.5,      0.4,        0,                4
)";
// clang-format on

namespace core = simbad::core;

BOOST_AUTO_TEST_CASE(smoke)
{
  // prepare initial configuration
  std::stringstream initial_configuration_stream(initial_configuration_csv);
  core::stream_as_configuration const initial_configuration(
      std::unique_ptr<core::stream_reader>(
          new core::csv_reader(initial_configuration_stream, ",")));
  BOOST_CHECK_EQUAL(3, initial_configuration.size());

  std::stringstream stream(event_stream_csv);
  core::csv_reader reader(&stream);
  core::attribute_description event_description = reader.read_header();
  event_description.standardize_record("position");

  std::stringstream output;

  core::csv_printer printer{&output, "; ", 3};
  core::printing_chronicle_builder builder(event_description, "position",
                                           {"property"}, printer);

  builder.set_configuration(initial_configuration);

  auto read_visitor = [&builder](core::attribute_list const &event) {
    builder.push_event(event);
  };

  builder.write_header();
  reader.visit_entries(read_visitor, 1000);
  builder.write_footer();

  std::string line;
  std::getline(output, line);
  BOOST_REQUIRE_EQUAL(
      line,
      R"("id"; "parent.id"; "birth_time"; "death_time"; "position"; "property")");

  std::getline(output, line);
  BOOST_REQUIRE_EQUAL(line, "1; 0; -inf; 0.1; 1; 1.1");

  std::getline(output, line);
  BOOST_REQUIRE_EQUAL(line, "2; 0; -inf; 0.2; 2; 2.1");

  std::getline(output, line);
  BOOST_REQUIRE_EQUAL(line, "3; 0; -inf; 0.3; 3; 3.1");

  std::getline(output, line);
  BOOST_REQUIRE_EQUAL(line, "5; 2; 0.2; 0.4; 2.1; 2.3");

  // has 3 more lines
  BOOST_REQUIRE(std::getline(output, line));
  BOOST_REQUIRE(std::getline(output, line));
  BOOST_REQUIRE(std::getline(output, line));

  BOOST_REQUIRE(!std::getline(output, line));
}

BOOST_AUTO_TEST_SUITE_END()
