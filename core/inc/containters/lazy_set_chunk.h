#ifndef LAZYSETCHUNK_H
#define LAZYSETCHUNK_H

#include <array>
#include <cstddef>
#include <inttypes.h>
#include <algorithm>
#include <memory>
#include <boost/iterator/iterator_adaptor.hpp>

namespace simbad{
namespace core {

struct default_size{
    typedef default_size type;
    typedef unsigned long value_type;
    constexpr static const value_type value=1000u;
};

template <class D, class size_param = default_size >
class
LazySetChunk
{

public:
    typedef D                                               value_type;
    typedef std::array<char,sizeof(D)*size_param::value >   array_type;
    typedef std::array<D,size_param::value>                 emulated_array_type;
    typedef typename size_param::value_type                 size_type;
    //template<class> friend class                            LazySet;

    typedef typename emulated_array_type::iterator iterator;
    typedef typename emulated_array_type::const_iterator const_iterator;

/*
 * Constructors, destructors, assigments
 */

    LazySetChunk() noexcept: occupied(0) {}
    LazySetChunk( LazySetChunk const & ) = delete;
    LazySetChunk( LazySetChunk && ) = default;

    LazySetChunk & operator=(LazySetChunk const &) = delete;
    LazySetChunk & operator=(LazySetChunk &&) = default;

    ~LazySetChunk(){
        while( occupancy()>0 ){
            pop_back();
        }
    }

    emulated_array_type &getEmulatedArray(){
        return *reinterpret_cast<emulated_array_type*>(&array);
    }

    emulated_array_type const &getEmulatedArray() const{
        return *reinterpret_cast<emulated_array_type const*>(&array);
    }

    /*
     * Modifiers
     */

    template<class ...Args>
    D &emplace_back(Args &&...args)
    {
        D *ptr = new(getPlace(occupied)) D( std::forward<Args>(args)...);
        ++occupied;
        return *ptr;
    }

    void pop_back(){
        assert(!isEmpty());
        at(occupied-1).~D();
        --occupied;

    }
    /*
     * status queries
     */
    size_type occupancy() const{
        return occupied;
    }
    constexpr static size_type length(){
        return size_param::value;
    }

    bool isFull() const{
        return occupied==size_param::value;
    }

    bool isEmpty() const{
        return occupied==0;
    }

    bool containsByAddress( D const *ptr ) const
    {
        if( std::greater_equal<D const*>()(ptr, getEmulatedArray().data()) &&
                std::less_equal<D const *>()(ptr, getEmulatedArray().data() + this->length() - 1) )
            return true;
        else
            return false;
    }

    size_type findIdx( D const *p ) const{
        return p - getEmulatedArray().data();
    }

    iterator find( D const *p ){
        if( !containsByAddress(p) )
            return end();
        else
            return begin() + findIdx(*p);
    }

    const_iterator find( D const *p ) const{
        if( !containsByAddress(p) )
            return end();
        else
            return begin() + findIdx(p);
    }

    /*
     * Element accessors
     */

    D &back(){
        assert(!isEmpty());
        return at(occupied-1);
    }
    const D &back() const{
        assert(!isEmpty());
        return at(occupied-1);
    }

    D &at( size_type idx ){
        assert(idx<occupied);
        return *reinterpret_cast<D*>(getPlace(idx));
    }

    const D &at( size_type idx ) const{
        assert(idx<occupied);
        return *reinterpret_cast<const D*>(getPlace(idx));
    }

    /*
     * Iterators retrievers
     */

    iterator begin(){
        return getEmulatedArray().begin();
    }
    const_iterator begin() const{
        return getEmulatedArray().begin();
    }

    iterator end(){
        return getEmulatedArray().begin()+occupancy();
    }

    const_iterator end() const{
        return getEmulatedArray().begin()+occupancy();
    }

protected:
    char* getPlace( size_type idx ){
        return &array.at( idx*sizeof(D) );
    }

    const char* getPlace( size_type idx ) const{
        return &array.at( idx*sizeof(D) );
    }


private:
    size_type occupied;
    array_type array;

};


}}
#endif
