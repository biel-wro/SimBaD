
#include "interface/extractor.hpp"

BEGIN_NAMESPACE_CORE
template<class S>
extractor<S>::~extractor() = default;

template class extractor<float>;
template class extractor<double>;

END_NAMESPACE_CORE