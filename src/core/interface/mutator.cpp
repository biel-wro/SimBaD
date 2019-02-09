#include "mutator.hpp"

BEGIN_NAMESPACE_CORE
template <class Scalar> mutator<Scalar>::~mutator() = default;

template class mutator<float>;
template class mutator<double>;

END_NAMESPACE_CORE
