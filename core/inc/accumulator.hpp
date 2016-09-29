#ifndef CORE_ACCUMULATOR_HPP
#define CORE_ACCUMULATOR_HPP
#include "core_def.hpp"
#include <utility>

BEGIN_NAMESPACE_CORE
template <class Accumulated> class accumulator
{
public:
  using accumulated_type = Accumulated;

  explicit accumulator(accumulated_type initval = accumulated_type(0))
      : m_acc(initval)
  {
  }
  void reset(accumulated_type initval = 0) { m_acc = initval; }
  template <class Fun, class... FunArgs>
  void include(Fun fun, FunArgs &&... args)
  {
    m_acc += fun(std::forward<FunArgs>(args)...);
  }

  template <class Fun, class... FunArgs>
  void exclude(Fun fun, FunArgs &&... args)
  {
    m_acc -= fun(std::forward<FunArgs>(args)...);
  }

  accumulated_type current_value() const { return m_acc; }
protected:
  accumulated_type m_acc;
};
END_NAMESPACE_CORE
#endif
