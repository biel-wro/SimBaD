#include "parameter_evolution_3d.hpp"

#include "configurations/cubic_crystal_configuration.hpp"
#include "interface/attribute_list.hpp"
#include "interface/property_tree.hpp"
#include "io/csv_printer.hpp"
#include "io/text_printer.hpp"
#include "particle.hpp"

#include <boost/property_tree/ptree.hpp>
#include <boost/test/auto_unit_test.hpp>

#include <iostream>

using namespace simbad::core;
using namespace simbad::models::parameter_evolution_3d;

static property_tree make_paramaters()
{
  property_tree pt;
  pt.put("mutation.probability", 0.01);

  pt.put("interaction.sigma", 1);
  pt.put("interaction.gamma", 2);
  pt.put("interaction.tolerance", 0.1);

  pt.put("birth.dispersion.sigma", 1);

  pt.put("birth.saturation.class", "generalized_exponential");
  pt.put("birth.saturation.parameters.sigma", 10);
  pt.put("birth.saturation.parameters.gamma", 2);
  pt.put("birth.saturation.parameters.scale", 10);
  pt.put("birth.mutator.efficiency.class", "skewed_polynomial");
  pt.put("birth.mutator.efficiency.parameters.improvement_probability", 0.1);
  pt.put("birth.mutator.efficiency.parameters.improvement_exponent", 100);
  pt.put("birth.mutator.efficiency.parameters.deterioration_exponent", 1);
  pt.put("birth.mutator.resistance.class", "skewed_polynomial");
  pt.put("birth.mutator.resistance.parameters.improvement_probability", 0.1);
  pt.put("birth.mutator.resistance.parameters.improvement_exponent", 100);
  pt.put("birth.mutator.resistance.parameters.deterioration_exponent", 1);

  pt.put("death.saturation.class", "inverse_generalized_exponential");
  pt.put("death.saturation.parameters.sigma", 10);
  pt.put("death.saturation.parameters.gamma", 2);
  pt.put("death.saturation.parameters.scale", 1000);
  pt.put("death.mutator.efficiency.class", "skewed_polynomial");
  pt.put("death.mutator.efficiency.parameters.improvement_probability", 0.1);
  pt.put("death.mutator.efficiency.parameters.improvement_exponent", 100);
  pt.put("death.mutator.efficiency.parameters.deterioration_exponent", 1);
  pt.put("death.mutator.resistance.class", "skewed_polynomial");
  pt.put("death.mutator.resistance.parameters.improvement_probability", 0.1);
  pt.put("death.mutator.resistance.parameters.improvement_exponent", 100);
  pt.put("death.mutator.resistance.parameters.deterioration_exponent", 1);

  pt.put("success.saturation.class", "generalized_exponential");
  pt.put("success.saturation.parameters.sigma", 1);
  pt.put("success.saturation.parameters.gamma", 2);
  pt.put("success.saturation.parameters.scale", 1);

  pt.put("success.mutator.efficiency.class", "skewed_polynomial");
  pt.put("success.mutator.efficiency.parameters.improvement_probability", 0.1);
  pt.put("success.mutator.efficiency.parameters.improvement_exponent", 100);
  pt.put("success.mutator.efficiency.parameters.deterioration_exponent", 1);
  pt.put("success.mutator.resistance.class", "skewed_polynomial");
  pt.put("success.mutator.resistance.parameters.improvement_probability", 0.1);
  pt.put("success.mutator.resistance.parameters.improvement_exponent", 100);
  pt.put("success.mutator.resistance.parameters.deterioration_exponent", 1);

  pt.put("seed", 0);
  pt.put("space.tile_size", 1);

  return pt;
}

static property_tree make_initial_configuraiton(double radius)
{
  property_tree pt;
  pt.put("dimension", 3);
  pt.put("radius", radius);
  pt.put("spacing", 1);
  return pt;
}

static property_tree make_initial_vals()
{
  property_tree pt;
  pt.put("birth.efficiency", 0.1);
  pt.put("birth.resistance", 0.5);
  pt.put("death.efficiency", 0.1);
  pt.put("death.resistance", 0.5);
  pt.put("success.efficiency", 0.9);
  pt.put("success.resistance", 0.5);
  pt.put("mutation.id", 1);
  pt.put("mutation.parent.id", 0);
  return pt;
}

BOOST_AUTO_TEST_SUITE(test_parameter_evolution_3d)

BOOST_AUTO_TEST_CASE(reading_configuration)
{
  property_tree parameters = make_paramaters();
  parameter_evolution_3d m(parameters);

  cubic_crystal_configuration initial_configuraiton(
      make_initial_configuraiton(3));

  m.set_configuration(initial_configuraiton, make_initial_vals());

  csv_printer printer(&std::cout);

  std::size_t const original_size = initial_configuraiton.size();
  BOOST_REQUIRE_EQUAL(m.current_configuration().size(), original_size);

  std::size_t counter(0);
  m.current_configuration().visit_records(
      [&counter](attribute_list const &p) { ++counter; });

  BOOST_REQUIRE_EQUAL(counter, original_size);
}

BOOST_AUTO_TEST_CASE(short_run)
{
  parameter_evolution_3d m(make_paramaters());

  cubic_crystal_configuration initial_configuraiton(
      make_initial_configuraiton(1));

  m.set_configuration(initial_configuraiton, make_initial_vals());

  //  text_printer configuration_printer(&std::cout);

  csv_printer event_printer(
      &std::cout, ",", 7,
      std::unordered_map<std::string, std::size_t>{{"event.time", 17}});

  event_printer.write_header(m.event_description());
  m.generate_events(
      [&event_printer](attribute_list const &attributes) {
        event_printer.write_entry(attributes);
      },
      100);

  std::cout << std::endl << std::endl << std::endl;
  m.check_accumulators();
  std::cout << std::endl << std::endl << std::endl;

  csv_printer configuration_printer(&std::cout);
  configuration_printer.write_header(m.current_configuration().description());
  configuration_printer.write_data(m.current_configuration());
  configuration_printer.write_footer();
}

BOOST_AUTO_TEST_SUITE_END()
