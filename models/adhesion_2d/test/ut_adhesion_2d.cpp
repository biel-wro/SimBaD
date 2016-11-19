#include "adhesion_2d.hpp"

#include "interface/model.hpp"
#include "particle.hpp"
#include "poisson_configuration.hpp"
#include "property_tree.hpp"
#include "snapshotter.hpp"

#include "core_fwd.hpp"

#include <iostream>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/test/unit_test.hpp>

#include <iostream>

// using namespace simbad::core;
using namespace simbad::models::adhesion_2d;
using namespace boost::property_tree;
using simbad::core::property_tree;
using simbad::core::model;
using simbad::core::poisson_configuration;
using simbad::core::snapshotter;

static property_tree make_test_model_parameters()
{
  simbad::core::property_tree pt;
  pt.put("max_jump", 0.001);
  pt.put("viscosity", 0.1);
  pt.put("diffusion", 0.01);
  pt.put("potential.depth", 1);
  pt.put("potential.order", 6);
  pt.put("potential.groundstate_location", 1);
  pt.put("potential.tolerance", 0.001);
  pt.put("space.tile_size", 1);
  pt.put("seed", 0);
  return pt;
}
/*
static property_tree make_test_model_properties()
{
  property_tree model_pt;
  model_pt.put("class", "adhesion_2d");
  model_pt.add_child("parameters", make_test_model_parameters());
  return model_pt;
}
*/
static poisson_configuration make_initial_configuration(size_t sz)
{
  property_tree pt;
  pt.put("dimension", 2);
  pt.put("radius", 2);
  pt.put("seed", 0);
  pt.put("size", sz);
  pt.put("ball", true);

  return poisson_configuration(pt);
}

BOOST_AUTO_TEST_SUITE(test_adhesion_2d)

BOOST_AUTO_TEST_CASE(model_params_load)
{
  property_tree params = make_test_model_parameters();

  property_tree model_pt = make_test_model_parameters();
  // xml_parser::write_xml(std::cout,
  // static_cast<property_tree::super>(model_pt),
  //                      xml_writer_make_settings<ptree::key_type>(' ', 4));
  std::unique_ptr<model> ptr(reinterpret_cast<model *>(
      simbad::core::create_from_property_tree<adhesion_2d>(model_pt)));
}

BOOST_AUTO_TEST_CASE(particle_size_check)
{
  BOOST_REQUIRE_EQUAL(sizeof(cell), 8 * sizeof(double));
}

BOOST_AUTO_TEST_CASE(single_particle)
{
  adhesion_2d model(make_test_model_parameters());
  BOOST_TEST_CHECKPOINT("created model");
  poisson_configuration initial_configuration = make_initial_configuration(1);
  BOOST_TEST_CHECKPOINT("created configuration");
  model.read_configuration(initial_configuration);
  BOOST_TEST_CHECKPOINT("read configuration");

  BOOST_REQUIRE_EQUAL(model.configuration_size(), 1);
}

BOOST_AUTO_TEST_SUITE_END()
