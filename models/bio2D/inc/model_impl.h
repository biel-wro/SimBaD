#pragma once

#include "model.h"

namespace simbad {
namespace models{
namespace empty_model {

class model_impl : public core::model
{
  friend class factory_impl;

  model_impl() = default;
  model_impl( model_impl const & ) = delete;
  model_impl( model_impl && ) = default;
  model_impl &operator=( model_impl const &) = delete;
  model_impl &operator=( model_impl && ) = default;
  // model interface
public:

  double run(std::size_t maxEvents, double maxTime) noexcept;
  const_iterator begin() const noexcept;
  const_iterator end() const noexcept;
  std::size_t particleCount() const noexcept;
  double simulationTime() const noexcept;
  std::size_t simulationEvents() const noexcept;
  std::string modelName() const noexcept;
  int dimension() const noexcept;

  static const std::string name_str;
  static const std::size_t dimension_int;
private:

};


}}}

