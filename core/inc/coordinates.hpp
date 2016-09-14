#ifndef CORE_COORDINATES_HPP
#define CORE_COORDINATES_HPP


#include <boost/functional/hash.hpp>
#include <boost/operators.hpp>

#include <array>
#include <cstddef>
#include <type_traits>
namespace simbad
{
namespace core
{

template <class scalar_type_, size_t dimension_>
struct coordinates
    : public std::array<scalar_type_, dimension_>,
      private boost::totally_ordered1<coordinates<scalar_type_, dimension_>>
{
  static constexpr size_t dimension = dimension_;
  using scalar_type = scalar_type_;

  using base_array = std::array<scalar_type_, dimension_>;


  static constexpr bool is_discrete = std::is_integral<scalar_type>();
  /*
   * Constructors
   */
  coordinates() = default;

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
};

struct coord_hasher
{

  template <class board_coord_type, size_t DIM>
  std::size_t
  operator()(coordinates<board_coord_type, DIM> const &v) const
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
    for (size_t i = 0; i < dimension; i++)
    {
      coord_scalar_type &c = coords[i];

      if (c == board_max[i])
      {
        c = board_min[i];
        return true;
      }
      else if (c == region_max[i])
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
    for (size_t i = 0; i < dimension; i++)
    {
      coord_scalar_type &c = coords[i];

      if (c == board_min[i])
      {
        c = board_max[i];
        return true;
      }
      else if (c == region_min[i])
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

template <class CV, class RL = CV>
class coord_incrementer_box
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
    for (size_t i = 0; i < dimension; i++)
    {
      coord_scalar_type &c = coords[i];

      if (c >= region_max[i])
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
    for (size_t i = 0; i < dimension; i++)
    {
      coord_scalar_type &c = coords[i];

      if (c <= region_min[i])
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
}
}
#endif
