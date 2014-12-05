
#include <limits>
#include "model_impl.h"

namespace simbad {
namespace models{
namespace empty_model {

std::string const model_impl::name_str = "bio2D";

double model_impl::run(std::size_t maxEvents, double maxTime) noexcept
{
  (void) maxEvents;
  (void) maxTime;
  return simulationTime();
}

core::model::const_iterator model_impl::begin() const noexcept{
  throw "not implemented yet";
}

core::model::const_iterator model_impl::end() const noexcept{
  throw "not implemented yet";
}

std::size_t model_impl::particleCount() const noexcept{
  return 0;
}

double model_impl::simulationTime() const noexcept{
  return std::numeric_limits<double>::infinity();
}

std::size_t model_impl::simulationEvents() const noexcept{
  return 0;
}

std::string model_impl::modelName() const noexcept{
  return name_str;
}

int model_impl::dimension() const noexcept{
  return 0;
}


}}}
