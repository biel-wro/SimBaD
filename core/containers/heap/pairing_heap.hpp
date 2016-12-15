#ifndef CORE_PAIRING_HEAP
#define CORE_PAIRING_HEAP

#include "core_def.hpp"

#include <boost/intrusive/list.hpp>

#include "pairing_hook.hpp"

BEGIN_NAMESPACE_CORE

template <class T, class Compare> class pairing_heap
{
public:
  using children_list_type = pairing_heap_hook::children_list_type;

private:
  T *m_root;
};

END_NAMESPACE_CORE
#endif
