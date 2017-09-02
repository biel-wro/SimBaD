#ifndef PROPERTY_HPP
#define PROPERTY_HPP
#include "interface_fwd.hpp"

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
  using super::super;
  // array_attribute(super const &s) : super(s) {}
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
  UNKNOWN,
  STRING,
  REAL,
  INT,
};

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
  attribute(std::string val);
  attribute(char const *cstr);
  attribute(array_attribute<std::string> val);

  // integer
  attribute(int_type val);
  attribute(std::initializer_list<std::int64_t> il);
  attribute(int2_type const &val);
  attribute(int3_type const &val);
  attribute(array_attribute<int_type> &&val);
  attribute(array_attribute<int_type> const &val);


  /*
  // std::size_t
  attribute(std::initializer_list<std::size_t> val);
  attribute(std::size_t val);
  attribute(coordinates<std::size_t, 2> const &val);
  attribute(coordinates<std::size_t, 3> const &val);
  attribute(std::vector<std::size_t> const &val);
*/
  // float
  attribute(real_type val);
  attribute(std::initializer_list<real_type> il);
  attribute(coordinates<real_type, 2> const &val);
  attribute(coordinates<real_type, 3> const &val);
  attribute(array_attribute<real_type> &&val);
  attribute(array_attribute<real_type> const &val);

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

  // modify
  void clear();

  // relational operators
  bool operator==(attribute const &rhs) const;
  bool operator!=(attribute const &rhs) const;

  // hash
  std::size_t hash() const;
};

#undef SIMBAD_CORE_ATTRIBUTE_SUBTYPES

END_NAMESPACE_CORE

namespace std
{
template <> struct hash<::simbad::core::attribute>
//    : public boost::hash<::simbad::core::attribute::super>
{
  std::size_t operator()(::simbad::core::attribute const &attr) const
  {
    return attr.hash();
  }
};
}

#endif // PROPERTY_HPP
