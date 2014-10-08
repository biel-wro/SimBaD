#ifndef TILE_IMPL_H
#define TILE_IMPL_H

#include <array>

#include <boost/intrusive/unordered_set.hpp>
#include <boost/operators.hpp>
#include <boost/functional/hash.hpp>
#include <cmath>

#include "lazy_set.h"

namespace simbad{
namespace core{



template< class tile_coord_type, class integral_dim >
struct tile_coordinates : public std::array<tile_coord_type, integral_dim::value> {

    constexpr static const size_t dimension = integral_dim::value;

    tile_coordinates() = default;


    template< class coord_type, class tile_size_type>
    tile_coordinates( std::array<coord_type,dimension> const &c, tile_coord_type const &tile_size ){
        compute<coord_type,tile_size_type,dimension>(c,tile_size);
    }


    template<class coord_type, class tile_size_type, int N=dimension>
    void inline compute(
            std::array<coord_type,dimension> const &coords,
            tile_size_type tile_size, std::integral_constant<int,N>* = nullptr)
            noexcept{
        compute<coord_type,tile_size_type,N-1>();
        std::get<N-1>(*this) = std::floor( std::get<N-1>(coords) / tile_size );
    }

    template<class coord_type, class tile_size_type, int >
    void inline compute(
            std::array<coord_type,dimension> const &,
            tile_size_type, std::integral_constant<int,0>* = nullptr)
            noexcept
    {}

    template< class coord_type, class tile_size_type>
    static tile_coordinates from_coordinates( std::array<coord_type,dimension> const &c, tile_size_type const &tileSize  ){
        return tile_coordinates( c, tileSize );
    }


    static bool next(
        tile_coordinates &what,
        tile_coordinates const &region_min,
        tile_coordinates const &region_max,
        tile_coordinates const &board_min,
        tile_coordinates const &board_max)
    {

        for( size_t i=0; i<dimension; i++ )
        {
            tile_coord_type &c = what[i];

            if( c == board_max[i] ){
                c = board_min[i];
                return true;
            }
            else if( c == region_max[i] )
                c = region_min[i];
            else
            {
                c++;
                return true;
            }
        }
        return false;

    }
    static bool prev(
        tile_coordinates &what,
        tile_coordinates const &region_min,
        tile_coordinates const &region_max,
        tile_coordinates const &board_min,
        tile_coordinates const &board_max)
    {

        for( size_t i=0; i<dimension; i++ )
        {
            tile_coord_type &c = what[i];

            if( c == board_min[i] ){
                c = board_max[i];
                return true;
            }
            else if( c == region_min[i] )
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

template<class tile_coord_type, class integral_dim>
std::size_t hash_value( tile_coordinates<tile_coord_type,integral_dim> const &v ){
    typedef std::array<tile_coord_type, integral_dim::value> base_type;

    boost::hash<base_type> hasher;
    return hasher( static_cast<base_type const&>(v) );
}



//template<class D,class int_dim,class chunk_size>
//struct Tile : public
//        LazySet<D,chunk_size>,
////        boost::totally_ordered1< Tile<D,int_dim,chunk_size>,
//        boost::intrusive::unordered_set_base_hook<>
////        >
//{
//public:
//    // types
//    typedef LazySet<D,chunk_size> set_type;
//    typedef tile_coords tile_coords_type;
//    typedef Tile<D,tile_coords> type;
//    typedef typename set_type::iterator iterator;
//    typedef typename set_type::const_iterator const_iterator;
//    typedef typename set_type::size_type size_type;

//    struct SameCoords{
//        bool operator()(type const &tile, tile_coords const &tc ) const{
//            return tile.coords==tc;
//        }
//        bool operator()(tile_coords const &tc, type const &tile ) const{
//            return tile.coords==tc;
//        }
//    };
//    friend class SameCoords;
//    // constructors
//    Tile() = default;
//    Tile(tile_coords const &tc) : coords(tc) {}

//    // member functions
//    template<class ...Args>
//    D &emplace_back(Args &&...args){

//        return set.emplace_back(std::forward<Args>(args)...);
//    }

//    D &back(){
//        return set.back();
//    }
//    D const &back() const{
//        return set.back();
//    }

//    void popBack(){
//        set.pop_back();;
//    }

//    void backSwapAndPop(D &val){
//        set.backSwapAndPop(val);
//    }

//    iterator begin(){
//        return set.begin();
//    }
//    const_iterator begin() const{
//        return set.begin();
//    }
//    iterator end(){
//        return set.end();
//    }
//    const_iterator end() const{
//        return set.end();
//    }

//    iterator find( D const &d ){
//        return set.find(d);
//    }
//    const_iterator find( D const &d ) const{
//        return set.find(d);
//    }

//    size_type size() const{
//        return set.size();
//    }

//    bool operator==(Tile const &o) const{
//        return coords == o.coords;
//    }

//    bool operator<(Tile const &o) const{
//        return coords < o.coords;
//    }

//    tile_coords &getCoords(){
//        return coords;
//    }
//    tile_coords const &getCoords() const{
//        return coords;
//    }

//    bool checkConsistenty() const{
//        return set.checkConsistency();
//    }
//private:
//    tile_coords coords;
//    set_type set;
//};

//template<class D, class coord_type>
//std::size_t hash_value( Tile<D,coord_type> const &t ){
//    boost::hash<coord_type> hasher;
//    size_t h = hasher(t.getCoords() );
//    return h;
//}


}}
#endif // TILE_IMPL_H
