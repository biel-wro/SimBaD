#ifndef CORE_COORDINATES_HPP
#define CORE_COORDINATES_HPP

#include <boost/functional/hash.hpp>
#include <boost/operators.hpp>

#include <algorithm>
#include <array>
#include <cmath>
#include <cstddef>
#include <functional>
#include <iostream>
#include <numeric>
#include <type_traits>

#include "core_def.hpp"

BEGIN_NAMESPACE_CORE

// clang-format off
template <class scalar_type_, std::size_t dimension_>
struct coordinates:
    public std::array<scalar_type_, dimension_>,
    boost::totally_ordered1<coordinates<scalar_type_, dimension_>,
    boost::additive1<coordinates<scalar_type_, dimension_>,
    boost::field_operators2<coordinates<scalar_type_, dimension_>,scalar_type_
    >>>
// clang-format on
{
  using dimension_type = std::size_t;
  static constexpr dimension_type dimension = dimension_;
  using scalar_type = scalar_type_;
  using base_array = std::array<scalar_type_, dimension_>;
  static constexpr bool is_discrete = std::is_integral<scalar_type>::value;
  /*
   * Constructors
   */
  coordinates() = default;
  explicit coordinates(scalar_type fillval)
  {
    std::fill(this->begin(), this->end(), fillval);
  }

  coordinates(std::initializer_list<scalar_type> l)
  {
    std::copy(l.begin(), l.end(), this->begin());
  }

  coordinates(base_array const &arr) : base_array(arr) {}
  template <class coord_type, class tile_size_type>
  coordinates(std::array<coord_type, dimension> const &c,
              scalar_type const &tile_size)
  {
    compute<coord_type, tile_size_type, dimension>(c, tile_size);
  }

  template <class OtherScalar>
  coordinates(coordinates<OtherScalar, dimension> const &o)
  {
    std::transform(o.begin(), o.end(), this->begin(),
                   [](OtherScalar const &os) { return scalar_type(os); });
  }

  template <class coord_vector>
  static coordinates from_indexed(coord_vector const &vec)
  {
    coordinates c;
    for(size_t d = 0; d < dimension; ++d)
      c[d] = vec[d];
    return c;
  }
  template <class coord_vector>
  static coordinates from_callable(coord_vector const &vec)
  {
    coordinates c;
    for(size_t d = 0; d < dimension; ++d)
      c[d] = vec(d);
    return c;
  }

  /*
   * compute tile
   */
  template <class coord_type, class tile_size_type, size_t N>
  void inline compute(std::array<coord_type, dimension> const &coords,
                      tile_size_type tile_size,
                      std::integral_constant<int, N> * = nullptr) noexcept
  {
    compute<coord_type, tile_size_type, N - 1>();
    std::get<N - 1>(*this) = std::floor(std::get<N - 1>(coords) / tile_size);
  }

  template <class coord_type, class tile_size_type, size_t>
  void inline compute(std::array<coord_type, dimension> const &, tile_size_type,
                      std::integral_constant<int, 0> * = nullptr) noexcept
  {
  }

  template <class coord_type, class tile_size_type>
  static coordinates
  from_coordinates(std::array<coord_type, dimension> const &c,
                   tile_size_type const &tileSize) noexcept
  {
    return coordinates(c, tileSize);
  }

  /*
   * call operators
   */
  scalar_type const &operator()(size_t dim) const { return (*this)[dim]; }
  scalar_type &operator()(size_t dim) { return (*this)[dim]; }
  scalar_type const &operator[](size_t dim) const
  {
    assert(dim < dimension);
    return base_array::operator[](dim);
  }
  scalar_type &operator[](size_t dim)
  {
    assert(dim < dimension);
    return base_array::operator[](dim);
  }
  /*
   * comparison operators
   */
  bool operator==(coordinates const &o)
  {
    return static_cast<base_array const &>(*this) ==
           static_cast<base_array const &>(o);
  }
  bool operator<(coordinates const &o)
  {
    return static_cast<base_array const &>(*this) <
           static_cast<base_array const &>(o);
  }
  /*
   * additive operators
   */
  coordinates operator+() const { return *this; }
  coordinates operator-() const { return *this * scalar_type(-1); }
  coordinates operator+=(coordinates const &o)
  {
    for(size_t idx = 0; idx < dimension; ++idx)
      this->operator[](idx) += o[idx];

    return *this;
  }
  coordinates operator-=(coordinates const &o)
  {
    for(size_t idx = 0; idx < dimension; ++idx)
      this->operator[](idx) -= o[idx];

    return *this;
  }
  coordinates operator+=(scalar_type_ const &o)
  {
    std::for_each(this->begin(), this->end(), [&o](scalar_type &c) { c += o; });
    return *this;
  }
  coordinates operator-=(scalar_type_ const &o)
  {
    std::for_each(this->begin(), this->end(), [&o](scalar_type &c) { c -= o; });
    return *this;
  }
  coordinates operator*=(scalar_type_ const &o)
  {
    std::for_each(this->begin(), this->end(), [&o](scalar_type &c) { c *= o; });
    return *this;
  }
  coordinates operator/=(scalar_type_ const &o)
  {
    std::for_each(this->begin(), this->end(), [&o](scalar_type &c) { c /= o; });
    return *this;
  }

  /*
   * stream operators
   */
  friend std::ostream &operator<<(std::ostream &os, coordinates const &c)
  {
    if(dimension == 0)
      os << "[]";
    os << "[" << c[0];
    for(dimension_type d = 1; d < dimension; ++d)
      os << ", " << c[d];
    os << "]";
    return os;
  }

  friend std::istream &operator>>(std::istream &os, coordinates const &c)
  {
    throw "not implemented yet";
  }
  /*
   * norms and distance
   */
  static scalar_type distance_square(coordinates const &c1,
                                     coordinates const &c2)
  {
    return std::inner_product(c1.begin(), c1.end(), c2.begin(), scalar_type(0),
                              std::plus<scalar_type>(),
                              [](scalar_type const &s1, scalar_type const &s2) {
                                return (s1 - s2) * (s1 - s2);
                              });
  }

  static scalar_type distance(coordinates const &c1, coordinates const &c2)
  {
    return std::inner_product(c1.begin(), c1.end(), c2.begin(), scalar_type(0),
                              [](scalar_type const &c1, scalar_type const &c2) {
                                return std::hypot(c1, c2);
                              },
                              std::minus<scalar_type>());
  }
  scalar_type distance_square_to(coordinates const &o) const
  {
    return distance_square(*this, o);
  }
  scalar_type distance_to(coordinates const &o) const
  {
    return distance(*this, o);
  }
  scalar_type hypot() const
  {
    scalar_type (*hypot)(scalar_type, scalar_type) = std::hypot;
    return std::accumulate(this->begin(), this->end(), scalar_type(0), hypot);
  }
};
/*
 * std::min std::max
 */

/*
 *  Hash
 */
struct coord_hasher
{
  template <class board_coord_type, size_t DIM>
  std::size_t operator()(coordinates<board_coord_type, DIM> const &v) const
  {
    return boost::hash_range(v.begin(), v.end());
  }
};

template <class CV, class RL = CV, class BL = CV>
class coord_incrementer_wrapped
{
public:
  using coord_vector_type = CV;
  using region_limit_type = RL;
  using board_limit_type = BL;

  using coord_scalar_type = typename coord_vector_type::scalar_type;
  static constexpr size_t dimension = coord_vector_type::dimension;

  coord_incrementer_wrapped() {}
  coord_incrementer_wrapped(region_limit_type rmin, region_limit_type rmax,
                            board_limit_type bmin, region_limit_type bmax)
      : region_min(rmin), region_max(rmax), board_min(bmin), board_max(bmax)
  {
  }

  region_limit_type first() const { return region_min; }
  region_limit_type last() const { return region_max; }
  inline bool next(coord_vector_type &coords) noexcept
  {
    for(size_t i = 0; i < dimension; i++)
    {
      coord_scalar_type &c = coords[i];

      if(c == board_max[i])
      {
        c = board_min[i];
        return true;
      }
      else if(c == region_max[i])
        c = region_min[i];
      else
      {
        c++;
        return true;
      }
    }
    return false;
  }

  inline bool prev(coord_vector_type &coords) noexcept
  {
    for(size_t i = 0; i < dimension; i++)
    {
      coord_scalar_type &c = coords[i];

      if(c == board_min[i])
      {
        c = board_max[i];
        return true;
      }
      else if(c == region_min[i])
        c = region_max[i];
      else
      {
        c--;
        return true;
      }
    }
    return false;
  }

private:
  region_limit_type region_min, region_max;
  board_limit_type board_min, board_max;
};

template <class CV, class RL = CV> class coord_incrementer_box
{
public:
  using coord_vector_type = CV;
  using region_limit_type = RL;

  using coord_scalar_type = typename coord_vector_type::scalar_type;
  static constexpr size_t dimension = coord_vector_type::dimension;

  coord_incrementer_box() {}
  coord_incrementer_box(region_limit_type rmin, region_limit_type rmax)
      : region_min(rmin), region_max(rmax)
  {
  }

  region_limit_type first() const { return region_min; }
  region_limit_type last() const { return region_max; }
  inline bool next(coord_vector_type &coords) const noexcept
  {
    for(size_t i = 0; i < dimension; i++)
    {
      coord_scalar_type &c = coords[i];

      if(c >= region_max[i])
        c = region_min[i];
      else
      {
        c++;
        return true;
      }
    }
    return false;
  }
  inline bool prev(coord_vector_type &coords) const noexcept
  {
    for(size_t i = 0; i < dimension; i++)
    {
      coord_scalar_type &c = coords[i];

      if(c <= region_min[i])
        c = region_max[i];
      else
      {
        c--;
        return true;
      }
    }
    return false;
  }

private:
  region_limit_type region_min, region_max;
};

END_NAMESPACE_CORE

namespace boost
{
template <class Scalar, size_t dimension>
struct hash<simbad::core::coordinates<Scalar, dimension>>
{
  using result_type = std::size_t;
  using arg_type = simbad::core::coordinates<Scalar, dimension>;
  std::size_t operator()(arg_type const &c) const
  {
    return boost::hash_range(c.begin(), c.end());
  }
};
}

namespace std
{
template <class Scalar, size_t dimension>
struct hash<simbad::core::coordinates<Scalar, dimension>>
{
  using result_type = std::size_t;
  using arg_type = simbad::core::coordinates<Scalar, dimension>;
  std::size_t operator()(arg_type const &c) const
  {
    return boost::hash_range(c.begin(), c.end());
  }
};
}

#endif
