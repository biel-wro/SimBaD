#ifndef BOARD_IMPL_H
#define BOARD_IMPL_H

#include <array>

#include <boost/intrusive/unordered_set.hpp>
#include <boost/operators.hpp>
#include <boost/functional/hash.hpp>
#include <cmath>
#include <initializer_list>
#include "lazy_set.h"


namespace simbad{
namespace core{

template< class board_coord_type, class integral_dim >
struct board_coordinates :
    public std::array<board_coord_type, integral_dim::value>,
    private boost::totally_ordered1<
        board_coordinates<board_coord_type, integral_dim>
        >
{

    constexpr static const size_t dimension = integral_dim::value;
    typedef std::array<board_coord_type, integral_dim::value> base_array;

    board_coordinates() = default;

    board_coordinates( std::initializer_list<board_coord_type> l){
      std::copy(l.begin(),l.end(),this->begin() );
    }

    board_coordinates( base_array const &arr ) :
      base_array(arr)
    {}


    //using base_array::base_array;

    template< class coord_type, class tile_size_type>
    board_coordinates( std::array<coord_type,dimension> const &c, board_coord_type const &tile_size ){
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
    static board_coordinates from_coordinates(
            std::array<coord_type,dimension> const &c,
            tile_size_type const &tileSize  )
    noexcept{
        return board_coordinates( c, tileSize );
    }

  bool operator==(board_coordinates const &o ){
    return static_cast<base_array const&>(*this)
        == static_cast<base_array const&>(o);
  }
  bool operator<(board_coordinates const &o ){
    return static_cast<base_array const&>(*this)
        < static_cast<base_array const&>(o);
  }

    static bool next (
        board_coordinates &what,
        board_coordinates const &region_min,
        board_coordinates const &region_max,
        board_coordinates const &board_min,
        board_coordinates const &board_max)
    noexcept{
        for( size_t i=0; i<dimension; i++ )
        {
            board_coord_type &c = what[i];

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
        board_coordinates &what,
        board_coordinates const &region_min,
        board_coordinates const &region_max,
        board_coordinates const &board_min,
        board_coordinates const &board_max)
    noexcept{
        for( size_t i=0; i<dimension; i++ )
        {
            board_coord_type &c = what[i];

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

struct coord_hash{

template<class board_coord_type, class dim>
std::size_t operator()( board_coordinates<board_coord_type,dim> const &v )
const{
    typedef std::array<board_coord_type, dim::value> base_array;
    boost::hash<base_array> hasher;
    return hasher( static_cast<base_array const&>(v) );
}
};


/*
 * Board Tile contains particles which belongs to some cube in space
 */

typedef boost::intrusive::unordered_set_base_hook<
  /*boost::intrusive::store_hash<true>*/ >          tile_hook;

template<class T, class config>
class board_tile :
    public LazySet<T,config::int_chunk_size::value>,
    public tile_hook,
    boost::totally_ordered<board_tile<T,config>>
{
public:
  typedef typename config::board_coord           board_coord_type;
  typedef typename config::int_dimension            int_dimension;

  typedef board_coordinates<
    board_coord_type,int_dimension>               coordinates_type;
private:
    coordinates_type coords;
public:
  /*
   * constructors, assignments, destructor
   */
  board_tile() = delete;
  board_tile( const coordinates_type& board_coords):
    coords(board_coords)
  {}
  board_tile( const board_tile & ) = delete;
  board_tile( board_tile && ) = default;
  board_tile &operator=( const board_tile & ) = delete;
  board_tile &operator=( board_tile && ) = default;
  ~board_tile(){}

  /*
   * operations on coordinates
   */
  coordinates_type const &get_coords() const{
    return coords;
  }

};

struct same_coords{

  template<class T, class conf>
  bool operator()( board_tile<T,conf> const &o1, board_tile<T,conf> const &o2 )
  const{
   return o1.get_coords() == o2.get_coords();
  }

  template<class T, class conf>
  bool operator()( board_tile<T,conf> const &b,
                   typename board_tile<T,conf>::coordinates_type const &c)
  const{
    return b.get_coords() == c;
  }

  template<class T, class conf>
  bool operator()( typename board_tile<T,conf>::coordinates_type const &c,
                   board_tile<T,conf> const &b)
  const{
    return c == b.get_coords();
  }

};

struct tile_hash{
template<class T, class conf>
  std::size_t operator()( board_tile<T,conf> const &v )
  const{
    coord_hash h;
    return h(v.get_coords() );
  }
};
/*
 * Implementation of the iterator over elements in some region of the board
 */
template< class D_, class itercfg >
class board_region_iterator : public boost::iterator_facade<
        board_region_iterator<D_,itercfg>,            // itself
        D_,                                            // value
        boost::bidirectional_traversal_tag             // travelsal type
        >
{
  typedef typename std::is_const<D_>::type            is_const;

  typedef typename std::conditional<is_const::value,
    typename itercfg::board_type const,
    typename itercfg::board_type>::type               board_type;

  typedef typename std::conditional<is_const::value,
    typename board_type::tile_type const,
    typename board_type::tile_type>::type             tile_type;

  typedef typename std::conditional<is_const::value,
    typename tile_type::const_iterator,
    typename tile_type::iterator>::type               tile_iter;

  typedef typename tile_type::board_coord             board_coord;
  typedef typename itercfg::region_coord              region_coord;
  typedef typename itercfg::limit_coord               limit_coord;
    struct enabler{}; // for std::enable_if
 public:
    board_region_iterator():
      p_board(nullptr)
    {}

    board_region_iterator( board_type &board, bool begin_not_end,
        region_coord rmin, region_coord rmax,
        limit_coord bmin=limit_coord(), limit_coord bmax=limit_coord() ):
      p_board( &board ),
      rmin( std::move(rmin) ),
      rmax( std::move(rmax) ),
      bmin( std::move(bmin) ),
      bmax( std::move(bmax) ),
      valid( false )
    {
      if(begin_not_end)
        increment();
      else
        decrement();
    }

    template< class OtherD_>
    board_region_iterator( board_region_iterator<OtherD_,itercfg > const &o,
      typename std::enable_if<std::is_convertible<OtherD_*,D_*>::value,
      enabler >::type = enabler() ) :
      p_board(o.p_board), it(o.it), p_tile(o.p_tile),
      rmax(o.rmax),rmin(o.rmin),bmax(o.bmax), bmin(o.bmin), valid(o.valid)
    {}

 protected:
    friend class boost::iterator_core_access;
    template<class,class> friend class board_region_iterator;

    template< class OtherD_>
    inline bool equal(board_region_iterator<OtherD_,itercfg> const& o) const
    {
      if( !valid )
        return !o.valid;
      else
        return it == o.it && p_tile == o.p_tile;
    }

    inline void increment()
    {
      if( valid ) // including into the while condition complicates too much
        ++it;

      while( !valid || p_tile->end() == it )   // if done iterating over current tile
      {
        board_coord c = p_tile->get_coords();

        valid = increment_to_valid(c);

        if( !valid )
          return;                       //iterator is now end()
        else
        {
          p_tile = p_board->get_tile(c);   // iterator goes to next tile
          it = p_tile.begin();
        }
      }
    }
    inline void decrement()    {
      while( !valid || p_tile->begin() == it )
      {
        board_coord c = p_tile->get_coords();
        valid = decrement_to_valid(c);
        if( !valid )
          return;
        else{
          p_tile = &p_board.get_tile(c);
          it = p_tile.end();
        }
      }
      --it;
    }
    inline D_ & dereference() const{
      return *it;
    }

private:
    inline bool increment_to_valid( board_coord &c){
      while( true )
      {
        bool has_next = board_coord::next(c,rmin,rmax,bmin,bmax);
        if( has_next )
          return true;
        else if( p_board->tile_exists(c) )
          return false;
      }
    }

    inline bool decrement_to_valid( board_coord &c){
      while( true )
      {
        bool has_prev = board_coord::prev(c,rmin,rmax,bmin,bmax);
        if( !has_prev )
          return false;
        else if( p_board->tile_exists(c) )
          return true;
      }
    }

  board_type *p_board;
  tile_iter it;
  tile_type *p_tile;
  board_coord rmax;
  board_coord rmin;
  board_coord bmax;
  board_coord bmin;
  bool valid;

};
/*
 * Iterator over ALL the elements in the boards
 */

template< class D_, class itercfg >
class board_iterator : public boost::iterator_facade<
        board_iterator<D_,itercfg>,            // itself
        D_,                                            // value
        boost::forward_traversal_tag             // travelsal type
        >
{
  typedef typename std::is_const<D_>::type            is_const;

  typedef typename std::conditional<is_const::value,
    typename itercfg::board_type const,
    typename itercfg::board_type>::type               board_type;

  typedef typename std::conditional<is_const::value,
    typename board_type::set_type const,
    typename board_type::set_type>::type              set_type;

  typedef typename std::conditional<is_const::value,
    typename board_type::tile_type const,
    typename board_type::tile_type>::type             tile_type;

  typedef typename std::conditional<is_const::value,
    typename set_type::const_iterator,
    typename set_type::iterator>::type                outer_iter;

  typedef typename std::conditional<is_const::value,
    typename tile_type::const_iterator,
    typename tile_type::iterator>::type               inner_iter;

    struct enabler{}; // for std::enable_if
 public:
    explicit board_iterator(bool valid=false):
      p_set(nullptr),
      oit(),
      iit(),
      valid(valid)
    {}
    explicit board_iterator(set_type &s):
      p_set(&s),
      oit(s.begin()),
      iit(oit->begin()),
      valid(true)
    {
      while(iit == oit->end())
      {
        ++oit;
        if( p_set->end() == oit)
        {
          valid = false;
          return;
        }
        iit = oit->begin();
      }
    }


    template< class OtherD_>
    board_iterator( board_iterator<OtherD_,itercfg > const &o,
      typename std::enable_if<std::is_convertible<OtherD_*,D_*>::value,
      enabler >::type = enabler() ) :
      p_set(o.p_set),
      oit(o.oit),
      iit(o.iit),
      valid(o.valid)
    {}

    board_iterator( set_type &s, outer_iter o, inner_iter i, bool v = true ):
      p_set(&s),
      oit(o),
      iit(i),
      valid(v)
    {}

    bool is_valid() const{
      return valid;
    }

    outer_iter get_outer() const{
      return oit;
    }

    inner_iter get_inner() const{
      return iit;
    }
 protected:
    friend class boost::iterator_core_access;
    template<class,class> friend class board_iterator;

    template< class OtherD_>
    inline bool equal(board_iterator<OtherD_,itercfg> const& o) const
    {
      if( !valid )
        return !o.valid;
      else
        return iit == o.iit
            && oit == o.oit
            && p_set == o.p_set;
    }

    inline void increment()
    {
      ++iit;
      while( oit->end() == iit )
      {
        ++oit;
        if( p_set->end() == oit )
        {
          valid = false;
          return;
        }
        else
          iit = oit->begin();
      }
    }

    inline D_ & dereference() const{
      return *iit;
    }

private:

  set_type *p_set;
  outer_iter oit;
  inner_iter iit;
  bool valid;
};


struct tile_disposer{
  template< class tile> void operator()(tile *ptr){ delete ptr; }
};

/*
 * Implementation of the main template class
 */
template<class T, class config>
class board_impl{
public:
  typedef T                                               value_type;
  typedef T&                                              reference_type;
  typedef const T&                                        const_refernce_type;
  typedef board_tile<T,config>                            tile_type;
  typedef typename tile_type::coordinates_type            coordinates_type;
  typedef boost::intrusive::unordered_set<
    tile_type,
    boost::intrusive::equal< same_coords >,
    boost::intrusive::hash< tile_hash >
  >                                                       set_type;
  typedef typename set_type::bucket_type                  bucket_type;
  typedef typename set_type::bucket_traits                bucket_traits;
  //typedef typename config::int_dimension                  integral_dim;
  static constexpr const int DEFAULT_BUCKET_COUNT =       1024;

  struct itercfg{
    typedef board_impl<T,config> board_type;
  };

  struct linreg_itercfg{
    typedef board_impl<T,config> board_type;
    typedef coordinates_type board_coords;
  };

  typedef board_iterator<T, itercfg>                  iterator;
  typedef board_iterator<T const, itercfg>            const_iterator;

  //typedef board_region_iterator<T, iter_config>           region_iterator;
  //typedef board_region_iterator<T const, iter_config>     const_region_iterator;

  typedef typename set_type::iterator                     outer_iterator;
  typedef typename set_type::const_iterator               const_outer_iterator;
  typedef typename tile_type::iterator                    inner_iterator;
  typedef typename tile_type::const_iterator              const_inner_iterator;
  typedef size_t                                          size_type;
private:
  std::unique_ptr<bucket_type[]>      buckets;
  set_type                            tile_set;
  size_type                           count;
public:

  board_impl(size_t bucket_count=DEFAULT_BUCKET_COUNT):
    buckets( new bucket_type[bucket_count] ),
    tile_set( bucket_traits( buckets.get(), bucket_count) )
  {}
  board_impl( board_impl const & ) = delete;
  board_impl( board_impl && ) = default;
  board_impl &operator=( board_impl const &) = delete;
  board_impl &operator=( board_impl && ) = default;
  ~board_impl(){
    tile_set.clear_and_dispose( std::default_delete<tile_type>() );
  }

  void rehash(size_t new_count){
    bucket_type *new_buckets = new bucket_type[new_count];
    tile_set.rehash( bucket_traits(new_buckets, new_count));
    buckets.reset(new_buckets);
  }
  template<class ...Args>
  iterator emplace(coordinates_type const &cs, Args... args){
    outer_iterator oit = find_tile_or_create(cs);
    inner_iterator iit = oit->emplace_back(std::forward<Args>(args)...);
    iterator it(tile_set,oit,iit);
    ++count;
    return it;
  }

  void remove( coordinates_type const &c, T &v ){
     outer_iterator oit = find_tile_or_die( c );
     assert( oit->end() != oit->find(v) );
     oit->swap_and_delete( v );
     --count;
  }

  size_type size() const{
    return count;
  }

  iterator iterator_to( coordinates_type const &cs, T const &v ){
    outer_iterator oit = find_tile_or_die(cs);
    inner_iterator iit = oit->find( v );
    return iterator( tile_set, oit, iit );
  }

  const_iterator iterator_to( coordinates_type const &cs, T const &v ) const{
    const_outer_iterator oit = find_tile_or_die(cs);
    const_inner_iterator iit = oit->find( v );
    return const_iterator( tile_set, oit, iit );
  }

  iterator begin(){
    return iterator(tile_set);
  }

  const_iterator begin() const{
    return const_iterator(tile_set);
  }

  iterator end(){
    return iterator(false);
  }

  const_iterator end() const{
    return const_iterator(false);
  }
  /*
  region_iterator begin_region(){
    return region_iterator();
  }
*/


protected:
  outer_iterator find_tile_or_die( coordinates_type const &cs ){
    return tile_set.find( cs, coord_hash(), same_coords() );
  }
  const_outer_iterator find_tile_or_die( coordinates_type const &cs ) const{
    return tile_set.find( cs, coord_hash(), same_coords() );
  }

  outer_iterator find_tile_or_create( coordinates_type const &cs ){
    typename set_type::insert_commit_data cdata;
    std::pair<outer_iterator,bool> result;
    result = tile_set.insert_check( cs, coord_hash(), same_coords(), cdata );
    if( result.second )
    {
      tile_type *p_tile = new tile_type(cs);
      result.first = tile_set.insert_commit( *p_tile, cdata );
    }
    return result.first;
  }

};




}}
#endif // TILE_IMPL_H
