#ifndef MODEL_REGISTER_HPP
#define MODEL_REGISTER_HPP

#include "model_register.hpp"

#include "interface/interface_fwd.hpp"

#include "interface/class_register.hpp"

#include <boost/iterator/transform_iterator.hpp>

#include <memory>
#include <string>
#include <unordered_map>

BEGIN_NAMESPACE_CORE
class model_register : public class_register<model>
{
public:
  virtual ~model_register();
};

END_NAMESPACE_CORE

#endif // MODEL_REGISTER_HPP
