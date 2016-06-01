#ifndef MODEL_FACTORY_HPP
#define MODEL_FACTORY_HPP

#include "core_fwd.hpp"

#include <memory>
#include <string>

#include <boost/property_tree/ptree_fwd.hpp>

BEGIN_NAMESPACE_CORE

class model_factory
{
public:
  using params = boost::property_tree::ptree;

  model_factory();
  virtual ~model_factory();

  virtual std::unique_ptr<model>
  create_instance(params const &mp) const = 0;
  virtual std::string model_name() const = 0;
  virtual std::size_t dimension() const = 0;
};
END_NAMESPACE_CORE

#endif // MODEL_FACTORY_HPP
