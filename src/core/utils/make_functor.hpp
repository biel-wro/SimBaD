#ifndef MAKE_FUNCTOR_HPP
#define MAKE_FUNCTOR_HPP
#include "core_fwd.hpp"
#include <functional>

BEGIN_NAMESPACE_CORE
template <class... Fs> struct functor;

template <class F> struct functor<F>
{
  F f;
  functor(F &&f) : f(f) {}
  template <class... Args>
  decltype(std::declval<F>()(std::forward<Args>...))
  operator()(Args &&args) const
  {
    return f(std::forward<Args>(args)...);
  }
};

template <class F0, class... Fs>
struct functor<F0, Fs...> : functor<F0>, functor<Fs...>
{
  using functor<F0>::operator();
  using functor<Fs...>::operator();

  functor(F0 &&f0, Fs &&... fs)
      : functor<F0>(std::forward(f0)), functor<Fs...>(std::forward<Fs>(fs)...)
  {
  }
};

template <class... Fs> functor<Fs...> make_functor(Fs... fs)
{
  return functor(std::forward<Fs>(fs)...);
}

END_NAMESPACE_CORE
#endif // MAKE_FUNCTOR_HPP
