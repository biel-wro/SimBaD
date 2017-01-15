#include "interface/attribute.hpp"
#include "utils/attribute_converter.hpp"
#include "utils/attribute_exceptions.hpp"
#include <boost/numeric/conversion/cast.hpp>

#include <iostream>
// using namespace boost;

BEGIN_NAMESPACE_CORE

attribute::attribute() : super("") {}
attribute::attribute(std::string val) : super(std::move(val)) {}
attribute::attribute(const char *cstr) : attribute(std::string(cstr)) {}
attribute::attribute(attribute_array<std::string> v) : super(std::move(v)) {}
attribute::attribute(std::int64_t val) : super(std::move(val)) {}
attribute::attribute(std::initializer_list<std::int64_t> il)
{
  if(3 == il.size())
    *this = attribute(coordinates<std::int64_t, 3>(il));
  else if(2 == il.size())
    *this = attribute(coordinates<std::int64_t, 2>(il));
  else if(1 == il.size())
    *this = (attribute(*il.begin()));
  else
    *this = attribute_array<std::int64_t>(il);
}
attribute::attribute(const coordinates<int_type, 2> &val) : super(val) {}
attribute::attribute(const coordinates<int_type, 3> &val) : super(val) {}
attribute::attribute(attribute_array<int_type> &&v) : super(std::move(v)) {}
attribute::attribute(const attribute_array<attribute::int_type> &val)
{
  if(3 == val.size())
    super::operator=(attribute_cast<coordinates<int_type, 3>>(val));
  else if(2 == val.size())
    super::operator=(attribute_cast<coordinates<int_type, 2>>(val));
  else if(1 == val.size())
    super::operator=(attribute_cast<int_type>(val));
  else
    super::operator=(val);
}

attribute::attribute(real_type val) : super(val) {}
attribute::attribute(std::initializer_list<real_type> il)
{
  if(3 == il.size())
    *this = attribute(coordinates<real_type, 3>(il));
  else if(2 == il.size())
    *this = attribute(coordinates<real_type, 2>(il));
  else if(1 == il.size())
    *this = (attribute(*il.begin()));
  else
    *this = attribute_array<real_type>(il);
}
attribute::attribute(coordinates<real_type, 2> const &val) : super(val) {}
attribute::attribute(coordinates<real_type, 3> const &val) : super(val) {}
attribute::attribute(attribute_array<real_type> &&val) : super(std::move(val))
{
}
attribute::attribute(attribute_array<real_type> const &val)
{
  if(3 == val.size())
    super::operator=(
        attribute_converter::convert_to<coordinates<real_type, 3>>(val).get());
  else if(2 == val.size())
    super::operator=(
        attribute_converter::convert_to<coordinates<real_type, 2>>(val).get());
  else if(1 == val.size())
    super::operator=(attribute_converter::convert_to<real_type>(val).get());
  else
    super::operator=(val);
}
namespace
{
struct empty_checker
{
  using result_type = bool;
  template <class T> bool operator()(T const &) const { return false; }
  bool operator()(std::string const &str) const { return str.empty(); }
};
}
bool attribute::empty() const
{
  return boost::apply_visitor(empty_checker(), *this);
}

namespace
{
struct dimension_checker
{
  using result_type = std::size_t;
  template <class T, std::size_t N>
  std::size_t operator()(coordinates<T, N> const &) const
  {
    return N;
  }
  template <class T> std::size_t operator()(attribute_array<T> const &v) const
  {
    return v.size();
  }
  template <class T> std::size_t operator()(T const &v) const { return 1; }
};
}

std::size_t attribute::dimension() const
{
  return boost::apply_visitor(dimension_checker(), *this);
}

namespace
{
struct scalar_getter
{
  using result_type = attribute;
  std::size_t m_idx;
  scalar_getter(std::size_t idx) : m_idx(idx) {}
  template <class T, std::size_t N>
  attribute operator()(coordinates<T, N> const &v) const
  {
    assert(N > m_idx);
    return v[m_idx];
  }
  template <class T> attribute operator()(attribute_array<T> const &v) const
  {
    assert(v.size() > m_idx);
    return v[m_idx];
  }
  template <class T> attribute operator()(T const &v) const
  {
    assert(0 == m_idx);
    return v;
  }
};
}

attribute attribute::get_scalar(std::size_t idx) const
{
  return boost::apply_visitor(scalar_getter(idx), *this);
}

attribute::real_type attribute::get_real_val(std::size_t idx) const
{
  attribute scalar = get_scalar(idx);
  return attribute_cast<real_type>(scalar);
}
namespace
{
template <class Ref, class Bind> struct getter
{
  using result_type = Ref;
  std::size_t const m_idx;
  getter(std::size_t idx) : m_idx(idx) {}
  Ref operator()(Ref v) const
  {
    assert(0 == m_idx);
    return v;
  }
  template <std::size_t N> Ref operator()(coordinates<Bind, N> &v) const
  {
    assert(m_idx < N);
    return v[m_idx];
  }
  template <std::size_t N> Ref operator()(coordinates<Bind, N> const &v) const
  {
    assert(m_idx < N);
    return v[m_idx];
  }
  Ref operator()(attribute_array<Bind> &v) const
  {
    assert(m_idx < v.size());
    return v[m_idx];
  }
  Ref operator()(attribute_array<Bind> const &v) const
  {
    assert(m_idx < v.size());
    return v[m_idx];
  }
  template <class Other> Ref operator()(Other const &) const
  {
    throw bad_attribute();
  }
};
}
attribute::string_type &attribute::get_string_ref(std::size_t i)
{
  return boost::apply_visitor(getter<string_type &, string_type>(i), *this);
}
attribute::string_type const &attribute::get_string_ref(std::size_t i) const
{
  using boost::apply_visitor;
  return apply_visitor(getter<string_type const &, string_type>(i), *this);
}
attribute::real_type &attribute::get_real_ref(std::size_t i)
{
  return boost::apply_visitor(getter<real_type &, real_type>(i), *this);
}
attribute::real_type const &attribute::get_real_ref(std::size_t i) const
{
  return boost::apply_visitor(getter<real_type const &, real_type>(i), *this);
}
attribute::int_type &attribute::get_integer_ref(std::size_t i)
{
  return boost::apply_visitor(getter<int_type &, int_type>(i), *this);
}
attribute::int_type const &attribute::get_integer_ref(std::size_t i) const
{
  return boost::apply_visitor(getter<int_type const &, int_type>(i), *this);
}

void attribute::clear() { *this = ""; }
namespace
{
struct equal_visitor
{
  using result_type = bool;
  template <class T1, class T2>
  typename std::enable_if<!std::is_same<T1, T2>::value, bool>::type
  operator()(T1 const &, T2 const &) const
  {
    return false;
  }
  template <class T> bool operator()(T const &lhs, T const &rhs) const
  {
    return lhs == rhs;
  }
};
}

bool attribute::operator==(const attribute &rhs) const
{
  return boost::apply_visitor(equal_visitor(), *this, rhs);
}
bool attribute::operator!=(const attribute &rhs) const
{
  return !this->operator==(rhs);
}
namespace
{
struct hashing_visitor
{
  using result_type = std::size_t;
  template <class T> std::size_t operator()(T const &t) const
  {
    return boost::hash<T>()(t);
  }
};
}

std::size_t attribute::hash() const
{
  return boost::apply_visitor(hashing_visitor(), *this);
}

END_NAMESPACE_CORE
