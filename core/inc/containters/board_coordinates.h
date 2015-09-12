#ifndef BOARD_COORDINATES_H
#define BOARD_COORDINATES_H

#include <boost/operators.hpp>
#include <boost/functional/hash.hpp>

#include <cstddef>


namespace simbad{
namespace core{


template <class board_coord_type, size_t DIM>
struct board_coordinates
    : public std::array<board_coord_type, DIM>,
      private boost::totally_ordered1<board_coordinates<board_coord_type, DIM>>
{

    using base_array = std::array<board_coord_type, DIM>;
    // using base_array::base_array;

    /*
     * Constructors
     */
    board_coordinates() = default;

    board_coordinates(std::initializer_list<board_coord_type> l)
    {
        std::copy(l.begin(), l.end(), this->begin());
    }

    board_coordinates(base_array const &arr) : base_array(arr) {}

    template <class coord_type, class tile_size_type>
    board_coordinates(std::array<coord_type, DIM> const &c,
                      board_coord_type const &tile_size)
    {
        compute<coord_type, tile_size_type, DIM>(c, tile_size);
    }

    /*
     * compute tile
     */
    template <class coord_type, class tile_size_type, size_t N>
    void inline compute(std::array<coord_type, DIM> const &coords,
                        tile_size_type tile_size,
                        std::integral_constant<int, N> * = nullptr) noexcept
    {
        compute<coord_type, tile_size_type, N - 1>();
        std::get<N - 1>(*this) =
            std::floor(std::get<N - 1>(coords) / tile_size);
    }

    template <class coord_type, class tile_size_type, size_t>
    void inline compute(std::array<coord_type, DIM> const &,
                        tile_size_type,
                        std::integral_constant<int, 0> * = nullptr) noexcept
    {
    }

    template <class coord_type, class tile_size_type>
    static board_coordinates
    from_coordinates(std::array<coord_type, DIM> const &c,
                     tile_size_type const &tileSize) noexcept
    {
        return board_coordinates(c, tileSize);
    }

    /*
     * comparison operators
     */
    bool operator==(board_coordinates const &o)
    {
        return static_cast<base_array const &>(*this) ==
               static_cast<base_array const &>(o);
    }
    bool operator<(board_coordinates const &o)
    {
        return static_cast<base_array const &>(*this) <
               static_cast<base_array const &>(o);
    }

    static bool next(board_coordinates &what,
                     board_coordinates const &region_min,
                     board_coordinates const &region_max,
                     board_coordinates const &board_min,
                     board_coordinates const &board_max) noexcept
    {
        for (size_t i = 0; i < DIM; i++)
        {
            board_coord_type &c = what[i];

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

    static bool prev(board_coordinates &what,
                     board_coordinates const &region_min,
                     board_coordinates const &region_max,
                     board_coordinates const &board_min,
                     board_coordinates const &board_max) noexcept
    {
        for (size_t i = 0; i < DIM; i++)
        {
            board_coord_type &c = what[i];

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
};

struct coord_hasher
{

    template <class board_coord_type, size_t DIM>
    std::size_t
    operator()(board_coordinates<board_coord_type, DIM> const &v) const
    {
        typedef std::array<board_coord_type, DIM> base_array;
        boost::hash<base_array> hasher;
        return hasher(static_cast<base_array const &>(v));
    }
};

}}
#endif // BOARD_COORDINATES_H
