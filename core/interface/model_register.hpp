#ifndef MODEL_REGISTER_HPP
#define MODEL_REGISTER_HPP

#include "model_register.hpp"

#include "core_fwd.hpp"

#include "utils/class_register.hpp"

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
