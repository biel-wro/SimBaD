#include <boost/test/auto_unit_test.hpp>

#include "interface/attribute_list.hpp"
#include "io/csv_reader.hpp"
#include "processors/chronicles_builder.hpp"
#include "processors/tracker_record.hpp"
#include "utils/stream_as_configuration.hpp"

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

class checking_chronicles_builder : public core::chronicles_builder
{
public:
  using chronicles_builder::chronicles_builder;

  using checker_fun = std::function<void(core::tracker_record const &,
                                         chronicle_datum const &, double)>;
  void emit_particle(core::tracker_record const &record,
                     chronicle_datum const &datum, double death_time) override
  {
    m_checker(record, datum, death_time);
  }
  checker_fun m_checker;
};

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

  checking_chronicles_builder builder(event_description, "position",
                                      {"property"});

  builder.set_configuration(initial_configuration);

  auto read_visitor = [&builder](core::attribute_list const &event) {
    builder.push_event(event);
  };


          BOOST_REQUIRE(false);
}

BOOST_AUTO_TEST_SUITE_END()
