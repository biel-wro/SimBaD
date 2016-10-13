#ifndef CORE_PAIRING_HEAP_HOOK
#define CORE_PAIRING_HEAP_HOOK

#include "core_def.hpp"

#include <boost/intrusive/list.hpp>

BEGIN_NAMESPACE_CORE

struct pairing_heap_hook
{
  using sibling_hook_type = boost::intrusive::list_member_hook<>;
  sibling_hook_type m_sibling_hook;

  using children_list_type = boost::intrusive::list<
      pairing_heap_hook,
      boost::intrusive::member_hook<pairing_heap_hook, sibling_hook_type,
                                    &pairing_heap_hook::m_sibling_hook>>;
  children_list_type m_children;
};

END_NAMESPACE_CORE
#endif
