#ifndef ATTRIBUTE_CONVERTER_HPP
#define ATTRIBUTE_CONVERTER_HPP

#include "core_fwd.hpp"

#include <boost/lexical_cast.hpp>
#include <boost/optional.hpp>

#include <cstddef>
#include <string>
#include <vector>

BEGIN_NAMESPACE_CORE
// clang-format off
template<class> struct is_attribute_array : std::false_type {};
template<class T>
struct is_attribute_array<array_attribute<T>> : std::true_type {};
template<class T, class A>
struct is_attribute_array<std::vector<T,A>> : std::true_type {};

template<class> struct attribute_array_element { using element_type = void; };
template<class T>
struct attribute_array_element<array_attribute<T>> { using element_type = T; };
template<class T, class A>
struct attribute_array_element<std::vector<T,A>>{ using element_type = T; };

template<class> struct is_coordinates : std::false_type {};
template<class Scalar, std::size_t DIM>
struct is_coordinates<coordinates<Scalar,DIM>> : std::true_type {};
template<class>
struct coordinates_element {
  using element_type = void;
  constexpr static std::size_t dimension = 0;
};
template<class Scalar, std::size_t DIM>
struct coordinates_element<coordinates<Scalar,DIM>>
{
  using element_type = Scalar;
  constexpr static std::size_t dimension = DIM;
};

template <class T> struct is_scalar_attribute
{
  constexpr static bool value =
      !is_attribute_array<T>::value && !is_coordinates<T>::value;
};
// clang-format on

template <class Target> class scalar_attribute_converter
{
  /* clang-format off */ struct enabler{}; /* clang-format on */
public:
  // string to everything
  boost::optional<Target> operator()(std::string const &str)
  {
    try
    {
      return boost::lexical_cast<Target>(str);
    }
    catch(std::exception &)
    {
      return boost::optional<Target>();
    }
  }

  // artihmetic to string
  template <class Internal>
  boost::optional<Target>
  operator()(Internal const &v,
             typename std::enable_if<std::is_same<Target, std::string>::value &&
                                         !is_attribute_array<Internal>::value,
                                     enabler>::type = enabler())
  {
    try
    {
      return boost::lexical_cast<Target>(v);
    }
    catch(std::exception &)
    {
      return boost::optional<Target>();
    }
  }

  // artithmetic to arithmetic
  template <class Internal>
  boost::optional<Target>
  operator()(Internal val,
             typename std::enable_if<std::is_arithmetic<Target>::value &&
                                         std::is_arithmetic<Internal>::value,
                                     enabler>::type = enabler())
  {
    try
    {
      return boost::numeric_cast<Target>(val);
    }
    catch(std::exception &)
    {
      return boost::optional<Target>();
    }
  }
};

class attribute_converter
{
  template <class T> using optional = boost::optional<T>;
  /* clang-format off */ struct enabler{}; /* clang-format on */
public:
  // scalar to scalar
  template <class Target, class Internal>
  static optional<Target>
  convert_to(Internal val,
             typename std::enable_if<is_scalar_attribute<Internal>::value &&
                                         is_scalar_attribute<Target>::value,
                                     enabler>::type = enabler())
  {
    return scalar_attribute_converter<Target>()(val);
  }

  // scalar to coords => allowed only for 1D coords
  template <class Target, class Internal>
  static optional<Target>
  convert_to(Internal arr,
             typename std::enable_if<is_scalar_attribute<Internal>::value &&
                                         is_coordinates<Target>::value,
                                     enabler>::type = enabler())
  {
    using output_element_type =
        typename coordinates_element<Target>::element_type;
    if(1 != coordinates_element<Target>::dimension)
      return optional<Target>();
    scalar_attribute_converter<output_element_type> converter;
    optional<output_element_type> opt_res = converter(arr);
    if(opt_res)
      return Target{opt_res.get()};
    return optional<Target>();
  }
  // scalar to array
  template <class Target, class Internal>
  static optional<Target>
  convert_to(Internal val,
             typename std::enable_if<is_scalar_attribute<Internal>::value &&
                                         is_attribute_array<Target>::value,
                                     enabler>::type = enabler())
  {
    using element_type = typename attribute_array_element<Target>::element_type;
    boost::optional<element_type> opt_elem =
        scalar_attribute_converter<element_type>()(val);

    if(!opt_elem)
      return boost::none;

    return Target{opt_elem.get()};
  }

  // coords to scalar => allowed only for 1d coords
  template <class Target, class Internal>
  static optional<Target>
  convert_to(Internal val,
             typename std::enable_if<is_coordinates<Internal>::value &&
                                         is_scalar_attribute<Target>::value,
                                     enabler>::type = enabler())
  {
    //    using input_element_type =
    //        typename coordinates_element<Internal>::element_type;

    if(1 != coordinates_element<Internal>::dimension)
      return optional<Target>();
    scalar_attribute_converter<Target> converter;
    optional<Target> opt = converter(val[0]);
    return opt;
  }

  // coords to coords => works if sizes are the same
  template <class Target, class Internal>
  static optional<Target>
  convert_to(Internal src,
             typename std::enable_if<is_coordinates<Internal>::value &&
                                         is_coordinates<Target>::value,
                                     enabler>::type = enabler())
  {
    using internal_coord_elements = coordinates_element<Internal>;
    using target_coord_elements = coordinates_element<Target>;

    if(internal_coord_elements::dimension != target_coord_elements::dimension)
      return optional<Target>();

    using target_element_type = typename target_coord_elements::element_type;

    Target tgt;
    for(std::size_t d = 0; d < internal_coord_elements::dimension; ++d)
    {
      scalar_attribute_converter<target_element_type> scalar_converter;
      optional<target_element_type> opt_el = scalar_converter(src[d]);
      if(!opt_el)
        return optional<Target>();
      tgt[d] = opt_el.get();
    }
    return tgt;
  }

  // coords to array
  template <class Target, class Internal>
  static optional<Target>
  convert_to(Internal const &coords,
             typename std::enable_if<is_coordinates<Internal>::value &&
                                         is_attribute_array<Target>::value,
                                     enabler>::type = enabler())
  {
    using target_coord_elements = coordinates_element<Target>;
    using target_element_type = typename target_coord_elements::element_type;
    constexpr std::size_t dimension = target_coord_elements::dimension;

    optional<Target> ret;
    Target tgt(dimension);
    for(std::size_t d = 0; d < dimension; ++d)
    {
      scalar_attribute_converter<Target> converter;
      optional<target_element_type> opt_el = converter(coords[d]);
      if(!opt_el)
        return ret;
      tgt[d] = opt_el.get();
    }
    ret.emplace(std::move(tgt));
    return ret;
  }

  // array to scalar
  template <class Target, class Internal>
  static optional<Target>
  convert_to(Internal const &arr,
             typename std::enable_if<is_attribute_array<Internal>::value &&
                                         is_scalar_attribute<Target>::value,
                                     enabler>::type = enabler())
  {
    if(1 != arr.size())
      return boost::optional<Target>();
    return scalar_attribute_converter<Target>()(arr[0]);
  }
  // array to coords
  template <class Target, class Internal>
  static optional<Target>
  convert_to(Internal const &arr,
             typename std::enable_if<is_attribute_array<Internal>::value &&
                                         is_coordinates<Target>::value,
                                     enabler>::type = enabler())
  {
    using output_element_type =
        typename coordinates_element<Target>::element_type;
    constexpr std::size_t size = coordinates_element<Target>::dimension;
    optional<Target> ret;

    if(size != arr.size())
      return ret;

    Target tgt;
    scalar_attribute_converter<output_element_type> converter;
    for(std::size_t d = 0; d < size; ++d)
    {
      optional<output_element_type> opt_elem = converter(arr[d]);
      if(!opt_elem)
        return ret;
      output_element_type elem = std::move(opt_elem.get());
      tgt[d] = (std::move(elem));
    }

    return tgt;
  }

  // array to array
  template <class Target, class Internal>
  static optional<Target>
  convert_to(Internal const &arr,
             typename std::enable_if<is_attribute_array<Internal>::value &&
                                         is_attribute_array<Target>::value,
                                     enabler>::type = enabler())
  {
    using input_element_type =
        typename attribute_array_element<Internal>::element_type;
    using output_element_type =
        typename attribute_array_element<Target>::element_type;

    Target tgt;
    tgt.reserve(arr.size());
    scalar_attribute_converter<output_element_type> converter;
    for(input_element_type const &inp_elem : arr)
    {
      optional<output_element_type> opt_out_elem = converter(inp_elem);
      if(!opt_out_elem)
        return boost::none;
      output_element_type out_elem = std::move(opt_out_elem.get());
      tgt.emplace_back(std::move(out_elem));
    }
    return tgt;
  }
};

namespace
{
template <class Target> struct convert_visitor
{
  using result_type = boost::optional<Target>;
  template <class Source>
  boost::optional<Target> operator()(Source const &val) const
  {
    return attribute_converter::convert_to<Target, Source>(val);
  }
};

}

template <class Target> Target attribute_cast(attribute const &attr)
{
  boost::optional<Target> opt =
      boost::apply_visitor(convert_visitor<Target>(), attr);
  if(opt)
    return opt.get();

  struct : public std::bad_cast
  {
    const char *what() const noexcept override { return "bad attribute cast"; }
  } ex;

  throw ex;
}

END_NAMESPACE_CORE
#endif // ATTRIBUTE_CONVERTER_HPP
