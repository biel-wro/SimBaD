#ifndef ATTRIBUTE_SCALAR_VISITOR_HPP
#define ATTRIBUTE_SCALAR_VISITOR_HPP
#include "core_def.hpp"

#include "interface/attribute.hpp"

BEGIN_NAMESPACE_CORE
template <class Visitor> struct attribute_scalar_visitor
{
  Visitor v;
  attribute_scalar_visitor(Visitor v = Visitor()) : v(v) {}
  template <class T> void operator()(T &val) { v(val); }
  template <class T> void operator()(T const &val) { v(val); }
  template <class T, std::size_t dim> void operator()(coordinates<T, dim> &val)
  {
    for(T &scalar : val)
      v(scalar);
  }
  template <class T, std::size_t dim>
  void operator()(coordinates<T, dim> &val) const
  {
    for(T const &scalar : val)
      v(scalar);
  }

  template <class T> void operator()(array_attribute<T> &val)
  {
    for(T &scalar : val)
      v(scalar);
  }
  template <class T> void operator()(array_attribute<T> const &val) const
  {
    for(T &scalar : val)
      v(scalar);
  }
};
template <class ScalarVisitor>
void visit_attribute_scalar(attribute &attr, ScalarVisitor sv = ScalarVisitor())
{
  boost::apply_visitor(sv, attr);
}
template <class ScalarVisitor>
void visit_attribute_scalar(attribute const &attr,
                            ScalarVisitor sv = ScalarVisitor())
{
  boost::apply_visitor(sv, attr);
}

END_NAMESPACE_CORE
#endif // ATTRIBUTE_SCALAR_VISITOR_HPP
