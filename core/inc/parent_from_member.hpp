#ifndef PARENT_FROM_MEMBER_HPP
#define PARENT_FROM_MEMBER_HPP

#include <cstddef>

#include "core_def.hpp"

BEGIN_NAMESPACE_CORE

template <class Parent, class Member>
std::ptrdiff_t
offset_from_pointer_to_member(const Member Parent::*ptr_to_member)
{
  std::ptrdiff_t p = 0;
  Parent const *parent = reinterpret_cast<Parent const *>(p);
  Member const *member = &((*parent).*ptr_to_member);
  return reinterpret_cast<const char *>(member) -
         reinterpret_cast<const char *>(parent);
}

template <class Parent, class Member>
inline Parent *parent_from_member(Member *member,
                                  const Member Parent::*ptr_to_member)
{
  return static_cast<Parent *>(
      static_cast<void *>(static_cast<char *>(static_cast<void *>(member)) -
                          offset_from_pointer_to_member(ptr_to_member)));
}

template <class Parent, class Member>
inline const Parent *parent_from_member(const Member *member,
                                        const Member Parent::*ptr_to_member)
{
  return static_cast<const Parent *>(static_cast<const void *>(
      static_cast<const char *>(static_cast<const void *>(member)) -
      offset_from_pointer_to_member(ptr_to_member)));
}

END_NAMESPACE_CORE
#endif // PARENT_FROM_MEMBER_HPP
