#include "interface/advance_estimator.hpp"
#include "advance_estimator.hpp"

#include <limits>
BEGIN_NAMESPACE_CORE

float advance_estimator::progress() const {
  return std::numeric_limits<float>::quiet_NaN();
}

advance_estimator::~advance_estimator() = default;


END_NAMESPACE_CORE