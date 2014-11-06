#ifndef LAZY_SET_H
#define LAZY_SET_H

#include "lazy_set_chunk.h"
#include "lazy_set_impl.h"

namespace simbad{
namespace core{

template<class T, size_t chunk_size=DEFAULT_CHUNK_SIZE  >
class LazySet
{
public:
    /*
     * types
     */
    typedef LazySetChunk<T,chunk_size>          chunk_type;
    typedef T                                   value_type;
    typedef LazySet_impl<chunk_type>            impl;
    typedef typename impl::reference_type       reference_type;
    typedef typename impl::const_reference_type const_reference_type;
    typedef typename impl::iterator             iterator;
    typedef typename impl::const_iterator       const_iterator;
    typedef typename impl::size_type            size_type;
private:
    impl impl_;

public:
    /*
     * Constructors and assignments
     */
    LazySet() = default;
    LazySet( LazySet const &) = delete;
    LazySet( LazySet && ) = default;

    LazySet & operator=( LazySet const &) = delete;
    LazySet & operator=( LazySet && ) = default;

    /*
     * Accessors
     */
    reference_type front(){
        return *impl_.begin();
    }
    const_reference_type front() const{
        return *impl_.begin();
    }

    reference_type back(){
        iterator it = impl_.end();
        return *(--it);
    }

    const_reference_type back() const{
        const_iterator it = impl_.end();
        return *(--it);
    }


    /*
     * Modifiers
     */
    template<class ... Args>
    iterator emplace_back( Args &&...args ){
        return impl_.emplace_back(std::forward<Args>(args)...);
    }

    void pop_back(){
        impl_.pop_back();
    }

    void swap_and_delete( T& ref){
        std::swap(ref,back());
        pop_back();
    }
    /*
     * Status
     */
    size_t size() const{
        return impl_.size();
    }
    bool empty() const{
        return 0 == impl_.size();
    }

    /*
     * Iterators
     */
    iterator begin(){
        return impl_.begin();
    }

    iterator end(){
        return impl_.end();
    }

    const_iterator begin() const{
        return impl_.begin();
    }

    const_iterator end() const{
        return impl_.end();
    }

    iterator find( value_type const &v){
        return impl_.find(v);
    }

    const_iterator find( value_type const &v ) const{
      return impl_.find(v);
    }


};


}}
#endif
