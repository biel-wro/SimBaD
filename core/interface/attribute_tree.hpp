#ifndef ATTRIBUTE_TREE_HPP
#define ATTRIBUTE_TREE_HPP

#include "core_def.hpp"

#include "interface/attribute.hpp"

#include <boost/lexical_cast.hpp>
#include <boost/property_tree/ptree.hpp>

BEGIN_NAMESPACE_CORE
template <class external_type> struct attribute_translator;
END_NAMESPACE_CORE

namespace boost
{
namespace property_tree
{
template <class ExternalType>
struct translator_between<simbad::core::attribute, ExternalType>
{
  using type = simbad::core::attribute_translator<ExternalType>;
};
}
}

BEGIN_NAMESPACE_CORE
template <class external_type> class attribute_converting_visitor
{
  struct enabler
  {
  };

public:
  template <class stored_type>
  boost::optional<external_type>
  operator()(stored_type const &attr,
             typename std::enable_if<
                 std::is_convertible<stored_type, external_type>::value,
                 enabler>::type = enabler()) const
  {
    return static_cast<external_type const &>(attr);
  }
  template <class internal_type>
  boost::optional<external_type>
  operator()(internal_type const &attr,
             typename std::enable_if<
                 !std::is_convertible<internal_type, external_type>::value,
                 enabler>::type = enabler()) const
  {
    return boost::optional<external_type>();
  }
};
template <class ExternalType> struct attribute_translator
{
  using internal_type = simbad::core::attribute;
  using external_type = ExternalType;

  boost::optional<external_type> get_value(internal_type const &attr)
  {
    return boost::apply_visitor(
        [](auto const &val) {
          external_type ex_val = boost::lexical_cast<external_type>(val);
          return ex_val;
        },
        attr);
  }

  boost::optional<internal_type> put_value(std::string const &str)
  {
    return internal_type(str);
  }
  template <class T>
  boost::optional<internal_type> put_value(
      T v,
      typename std::enable_if<std::is_integral<T>::value>::type * = nullptr)
  {
    return internal_type(std::int64_t(v));
  }
  template <class T>
  boost::optional<internal_type>
  put_value(T v,
            typename std::enable_if<std::is_floating_point<T>::value>::type * =
                nullptr)
  {
    return internal_type(double(v));
  }
};

using attribute_tree =
    boost::property_tree::basic_ptree<std::string, attribute>;

END_NAMESPACE_CORE

#endif // ATTRIBUTE_TREE_HPP
