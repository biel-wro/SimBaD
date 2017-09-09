#include "interface/attribute.hpp"
#include "utils/attribute_converter.hpp"
#include "utils/attribute_exceptions.hpp"
#include "utils/attribute_scalar_visitor.hpp"

#include <boost/numeric/conversion/cast.hpp>

#include <iostream>
// using namespace boost;

BEGIN_NAMESPACE_CORE

namespace
{
// clang-format off
template <class T> struct scalar_in{ using type = void;};

struct scalar_string{using type = attribute::string_type;};
struct scalar_real{using type = attribute::real_type;};
struct scalar_int{using type = attribute::int_type;};

template <> struct scalar_in<attribute::string_type> : public scalar_string{};
template <> struct scalar_in<attribute::stringn_type> : public scalar_string{};

template <> struct scalar_in<attribute::real_type> : public scalar_real{};
template <> struct scalar_in<attribute::real2_type> : public scalar_real{};
template <> struct scalar_in<attribute::real3_type> : public scalar_real{};
template <> struct scalar_in<attribute::realn_type> : public scalar_real{};

template <> struct scalar_in<attribute::int_type> : public scalar_int{};
template <> struct scalar_in<attribute::int2_type> : public scalar_int{};
template <> struct scalar_in<attribute::int3_type> : public scalar_int{};
template <> struct scalar_in<attribute::intn_type> : public scalar_int{};

// clang-format on
template <class T> constexpr bool is_scalar()
{
  return std::is_same<typename scalar_in<T>::type, T>::value;
}

template <class T, class Scalar> constexpr bool has_scalar()
{
  return std::is_same<typename scalar_in<T>::type, Scalar>::value;
}
}

attribute::attribute() : super("") {}
attribute::attribute(attribute::string_type val) : super(std::move(val)) {}
attribute::attribute(const char *cstr) : attribute(std::string(cstr)) {}

attribute::attribute(std::initializer_list<string_type> il)
    : super(array_attribute<string_type>(il.begin(), il.end()))
{
}

attribute::attribute(std::initializer_list<const char *> il)
    : super(array_attribute<string_type>(il.begin(), il.end()))
{
}
attribute::attribute(array_attribute<std::string> v) : super(std::move(v)) {}

attribute::attribute(int_type val) : super(std::move(val)) {}
attribute::attribute(std::initializer_list<std::int64_t> il)
{
  if(3 == il.size())
    *this = attribute(coordinates<std::int64_t, 3>(il));
  else if(2 == il.size())
    *this = attribute(coordinates<std::int64_t, 2>(il));
  else if(1 == il.size())
    *this = (attribute(*il.begin()));
  else
    *this = array_attribute<std::int64_t>(il);
}
attribute::attribute(int2_type const &val) : super(val) {}
attribute::attribute(int3_type const &val) : super(val) {}

attribute::attribute(intn_type &&v) : super(std::move(v)) {}
attribute::attribute(intn_type const &val)
{
  std::size_t const size = val.size();

  if(3 == size)
    super::operator=(attribute_converter::convert_to<int3_type>(val).get());
  else if(2 == size)
    super::operator=(attribute_converter::convert_to<int2_type>(val).get());
  else if(1 == size)
    super::operator=(attribute_converter::convert_to<int_type>(val).get());
  else
    super::operator=(val);
}

/*
 * Constructors from real_type
 */

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
    *this = array_attribute<real_type>(il);
}
attribute::attribute(real2_type const &val) : super(val) {}
attribute::attribute(real3_type const &val) : super(val) {}
attribute::attribute(realn_type &&val) : super(std::move(val)) {}
attribute::attribute(realn_type const &val)
{
  std::size_t const size = val.size();

  if(3 == size)
    super::operator=(attribute_converter::convert_to<real3_type>(val).get());
  else if(2 == val.size())
    super::operator=(attribute_converter::convert_to<real2_type>(val).get());
  else if(1 == val.size())
    super::operator=(attribute_converter::convert_to<real_type>(val).get());
  else
    super::operator=(val);
}

/*
 * Template constructors from other integer types
 */
template <class T, std::enable_if_t<std::is_integral<T>::value, int>>
attribute::attribute(T val) : super(static_cast<int_type>(val))
{
}

template <class T, std::enable_if_t<std::is_integral<T>::value, int>>
attribute::attribute(std::initializer_list<T> val)
{
  std::size_t const size = val.size();

  if(3 == size)
    *this = attribute{static_cast<int_type>(*(val.begin())),
                      static_cast<int_type>(*(std::next(val.begin(), 1))),
                      static_cast<int_type>(*(std::next(val.begin(), 2)))};
  else if(2 == size)
    *this = attribute{static_cast<int_type>(*(val.begin())),
                      static_cast<int_type>(*(std::next(val.begin(), 1)))};
  else if(1 == size)
    *this = (attribute_converter::convert_to<int_type>(*(val.begin())).get());
  else
    *this = attribute(std::vector<T>(val));
}

template <class T, std::enable_if_t<std::is_integral<T>::value, int>>
attribute::attribute(coordinates<T, 2> const &val)
    : attribute(int2_type{static_cast<int_type>(val[0]),
                          static_cast<int_type>(val[1])})
{
}

template <class T, std::enable_if_t<std::is_integral<T>::value, int>>
attribute::attribute(coordinates<T, 3> const &val)
    : attribute(int3_type{static_cast<int_type>(val[0]),
                          static_cast<int_type>(val[1]),
                          static_cast<int_type>(val[2])})
{
}

template <class T, std::enable_if_t<std::is_integral<T>::value, int>>
attribute::attribute(const std::vector<T> &val)
{
  std::size_t const size = val.size();
  if(3 == size)
    super::operator=(attribute_converter::convert_to<int3_type>(val).get());
  else if(2 == size)
    super::operator=(attribute_converter::convert_to<int2_type>(val).get());
  else if(1 == size)
    super::operator=(attribute_converter::convert_to<int_type>(val).get());
  else
  {
    intn_type list(size);
    std::copy(val.begin(), val.end(), list.begin());
    super::operator=(std::move(list));
  }
}
#define INSTANTIATE_INTEGER_CONSTRUCTORS(T)                                    \
  template attribute::attribute(T);                                            \
  template attribute::attribute(std::initializer_list<T>);                     \
  template attribute::attribute(coordinates<T, 2> const &);                    \
  template attribute::attribute(coordinates<T, 3> const &);                    \
  template attribute::attribute(std::vector<T> const &);

INSTANTIATE_INTEGER_CONSTRUCTORS(int)
INSTANTIATE_INTEGER_CONSTRUCTORS(std::size_t)

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
struct dimension_visitor
{
  using result_type = std::size_t;
  template <class T, std::size_t N>
  std::size_t operator()(coordinates<T, N> const &) const
  {
    return N;
  }
  template <class T> std::size_t operator()(array_attribute<T> const &v) const
  {
    return v.size();
  }
  template <class T> std::size_t operator()(T const &v) const { return 1; }
};
}

std::size_t attribute::dimension() const
{
  return boost::apply_visitor(dimension_visitor(), *this);
}
namespace
{
struct scalar_type_getter_visitor
{
  // clang-format off
  using result_type = ATTRIBUTE_SCALAR;
  template <class T>
  std::enable_if_t<has_scalar<T, attribute::string_type>(), ATTRIBUTE_SCALAR>
  operator()(T const &) const { return ATTRIBUTE_SCALAR::STRING; }

  template <class T>
  std::enable_if_t<has_scalar<T, attribute::real_type>(), ATTRIBUTE_SCALAR>
  operator()(T const &) const { return ATTRIBUTE_SCALAR::REAL; }

  template <class T>
  std::enable_if_t<has_scalar<T, attribute::int_type>(), ATTRIBUTE_SCALAR>
  operator()(T const &) const { return ATTRIBUTE_SCALAR::INT; }
  // clang-format on
};
}
ATTRIBUTE_SCALAR attribute::scalar() const
{
  return boost::apply_visitor(scalar_type_getter_visitor(), *this);
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
  template <class T> attribute operator()(array_attribute<T> const &v) const
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

attribute::string_type attribute::get_string_val(std::size_t idx) const
{
  attribute scalar = get_scalar(idx);
  return attribute_cast<string_type>(scalar);
}

attribute::real_type attribute::get_real_val(std::size_t idx) const
{
  attribute scalar = get_scalar(idx);
  real_type val = attribute_cast<real_type>(scalar);
  return val;
}

attribute::int_type attribute::get_int_val(std::size_t idx) const
{
  attribute scalar = get_scalar(idx);
  return attribute_cast<int_type>(scalar);
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
  Ref operator()(array_attribute<Bind> &v) const
  {
    assert(m_idx < v.size());
    return v[m_idx];
  }
  Ref operator()(array_attribute<Bind> const &v) const
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

namespace
{
template <class Container, bool is_const> struct iter_getter
{
  using scalar_type = typename scalar_in<Container>::type;
  using iterator_type =
      typename std::conditional<is_const,                                    //
                                typename scalar_in<Container>::type const *, //
                                typename scalar_in<Container>::type *        //
                                >::type;
  using result_type = std::pair<iterator_type, iterator_type>;

  template <class Other>
  std::enable_if_t<!std::is_same<Other, Container>::value, result_type>
  operator()(Other const &) const
  {
    return std::pair<iterator_type, iterator_type>(nullptr, nullptr);
  }

  result_type operator()(scalar_type &val) const
  {
    return result_type(&val, std::next(&val));
  }
  result_type operator()(scalar_type const &val) const
  {
    return result_type(&val, std::next(&val));
  }

  template <std::size_t dim>
  result_type operator()(coordinates<scalar_type, dim> &val) const
  {
    return result_type(val.data(), std::next(val.data(), dim));
  }

  template <std::size_t dim>
  result_type operator()(coordinates<scalar_type, dim> const &val) const
  {
    return result_type(val.data(), std::next(val.data(), dim));
  }

  result_type operator()(array_attribute<scalar_type> &val) const
  {
    return result_type(val.data(), std::next(val.data(), val.size()));
  }

  result_type operator()(array_attribute<scalar_type> const &val) const
  {
    return result_type(val.data(), std::next(val.data(), val.size()));
  }
};
}

std::pair<attribute::string_iterator, attribute::string_iterator>
attribute::get_string_data()
{
  return boost::apply_visitor(iter_getter<string_type, false>(), *this);
}
std::pair<attribute::string_const_iterator, attribute::string_const_iterator>
attribute::get_string_data() const
{
  return boost::apply_visitor(iter_getter<string_type, true>(), *this);
}

std::pair<attribute::real_iterator, attribute::real_iterator>
attribute::get_real_data()
{
  return boost::apply_visitor(iter_getter<real_type, false>(), *this);
}
std::pair<attribute::real_const_iterator, attribute::real_const_iterator>
attribute::get_real_data() const
{
  return boost::apply_visitor(iter_getter<real_type, true>(), *this);
}

std::pair<attribute::int_iterator, attribute::int_iterator>
attribute::get_int_data()
{
  return boost::apply_visitor(iter_getter<int_type, false>(), *this);
}
std::pair<attribute::int_const_iterator, attribute::int_const_iterator>
attribute::get_int_data() const
{
  return boost::apply_visitor(iter_getter<int_type, true>(), *this);
}

void attribute::clear() { *this = ""; }

namespace
{
struct equal_scalar
{
  using result_type = bool;
  using buffer = std::array<char, 64>;

  // string & string
  bool operator()(attribute::string_type const &lhs,
                  attribute::string_type const &rhs) const
  {
    return lhs == rhs;
  }
  // string & numeric
  template <class Rhs>
  bool operator()(attribute::string_type const &lhs, Rhs const &rhs) const
  {
    std::stringstream ss(lhs);
    Rhs lhs_parsed;
    ss >> lhs_parsed;
    if(ss.fail())
      return false;
    return lhs_parsed == rhs;
  }
  // numeric & string
  template <class Lhs>
  bool operator()(Lhs const &lhs, attribute::string_type const &rhs) const
  {
    return operator()(rhs, lhs);
  }

  // numeric & same numeric
  template <class T> bool operator()(T const &lhs, T const &rhs) const
  {
    return lhs == rhs;
  }

  // numeric1 & numeric2
  template <class Lhs, class Rhs>
  bool operator()(Lhs const &lhs, Rhs const &rhs) const
  {
    return static_cast<Rhs>(lhs) == rhs && lhs == static_cast<Lhs>(rhs);
  }
};

struct equal_visitor
{
  using result_type = bool;
  template <class Lhs, class Rhs>
  bool operator()(Lhs const &lhs, Rhs const &rhs) const
  {
    using lgetter = iter_getter<Lhs, true>;
    using rgetter = iter_getter<Rhs, true>;

    typename lgetter::iterator_type lit, lbeg, lend;
    typename rgetter::iterator_type rit, rbeg, rend;

    std::tie(lbeg, lend) = lgetter()(lhs);
    std::tie(rbeg, rend) = rgetter()(rhs);

    if(lend - lbeg != rend - rbeg)
      return false;

    for(lit = lbeg, rit = rbeg; lit != lend; ++lit, ++rit)
    {
      typename scalar_in<Lhs>::type const &lscalar = *lit;
      typename scalar_in<Rhs>::type const &rscalar = *rit;
      if(!equal_scalar()(lscalar, rscalar))
        return false;
    }
    return true;
  }
};
}

bool attribute::operator==(const attribute &rhs) const
{
  return boost::apply_visitor(equal_visitor(), *this, rhs);
}
bool attribute::operator!=(const attribute &rhs) const
{
  return !(operator==(rhs));
}

namespace
{
struct hash_scalar
{
  using result_type = std::size_t;
  result_type operator()(attribute::int_type v) const
  {
    return std::hash<attribute::int_type>()(v);
  }
  result_type operator()(attribute::real_type v) const
  {
    attribute::real_type integral;
    attribute::real_type fraction = std::modf(v, &integral);
    // fraction can be Inf or NaN
    bool is_integer = std::isfinite(v) && 0 == fraction;

    if(is_integer)
      return operator()(attribute::int_type(std::llround(v)));
    return std::hash<attribute::real_type>()(v);
  }
  result_type operator()(attribute::string_type const &str) const
  {
    //TODO: do real parsing (boost::spirit) & dispatch
    std::stringstream ss(str);

    attribute::real_type real_val;
    ss >> real_val;
    if(!ss.fail())
      return operator()(real_val);

    attribute::int_type int_val;
    ss >> int_val;
    if(!ss.fail())
      return operator()(int_val);

    ss.str() = str;


    return std::hash<attribute::string_type>()(str);
  }
};

struct hash_visitor
{
  using result_type = std::size_t;

  template <class T> result_type operator()(T const &val) const
  {
    using getter = iter_getter<T, true>;
    typename getter::iterator_type it, beg, end;
    std::tie(beg, end) = getter()(val);

    std::size_t seed = 0;
    for(it = beg; it != end; ++it)
    {
      typename scalar_in<T>::type const &scalar = *it;
      std::size_t scalar_hash = hash_scalar()(scalar);
      boost::hash_combine(seed, scalar_hash);
    }
    return seed;
  }
};
}

std::size_t attribute::hash() const
{
  return boost::apply_visitor(hash_visitor(), *this);
  /*
    std::size_t dim = dimension();
    std::size_t seed = 0;
    for(std::size_t idx = 0; idx < dim; ++idx)
      boost::hash_combine(seed, get_string_val(idx));
    return seed;*/
}

END_NAMESPACE_CORE
