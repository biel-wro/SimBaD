#ifndef LAZY_SET_IMPL_H
#define LAZY_SET_IMPL_H

#include <assert.h>
#include <cstddef>

#include <boost/intrusive/avl_set.hpp>
#include <boost/intrusive/list.hpp>
#include <type_traits>

//#include "lazy_set_impl.h"

namespace simbad{
namespace core{

/*
 * Class that wraps chunks as nodes of list and search tree
 */
template<class Chunk>
struct LazySetNode :
        public Chunk,
        public boost::intrusive::avl_set_base_hook<>,
        public boost::intrusive::list_base_hook<>
{
    //typedef LazySetNode<Chunk> type;
    //explicit LazySetNode( LazySet *tile ):
    //    parent(tile)
    //{}
    //LazySet *parent; //TODO: why it is needed?
};

/*
 * Template of an iterator over LazySet
 */
template< class D_, class Node >
class LazySetIterator_ : public boost::iterator_facade<
        LazySetIterator_<D_,Node>,              // itself
        D_,                                     // value
        boost::bidirectional_traversal_tag      // travelsal type
        >
{
    typedef typename std::remove_const<D_>::type        base_value_type;
    typedef Node                                        node_type;
    typedef typename node_type::iterator                inner_iter;

    typedef typename boost::intrusive::list<node_type>  list_type;
    typedef typename list_type::iterator                outer_iter;

    typedef size_t size_type;

    struct enabler{}; // for std::enable_if
 public:
    LazySetIterator_() : oit(),idx(0) {}

    LazySetIterator_( outer_iter oit, size_type idx ) : oit(oit), idx(idx) {}
    LazySetIterator_( outer_iter oit, inner_iter iit ) : oit(oit), idx(iit-oit->begin() ) {}

    template< class OtherD_ >
    LazySetIterator_( LazySetIterator_<OtherD_,Node > const &o,
                   typename std::enable_if<std::is_convertible<OtherD_*,D_*>::value,enabler >::type = enabler()
                   ) :
        oit(o.oit),
        idx(o.idx)
    {}

    inner_iter get_inner() const{
        return oit->begin()+idx;
    }

    outer_iter get_outer() const{
        return oit;
    }

 protected:
    friend class boost::iterator_core_access;               // required by Boost
    template<class,class> friend class LazySetIterator_;    // allows conversions

    template< class OtherD_>
    bool equal(LazySetIterator_<OtherD_,Node> const& o) const
    {
        return idx == o.idx && oit == o.oit;
    }


    void increment()
    {
        ++idx;
        if( idx>=node_type::length() )
        {
            ++oit;
            idx = 0;
        }
    }
    void decrement()
    {
        if( idx==0 )
        {
            --oit;
            idx = node_type::length() - 1;
        }else{
            --idx;
        }
    }
    D_ & dereference() const{
        return oit->at(idx);
    }



private:
    outer_iter oit;
    size_type idx;  //TODO: change to  inner_iter iit;


};

/*
 * Comparator used for ordering collections' elements by addres,
 * used in search tree for fast search for the chunk which contains
 * specified element
 */
struct LowerAddr{
    template<class Obj1,class Obj2>
    bool operator()(Obj1 const &o1, Obj2 const &o2 ) const{
        return std::less<const void*>()(static_cast<const void*>(&o1), static_cast<const void*>(&o2));
    }
};

/*
 * Disposer which deletes the node in boost::intrusive
 */
struct DeleteNodeDisposer{
    template<class Node>
    void operator()(Node *delete_this){
        delete delete_this;
    }
};


template<class SetChunk>
class LazySet_impl{

public:

    /*
     * Types
     */
    typedef SetChunk                                        chunk_type;
    typedef typename chunk_type::value_type                 value_type;
    typedef value_type &                                    reference_type;
    typedef value_type const &                              const_reference_type;
    typedef LazySetNode<chunk_type>                         node_type;
    typedef size_t                                          size_type;

    typedef boost::intrusive::list<node_type>               list_type;
    typedef boost::intrusive::avl_set<
        node_type, boost::intrusive::compare<LowerAddr> >   tree_type;

    typedef LazySetIterator_<value_type, node_type>         iterator;
    typedef LazySetIterator_<const value_type,node_type>    const_iterator;

    /*
     * Constructors, destructors and assignments
     */
    LazySet_impl() noexcept :
        size_(0)
    {
        pushChunk();
        itEnd = iterator(list.begin(),list.begin()->begin() );
    }
    LazySet_impl( LazySet_impl const & ) = delete;
    LazySet_impl( LazySet_impl && ) = default;

    ~LazySet_impl() noexcept{
        tree.clear();
        list.clear_and_dispose( DeleteNodeDisposer() );
    }

    LazySet_impl & operator=( LazySet_impl const & ) = delete;
    LazySet_impl & operator=( LazySet_impl && ) = default;
    /*
     * Status accessors
     */
    size_type size() const{
        return size_;
    }

    bool isEmpty() const{
        return list.front().isEmpty();
    }

//    size_type size() const{

//        bool emptyLast = list.rbegin()->isFree();
//        size_type sz = 0;

//    if( !emptyLast )
//            sz = list.rbegin()->occupancy() + (list.size() - 1) * chunk_type::max_occupancy();
//        else{
//            const_list_reverse_iterator it = list.rbegin();
//            ++it;
//            if( it == list.rend() )
//                return 0;
//            sz = it->occupancy() + (list.size() - 2) * chunk_type::max_occupancy();
//        }
//        return sz;
//    }

//    bool checkConsistency() const{
//        bool ok = true;
//        typename list_type::const_reverse_iterator it = list.rbegin();

//        if( it->isFree() ){
//            ++it;
//            if( it == list.rend() )
//                return ok;
//            ok &= !it->isFree();
//            ++it;
//        }
//        else
//        {
//            ++it;
//        }
//        for( ; it!=list.rend(); ++it)
//        {
//            ok &= it->isFull();
//        }

//        if( begin()==end() )
//            ok &= size_==0;
//        if( size_==0 ){
//            ok &= begin()==end();
//        }

//        ok &= size_ == size();

//        return ok;
//    }


    /*
     * Modifiers
     */
    template<class ...Args>
    reference_type emplace_back( Args &&...args)
    {

        typename list_type::iterator itOuter = itEnd.get_outer();
        typename chunk_type::iterator itInner = itEnd.get_inner();

        reference_type ref = itOuter->emplace_back( std::forward<Args>(args)... );

        if( ++(itInner) == itEnd.get_outer()->end() )
            pushChunk();

        ++itEnd;

        ++size_;

        return ref;
    }



    void pop_back(){
        assert( !isEmpty() );

        --itEnd;       // now it points to the last element
        typename list_type::iterator itNode = itEnd.get_outer();
        itNode->pop_back();

        if( itNode->isEmpty() )
        {
            // just made one of the nodes empty
            // check if there is another one empty
            ++itNode;
            if( itNode != list.end() && itNode->isEmpty() )
                popChunk(); // and release it
        }

        --size_;
    }



    /*
     * Iterator related
     */

    iterator begin(){
        typename list_type::iterator oi = list.begin();
        typename chunk_type::iterator ii = list.begin()->begin();

        return iterator(oi,ii);
    }
    iterator end(){
        return itEnd;
    }

    const_iterator begin() const{
        typename list_type::const_iterator oi = list.begin();
        typename chunk_type::const_iterator ii = list.begin()->begin();

        return iterator(oi,ii);
    }
    const_iterator end() const{
        return itEnd;
    }


    /*
     * Internal use only
     */
protected:

    void pushChunk(){
        node_type *pNode = new node_type;
        list.push_back(*pNode);
        tree.insert(*pNode);
    }

    void popChunk(){
        typename tree_type::iterator tit = tree.find( list.back() );
        tree.erase(tit);
        list.pop_back_and_dispose(DeleteNodeDisposer());
    }

//    LazySetNode &tileNodeOf(D const& d){
//        tree_iterator it = tree.find(d,ChunkContains() );
//        return *it;
//    }

//    LazySetNode const &tileNodeOf(D const& d) const{
//        const_tree_interator it = tree.find(d,ChunkContains() );
//        return *it;
//    }

//    list_type::iterator listIteratorTo(D &d){
//        return s_listIteratorTo(tileNodeOf(d));
//    }
//    static list_iterator s_listIteratorTo(LazySetNode &node){
//        return list_type::s_iterator_to(node);
//    }

//    list_type::const_iterator listIteratorTo(D const &d){
//        return s_listIteratorTo(tileNodeOf(d));
//    }

//    static const_list_iterator s_listIteratorTo(LazySetNode const &node){
//        return list_type::s_iterator_to(node);
//    }
private:
    size_type size_;            // counts value_type elements
    list_type list;             // keeps the order of chunks
    tree_type tree;             // allows search for particular address
    iterator itEnd;             // points past the last element

//    template<class,class> friend class LazySetIterator_;
//    friend class ::HashGridQuickTest;

};

}}
#endif
