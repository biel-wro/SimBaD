#include "launch_utils.hpp"

#include <boost/property_tree/ptree.hpp>

#include "model.hpp"
#include "model_factory.hpp"
#include "model_register.hpp"

BEGIN_NAMESPACE_CORE

launch_utils::launch_utils() {}

std::unique_ptr<model> load_model(const boost::property_tree::ptree &model_properites,
                                  const model_register &mr) {

  std::string model_class_name = model_properites.get<std::string>("class");

  boost::property_tree::ptree const &parameters_subtree =
      model_properites.get_child("parameters");

  simbad::core::model_factory const &factory =
      mr.get_model_factory(model_class_name);
  return factory.create_instance(parameters_subtree);
}

END_NAMESPACE_CORE
