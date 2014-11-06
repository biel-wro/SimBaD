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

constexpr size_t DEFAULT_CHUNK_SIZE = 1000;

template <class D, size_t chunk_size = DEFAULT_CHUNK_SIZE >
class
LazySetChunk
{

public:
    typedef D                                        value_type;
    typedef D&                                       reference_type;
    typedef D const &                                const_reference_type;
    typedef std::array<char,sizeof(D)*chunk_size >   array_type;
    typedef std::array<D,chunk_size>                 emulated_array_type;
    typedef size_t                                   size_type;
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
    iterator emplace_back(Args &&...args)
    {
        new(getPlace(occupied)) D( std::forward<Args>(args)...);
        iterator it = begin() + occupied;
        ++occupied;

        return it;
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
        return chunk_size;
    }

    bool isFull() const{
        return occupied==chunk_size;
    }

    bool isEmpty() const{
        return occupied==0;
    }

    bool containsByAddress( D const *ptr ) const
    {
    if( std::greater_equal<D const*>()(ptr, getEmulatedArray().data()) &&
        std::less_equal<D const *>()
            (ptr, getEmulatedArray().data() + this->length() - 1) )
        return true;
    else
        return false;
    }

    size_type findIdx( const_reference_type p ) const{
        return &p - getEmulatedArray().data();
    }

    iterator find( const_reference_type p ){
        if( !containsByAddress(&p) )
            return end();
        else
            return begin() + findIdx(p);
    }

    const_iterator find( const_reference_type p ) const{
        if( !containsByAddress( &p) )
            return end();
        else
            return begin() + findIdx(p);
    }

    /*
     * Element accessors
     */

    reference_type back(){
        assert(!isEmpty());
        return at(occupied-1);
    }
    const_reference_type back() const{
        assert(!isEmpty());
        return at(occupied-1);
    }

    reference_type at( size_type idx ){
        assert(idx<occupied);
        return *reinterpret_cast<D*>(getPlace(idx));
    }

    const_reference_type at( size_type idx ) const{
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
