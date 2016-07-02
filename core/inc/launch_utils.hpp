#ifndef LAUNCH_UTILS_HPP
#define LAUNCH_UTILS_HPP

#include "core_fwd.hpp"
#include <boost/property_tree/ptree_fwd.hpp>
#include <memory>

BEGIN_NAMESPACE_CORE
class launch_utils {
public:
  launch_utils();
};
std::unique_ptr<model> load_model(boost::property_tree::ptree const &model_properites,
                                  model_register const &mr);

END_NAMESPACE_CORE

#endif // LAUNCH_UTILS_HPP
