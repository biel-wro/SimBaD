#ifndef PROPERTY_HPP
#define PROPERTY_HPP
#include "core_fwd.hpp"

#include "coordinates/coordinates.hpp"

#include <boost/variant.hpp>
#include <cstddef>

#include <functional>
#include <initializer_list>
#include <iostream>
#include <string>
#include <vector>
BEGIN_NAMESPACE_CORE
template <class T> class attribute_array : public std::vector<T>
{
public:
  using super = std::vector<T>;
  using super::super;

  friend std::ostream &operator<<(std::ostream &os, attribute_array const &vec)
  {
    if(vec.empty())
      return os << "[]";
    os << "[" << vec[0];
    std::for_each(std::next(vec.begin()), vec.end(),
                  [&os](T const &v) { os << ", " << v; });
    return os << "]";
  }

  friend std::istream &operator>>(std::istream &is, attribute_array const &vec)
  {
    throw "not implemented yet";
  }
};

#define SIMBAD_CORE_ATTRIBUTE_SUBTYPES                                         \
  std::string, double, std::int64_t, attribute_array<double>,                  \
      attribute_array<std::int64_t>, attribute_array<std::string>,             \
      coordinates<double, 2>, coordinates<double, 3>,                          \
      coordinates<std::int64_t, 2>, coordinates<std::int64_t, 3>

class attribute : public boost::variant<SIMBAD_CORE_ATTRIBUTE_SUBTYPES>
{
public:
  using super = boost::variant<SIMBAD_CORE_ATTRIBUTE_SUBTYPES>;
  using self_type = attribute;
  using string_type = std::string;
  using real_type = double;
  using int_type = std::int64_t;
  static constexpr std::size_t max_inplace_dimension() { return 3; }
  // constructors
  attribute();

  attribute(std::string val);
  attribute(char const *cstr);
  attribute(attribute_array<std::string> val);

  attribute(int_type val);
  attribute(std::initializer_list<std::int64_t> il);
  attribute(coordinates<int_type, 2> const &val);
  attribute(coordinates<int_type, 3> const &val);
  attribute(attribute_array<int_type> &&val);
  attribute(attribute_array<int_type> const &val);

  attribute(real_type val);
  attribute(std::initializer_list<real_type> il);
  attribute(coordinates<real_type, 2> const &val);
  attribute(coordinates<real_type, 3> const &val);
  attribute(attribute_array<real_type> &&val);
  attribute(attribute_array<real_type> const &val);
  // query
  bool empty() const;

  // access
  string_type &get_string_ref(std::size_t i=0);
  string_type const &get_string_ref(std::size_t i=0) const;
  real_type &get_real_ref(std::size_t i = 0);
  real_type const &get_real_ref(std::size_t i = 0) const;
  int_type &get_integer_ref(std::size_t i = 0);
  int_type const &get_integer_ref(std::size_t i = 0) const;

  // modify
  void clear();

  // relational operators
  bool operator==(attribute const &rhs) const;
  bool operator!=(attribute const &rhs) const;
};

#undef SIMBAD_CORE_ATTRIBUTE_SUBTYPES

END_NAMESPACE_CORE

namespace std
{
template <>
struct hash<::simbad::core::attribute>
    : public boost::hash<::simbad::core::attribute::super>
{
};
}

#endif // PROPERTY_HPP
