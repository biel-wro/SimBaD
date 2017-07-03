#ifndef MODEL_FACTORY_HPP
#define MODEL_FACTORY_HPP

#include "interface_fwd.hpp"

#include "utils/object_factory.hpp"

#include <memory>
#include <string>

BEGIN_NAMESPACE_CORE
/*
class model_factory
{
public:
  using params = property_tree;
  model_factory();
  virtual ~model_factory();

  virtual std::unique_ptr<model> create_instance(params const &mp) const = 0;
  virtual std::string model_name() const = 0;
  virtual std::size_t dimension() const = 0;
};
*/
class model_factory : public object_factory<model>
{
public:
  using params = property_tree;
  virtual std::size_t dimension() const = 0;
  virtual std::string model_name() const = 0;
  std::string class_name() const override;
  model_factory();
  virtual ~model_factory();
};

END_NAMESPACE_CORE

#define SIMBAD_MAKE_MODEL_FACTORY(modelname, DIMENSION)                        \
  namespace simbad                                                             \
  {                                                                            \
  namespace models                                                             \
  {                                                                            \
  namespace modelname                                                          \
  {                                                                            \
  class modelname##_factory : public simbad::core::model_factory               \
  {                                                                            \
  public:                                                                      \
    modelname##_factory() {}                                                   \
    ~modelname##_factory() override {}                                         \
    std::unique_ptr<simbad::core::model>                                       \
    create_instance(simbad::core::property_tree const &mp) const override      \
    {                                                                          \
      modelname *p_model = new modelname(mp);                                  \
      return std::unique_ptr<simbad::core::model>(p_model);                    \
    }                                                                          \
    std::string model_name() const override { return #modelname; }             \
    std::size_t dimension() const override { return DIMENSION; }               \
  };                                                                           \
  std::unique_ptr<simbad::core::model_factory> get_factory()                   \
  {                                                                            \
    return std::unique_ptr<simbad::core::model_factory>(                       \
        new modelname##_factory);                                              \
  }                                                                            \
  }                                                                            \
  }                                                                            \
  }

#endif
