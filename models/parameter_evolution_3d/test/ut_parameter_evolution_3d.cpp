#include "parameter_evolution_3d.hpp"

#include "configurations/cubic_crystal_configuration.hpp"
#include "interface/event.hpp"
#include "interface/property_tree.hpp"
#include "particle.hpp"
#include "processors/csv_printer.hpp"
#include "processors/text_configuration_printer.hpp"

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

  pt.put("birth.saturation.sigma", 10);
  pt.put("birth.saturation.gamma", 2);
  pt.put("birth.saturation.scale", 10);
  pt.put("birth.efficiency.mutator.class", "skewed_polynomial_mutator");
  pt.put("birth.efficiency.mutator.parameters.improvement_probability", 0.1);
  pt.put("birth.efficiency.mutator.parameters.improvement_exponent", 100);
  pt.put("birth.efficiency.mutator.parameters.deterioration_exponent", 1);
  pt.put("birth.resistance.mutator.class", "skewed_polynomial_mutator");
  pt.put("birth.resistance.mutator.parameters.improvement_probability", 0.1);
  pt.put("birth.resistance.mutator.parameters.improvement_exponent", 100);
  pt.put("birth.resistance.mutator.parameters.deterioration_exponent", 1);

  pt.put("lifespan.saturation.sigma", 10);
  pt.put("lifespan.saturation.gamma", 2);
  pt.put("lifespan.saturation.scale", 1000);
  pt.put("lifespan.efficiency.mutator.class", "skewed_polynomial_mutator");
  pt.put("lifespan.efficiency.mutator.parameters.improvement_probability", 0.1);
  pt.put("lifespan.efficiency.mutator.parameters.improvement_exponent", 100);
  pt.put("lifespan.efficiency.mutator.parameters.deterioration_exponent", 1);
  pt.put("lifespan.resistance.mutator.class", "skewed_polynomial_mutator");
  pt.put("lifespan.resistance.mutator.parameters.improvement_probability", 0.1);
  pt.put("lifespan.resistance.mutator.parameters.improvement_exponent", 100);
  pt.put("lifespan.resistance.mutator.parameters.deterioration_exponent", 1);

  pt.put("success.saturation.sigma", 1);
  pt.put("success.saturation.gamma", 2);
  pt.put("success.saturation.scale", 1);

  pt.put("success.efficiency.mutator.class", "skewed_polynomial_mutator");
  pt.put("success.efficiency.mutator.parameters.improvement_probability", 0.1);
  pt.put("success.efficiency.mutator.parameters.improvement_exponent", 100);
  pt.put("success.efficiency.mutator.parameters.deterioration_exponent", 1);
  pt.put("success.resistance.mutator.class", "skewed_polynomial_mutator");
  pt.put("success.resistance.mutator.parameters.improvement_probability", 0.1);
  pt.put("success.resistance.mutator.parameters.improvement_exponent", 100);
  pt.put("success.resistance.mutator.parameters.deterioration_exponent", 1);

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
  pt.put("lifespan.efficiency", 0.1);
  pt.put("lifespan.resistance", 0.5);
  pt.put("success.efficiency", 0.9);
  pt.put("success.resistance", 0.5);
  return pt;
}

BOOST_AUTO_TEST_SUITE(test_parameter_evolution_3d)

BOOST_AUTO_TEST_CASE(reading_configuration)
{
  parameter_evolution_3d m(make_paramaters());

  cubic_crystal_configuration initial_configuraiton(
      make_initial_configuraiton(3));

  m.set_configuration(initial_configuraiton, make_initial_vals());

  csv_printer printer(&std::cout);


  std::size_t const original_size = initial_configuraiton.size();
  BOOST_REQUIRE_EQUAL(m.size(), original_size);

  std::size_t counter(0);
  m.visit_records([&counter](attribute_list const &p) { ++counter; });

  BOOST_REQUIRE_EQUAL(counter, original_size);
}

BOOST_AUTO_TEST_CASE(short_run)
{
  parameter_evolution_3d m(make_paramaters());

  cubic_crystal_configuration initial_configuraiton(
      make_initial_configuraiton(1));

  m.set_configuration(initial_configuraiton, make_initial_vals());

  //  text_configuration_printer configuration_printer(&std::cout);
  csv_printer configuration_printer(&std::cout);

  m.generate_events([](event const &e) { std::cout << e << std::endl; }, 100);
  m.check_accumulators();
  // configuration_printer.set_configuration(m);
  configuration_printer.write_header(m);
  configuration_printer.write_data(m);
  // configuration_printer.write_footer(m);
}

BOOST_AUTO_TEST_SUITE_END()
