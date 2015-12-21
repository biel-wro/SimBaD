#ifndef CONTAINERS_BOARD
#define CONTAINERS_BOARD

#include "board_impl.hpp"

namespace simbad
{
namespace core
{

template <class coord, int dim, int chunk_sz> struct board_config
{
    typedef coord board_coord;
    typedef std::integral_constant<int, dim> int_dimension;
    typedef std::integral_constant<int, chunk_sz> int_chunk_size;
};

template <class T, int dim, class coord = long, int chunk_sz = 1000> class board
{
  public:
    typedef board_config<coord, dim, chunk_sz> config;
    typedef board_impl<T, config> implementation;
    typedef typename implementation::coordinates_type coordinates_type;
    typedef typename implementation::iterator iterator;
    typedef typename implementation::const_iterator const_iterator;

    using size_type = typename implementation::size_type;

    using box_iterator = typename implementation::box_iterator;
    using const_box_iterator = typename implementation::const_box_iterator;

    using box_region = typename implementation::box_region;
    using const_box_region = typename implementation::const_box_region;

  protected:
    implementation impl;

  public:
    void rehash(size_t new_size) { impl.rehash(new_size); }

    iterator begin() { return impl.begin(); }

    iterator end() { return impl.end(); }

    const_iterator begin() const { return impl.begin(); }

    const_iterator end() const { return impl.end(); }

    box_region get_box(coordinates_type min, coordinates_type max)
    {
        return impl.get_box(min, max);
    }

    const_box_region get_box(coordinates_type min, coordinates_type max) const
    {
        return impl.get_box(min, max);
    }

    iterator iterator_to(coordinates_type const &cs, T const &v)
    {
        return impl.iterator_to(cs, v);
    }

    const_iterator iterator_to(coordinates_type const &cs, T const &v) const
    {
        return impl.iterator_to(cs, v);
    }

    template <class... Args>
    iterator emplace(coordinates_type const &c, Args... args)
    {
        return impl.emplace(c, std::forward<Args>(args)...);
    }

    void remove(coordinates_type const &c, T &v) { impl.remove(c, v); }

    size_type size() const{return impl.size();}
};
}
}

#endif
