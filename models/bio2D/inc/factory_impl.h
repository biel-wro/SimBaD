#pragma once

#include "model_loader.h"

namespace simbad {
namespace models{
namespace empty_model {

class factory_impl : public core::model_factory
{
  // model_factory interface
public:
  factory_impl();
  factory_impl(factory_impl const &) = delete;
  factory_impl(factory_impl && ) = default;
  factory_impl &operator=(factory_impl const &) = delete;
  factory_impl &operator=(factory_impl && ) = default;
  ~factory_impl() = default;

  std::string model_name() const noexcept override;
  int dimension() const noexcept override;
  const po::options_description &get_opt_desc() const noexcept override;

  std::unique_ptr<core::model>
  create_model(const po::variables_map &args) const noexcept override;
private:
  po::options_description opt_desc;
};


}}}
