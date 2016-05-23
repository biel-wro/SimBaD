#ifndef MODEL_LOADER_HPP
#define MODEL_LOADER_HPP

#include "core_fwd.hpp"

#include "model_register.hpp"
#include <memory>

BEGIN_NAMESPACE_CORE

class model_loader
{
public:


  template <class Factory>
  void load_by_type(model_register &target)
  {
    std::unique_ptr<const model_factory> f(new Factory());
    target.register_factory(std::move(f));
  }
};

END_NAMESPACE_CORE

#endif // MODEL_LOADER_HPP
