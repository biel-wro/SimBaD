#ifndef SPACE_HPP
#define SPACE_HPP

#include "space_impl.hpp"
BEGIN_NAMESPACE_CORE
template <class T, class config>
class space
{
public:
  // member types
  using implementation_type = space_impl<T, config>;
  using iterator = typename implementation_type::iterator;
  using space_coords = typename implementation_type::space_coords;

  std::size_t size() const { return m_impl.size(); }
  template <class... Args>
  iterator emplace(space_coords const &sc, Args... args)
  {
    return m_impl.emplace(sc, args...);
  }


private:
  implementation_type m_impl;
};

END_NAMESPACE_CORE

#endif // SPACE_HPP
