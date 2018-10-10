#include "utils/stream_as_model.hpp"

#include "configurations/cubic_crystal_configuration.hpp"
#include "interface/attribute_list.hpp"
#include "interface/event_kind.hpp"
#include "io/csv_reader.hpp"
#include "utils/stream_as_configuration.hpp"

#include <boost/test/auto_unit_test.hpp>

#include <sstream>

char const configuration_stream_csv[] = //
    R"("position","numeric", "text"
-2, -2.2, initial
-1, -1.1, initial
+0, -0.0, initial
+1, +1.1, initial
+2, +2.2, initial
)";

char const event_stream_csv[] = //
    R"("position", "event.kind", "numeric", "text"
-2, 2, -2.2, "removing"
-2, 1, -2.2, "not the same particle"
+3, 1, +3.3, "a new particle"
+0, 0,  0.0, "initial"
+1, 2,  1.1, "removing"
)";

namespace core = simbad::core;

BOOST_AUTO_TEST_CASE(test_stream_as_model)
{
  // prepare initial configuration
  std::stringstream initial_configuration_stream(configuration_stream_csv);
  core::stream_as_configuration const initial_configuration(
      std::unique_ptr<core::stream_reader>(
          new core::csv_reader(initial_configuration_stream, ",")));
  BOOST_CHECK_EQUAL(5, initial_configuration.size());

  std::string const key_name = "position";
  std::vector<std::string> const other_attributes = {"numeric", "text"};

  std::stringstream event_stream(event_stream_csv);
  std::unique_ptr<core::stream_reader> stream_reader_ptr(
      new core::csv_reader(event_stream, ","));

  core::stream_as_model model(std::move(stream_reader_ptr), key_name,
                              other_attributes);

  BOOST_CHECK_EQUAL(0, model.current_configuration().size());
  model.set_configuration(initial_configuration);
  BOOST_CHECK_EQUAL(5, model.current_configuration().size());
  {
    core::attribute_description event_description = model.event_description();

    std::size_t position_idx = event_description.get_attribute_idx("position");
    std::size_t event_idx = event_description.get_attribute_idx("event.kind");
    std::size_t numeric_idx = event_description.get_attribute_idx("numeric");
    std::size_t text_idx = event_description.get_attribute_idx("text");

    using event = core::attribute_list const &;
    using EVENT = core::EVENT_KIND;
    bool ok;

    ok = model.run(
        [=](event e) {
          BOOST_CHECK_EQUAL(EVENT::REMOVED, e[event_idx].get_event_kind_val());
          BOOST_CHECK_EQUAL(-2, e[position_idx].get_real_val());
        },
        1);
    BOOST_CHECK(ok);
    BOOST_CHECK_EQUAL(4, model.current_configuration().size());

    ok = model.run(
        [=](event e) {
          BOOST_CHECK_EQUAL(EVENT::CREATED, e[event_idx].get_event_kind_val());
          BOOST_CHECK_EQUAL(-2, e[position_idx].get_real_val());
          BOOST_CHECK_EQUAL(-2.2, e[numeric_idx].get_real_val());
          BOOST_CHECK_EQUAL("not the same particle",
                            e[text_idx].get_string_val());
        },
        1);
    BOOST_CHECK(ok);
    BOOST_CHECK_EQUAL(5, model.current_configuration().size());

    ok = model.run(
        [=](event e) {
          BOOST_CHECK_EQUAL(EVENT::CREATED, e[event_idx].get_event_kind_val());
          BOOST_CHECK_EQUAL(3, e[position_idx].get_real_val());
          BOOST_CHECK_EQUAL(3.3, e[numeric_idx].get_real_val());
          BOOST_CHECK_EQUAL("a new particle", e[text_idx].get_string_val());
        },
        1);
    BOOST_CHECK(ok);
    BOOST_CHECK_EQUAL(6, model.current_configuration().size());

    ok = model.run(
        [=](event e) {
          BOOST_CHECK_EQUAL(EVENT::NONE, e[event_idx].get_event_kind_val());
          BOOST_CHECK_EQUAL(0, e[position_idx].get_real_val());
          BOOST_CHECK_EQUAL(0.0, e[numeric_idx].get_real_val());
          BOOST_CHECK_EQUAL("initial", e[text_idx].get_string_val());
        },
        1);
    BOOST_CHECK(ok);
    BOOST_CHECK_EQUAL(6, model.current_configuration().size());

    ok = model.run(
        [=](event e) {
          BOOST_CHECK_EQUAL(EVENT::REMOVED, e[event_idx].get_event_kind_val());
          BOOST_CHECK_EQUAL(1, e[position_idx].get_real_val());
        },
        1);
    BOOST_CHECK(ok);
    BOOST_CHECK_EQUAL(5, model.current_configuration().size());
  }
  core::attribute_description description =
      model.current_configuration().description();

  std::size_t position_idx = description.get_attribute_idx("position");
  std::size_t numeric_idx = description.get_attribute_idx("numeric");
  std::size_t text_idx = description.get_attribute_idx("text");

  std::size_t num_visits = 0;
  model.current_configuration().visit_records(
      [=, &num_visits](core::attribute_list const &record) {
        double position = record[position_idx].get_real_val();
        int id = static_cast<int>(position);

        double numeric = record[numeric_idx].get_real_val();
        std::string text = record[text_idx].get_string_val();

        switch(id)
        {
        case -2:
          BOOST_CHECK_EQUAL(-2.2, numeric);
          BOOST_CHECK_EQUAL("not the same particle", text);
          break;

        case -1:
          BOOST_CHECK_EQUAL(-1.1, numeric);
          BOOST_CHECK_EQUAL("initial", text);
          break;

        case 0:
          BOOST_CHECK_EQUAL(0, numeric);
          BOOST_CHECK_EQUAL("initial", text);
          break;

        case 2:
          BOOST_CHECK_EQUAL(2.2, numeric);
          BOOST_CHECK_EQUAL("initial", text);
          break;

        case 3:
          BOOST_CHECK_EQUAL(3.3, numeric);
          BOOST_CHECK_EQUAL("a new particle", text);
          break;

        default: BOOST_ASSERT(false);
        }
        num_visits++;

      });

  BOOST_CHECK_EQUAL(5, num_visits);
}
