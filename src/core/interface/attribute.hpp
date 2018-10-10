#ifndef PROPERTY_HPP
#define PROPERTY_HPP
#include "interface/interface_fwd.hpp"

#include "coordinates/coordinates.hpp"

#include <boost/variant.hpp>

#include <cstddef>
#include <functional>
#include <initializer_list>
#include <iostream>
#include <string>
#include <vector>
BEGIN_NAMESPACE_CORE
template <class T> class array_attribute : public std::vector<T>
{
public:
  using super = std::vector<T>;
  // using super::super;
  array_attribute() = default;
  array_attribute(super const &s) : super(s) {}
  array_attribute(T const &v) : super{v} {}
  explicit array_attribute(std::size_t sz, T const &v = T()) : super(sz, v) {}
  array_attribute(std::initializer_list<T> il) : super(il) {}
  template <class It> array_attribute(It beg, It end) : super(beg, end) {}
  friend std::ostream &operator<<(std::ostream &os, array_attribute const &vec)
  {
    if(vec.empty())
      return os << "[]";
    os << "[" << vec[0];
    std::for_each(std::next(vec.begin()), vec.end(),
                  [&os](T const &v) { os << ", " << v; });
    return os << "]";
  }

  friend std::istream &operator>>(std::istream &is, array_attribute const &vec)
  {
    throw "not implemented yet";
  }
};

enum class ATTRIBUTE_SCALAR : std::uint32_t
{
  UNKNOWN = 0,
  STRING = 1,
  REAL = 2,
  INT = 3,
};
std::ostream &operator<<(std::ostream &, ATTRIBUTE_SCALAR);
std::istream &operator>>(std::istream &, ATTRIBUTE_SCALAR);
// clang-format off
#define SIMBAD_CORE_ATTRIBUTE_SUBTYPES                                         \
  std::string,                                                                 \
  double,                                                                      \
  std::int64_t,                                                                \
  array_attribute<double>,                                                     \
  array_attribute<std::int64_t>,                                               \
  array_attribute<std::string>,                                                \
  coordinates<double, 2>,                                                      \
  coordinates<double, 3>,                                                      \
  coordinates<std::int64_t, 2>,                                                \
  coordinates<std::int64_t, 3>
// clang-format on

class attribute : public boost::variant<SIMBAD_CORE_ATTRIBUTE_SUBTYPES>
{
public:
  // types
  using super = boost::variant<SIMBAD_CORE_ATTRIBUTE_SUBTYPES>;
  using self_type = attribute;

  // scalar subtypes
  using string_type = std::string;
  using real_type = double;
  using int_type = std::int64_t;

  // array subtypes
  using int2_type = coordinates<int_type, 2>;
  using int3_type = coordinates<int_type, 3>;
  using intn_type = array_attribute<int_type>;
  using real2_type = coordinates<real_type, 2>;
  using real3_type = coordinates<real_type, 3>;
  using realn_type = array_attribute<real_type>;
  using stringn_type = array_attribute<std::string>;

  // iterators
  using int_iterator = int_type *;
  using int_const_iterator = int_type const *;
  using real_iterator = real_type *;
  using real_const_iterator = real_type const *;
  using string_iterator = string_type *;
  using string_const_iterator = string_type const *;

  static constexpr std::size_t max_inplace_dimension() { return 3; }
  // constructors
  attribute();

  // string
  attribute(string_type val);
  attribute(char const *cstr);
  attribute(std::initializer_list<string_type> il);
  attribute(std::initializer_list<char const *> il);
  attribute(array_attribute<string_type> val);

  // integer
  attribute(int_type val);
  attribute(std::initializer_list<std::int64_t> il);
  attribute(int2_type const &val);
  attribute(int3_type const &val);
  attribute(intn_type &&val);
  attribute(intn_type const &val);

  // float
  attribute(real_type val);
  attribute(std::initializer_list<real_type> il);
  attribute(real2_type const &val);
  attribute(real3_type const &val);
  attribute(realn_type &&val);
  attribute(realn_type const &val);

  // other integers
  template <class T, std::enable_if_t<std::is_integral<T>::value, int> = 0>
  attribute(T val);
  template <class T, std::enable_if_t<std::is_integral<T>::value, int> = 0>
  attribute(std::initializer_list<T> il);
  template <class T, std::enable_if_t<std::is_integral<T>::value, int> = 0>
  attribute(coordinates<T, 2> const &val);
  template <class T, std::enable_if_t<std::is_integral<T>::value, int> = 0>
  attribute(coordinates<T, 3> const &val);
  template <class T, std::enable_if_t<std::is_integral<T>::value, int> = 0>
  attribute(std::vector<T> const &val);

  attribute(EVENT_KIND);

  // query
  bool empty() const;
  std::size_t dimension() const;
  ATTRIBUTE_SCALAR scalar() const;

  // access
  attribute get_scalar(std::size_t) const;

  string_type get_string_val(std::size_t idx = 0) const;
  real_type get_real_val(std::size_t idx = 0) const;
  int_type get_int_val(std::size_t idx = 0) const;

  string_type &get_string_ref(std::size_t i = 0);
  string_type const &get_string_ref(std::size_t i = 0) const;
  real_type &get_real_ref(std::size_t i = 0);
  real_type const &get_real_ref(std::size_t i = 0) const;
  int_type &get_integer_ref(std::size_t i = 0);
  int_type const &get_integer_ref(std::size_t i = 0) const;

  std::pair<string_iterator, string_iterator> get_string_data();
  std::pair<string_const_iterator, string_const_iterator>
  get_string_data() const;
  std::pair<real_iterator, real_iterator> get_real_data();
  std::pair<real_const_iterator, real_const_iterator> get_real_data() const;
  std::pair<int_iterator, int_iterator> get_int_data();
  std::pair<int_const_iterator, int_const_iterator> get_int_data() const;

  EVENT_KIND get_event_kind_val(std::size_t idx = 0) const;

  // modify
  void clear();

  // relational operators
  bool operator==(attribute const &rhs) const;
  bool operator!=(attribute const &rhs) const;

  // hash
  std::size_t hash_value() const;
};

#undef SIMBAD_CORE_ATTRIBUTE_SUBTYPES

END_NAMESPACE_CORE

namespace std
{
template <> struct hash<::simbad::core::attribute>
{
  size_t operator()(::simbad::core::attribute const &attr) const;
};
}
namespace boost
{
template <> struct hash<::simbad::core::attribute>
{
  std::size_t operator()(::simbad::core::attribute const &attr) const;
};
}

#endif // PROPERTY_HPP
