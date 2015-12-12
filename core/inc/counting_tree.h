#ifndef COUNTINGTREE_H
#define COUNTINGTREE_H
#include <boost/intrusive/rbtree_algorithms.hpp>
#include <cassert>
#include <stddef.h>
#include <algorithm>
#include <vector>
#include <type_traits>
#include <limits>

namespace counting_tree{

template<class Countable>
struct hook{
    typedef Countable countable_type;
    explicit
    hook( hook *parent=nullptr,
          hook *left_child=nullptr,
          hook *right_child=nullptr,
          countable_type countable=0)
        : m_parent(parent)
        , m_left_child(left_child)
        , m_right_child(right_child)
        , m_countable(countable)
    {}
    hook *m_parent;
    hook *m_left_child;
    hook *m_right_child;
    countable_type m_countable;
};

template< class Countable>
struct hook_traits_templ : public hook< Countable>
{
    typedef Countable                   countable_type;
    typedef hook<countable_type>        hook_type;
    typedef hook_type *                      hook_ptr;
    typedef const hook_type *                const_hook_ptr;


    static hook_ptr get_parent( const_hook_ptr h ) {
        return h->m_parent;
    }
    static hook_ptr get_left_child( const_hook_ptr h ) {
        return h->m_left_child;
    }
    static hook_ptr get_right_child( const_hook_ptr h) {
        return h->m_right_child;
    }
    static countable_type get_countable( const_hook_ptr h ){
        return h->m_countable;
    }

    static void set_parent( hook_ptr h, hook_ptr p) {
        h->m_parent = p;
    }
    static void set_left_child( hook_ptr h, hook_ptr c ) {
        h->m_left_child=c;
    }
    static void set_right_child( hook_ptr h, hook_ptr c ){
        h->m_right_child = c;
    }
    static void set_countable( hook_ptr h, countable_type c ){
        h->m_countable = c ;
    }
};

template< class Node, class CountTraits >
struct node_traits_templ{
    typedef Node                                    node_type;
    typedef Node *                                  node_ptr;
    typedef Node const *                            const_node_ptr;
    typedef Node &                                  node_reference;
    typedef Node const &                            const_node_reference;

    typedef CountTraits                             count_traits;
    typedef typename count_traits::countable_type   countable_type;

    typedef hook_traits_templ<countable_type>       hook_traits;
    typedef typename hook_traits::hook_ptr          hook_ptr;
    typedef typename hook_traits::const_hook_ptr    const_hook_ptr;

    static node_ptr get_parent( const_node_ptr n ){
        hook_ptr h = hook_traits::get_parent( static_cast<const_hook_ptr>(n) );
        return reinterpret_cast<node_ptr>(h);
    }
    static node_ptr get_left_child( const_node_ptr n ){
        hook_ptr h = hook_traits::get_left_child( static_cast<const_hook_ptr>(n) );
        return reinterpret_cast<node_ptr>(h);
    }
    static node_ptr get_right_child( const_node_ptr n){
        hook_ptr h = hook_traits::get_right_child( static_cast<const_hook_ptr>(n) );
        return reinterpret_cast<node_ptr>(h);
    }
    static countable_type get_countable( const_node_ptr n){
        return hook_traits::get_countable(n);
    }

    static void set_parent( node_ptr n, node_ptr p ){
        hook_traits::set_parent(n,p);
    }
    static void set_left_child( node_ptr n, node_ptr c){
        hook_traits::set_left_child(n,c);
    }
    static void set_right_child( node_ptr n, node_ptr c){
        hook_traits::set_right_child(n,c);
    }
    static void set_countable( node_ptr n, countable_type cnt ){
        hook_traits::set_countable( n, cnt );
    }

    static countable_type get_self_value( const_node_ptr n ){
        return count_traits::get_self_value( *n );
    }
};

template<class NodeTraits, class CountTraits>
struct counting_tree_algorithms{

    typedef NodeTraits                                  node_traits;
    typedef typename node_traits::node_type             node_type;
    typedef typename node_traits::node_ptr              node_ptr;
    typedef typename node_traits::const_node_ptr        const_node_ptr;

    typedef typename CountTraits::countable_type        countable_type;
    typedef size_t                                      size_type;

    enum CHILD{LEFT, RIGHT};

    static void cascade_update( node_ptr n){
        do
        {
            countable_type cnt = node_traits::get_self_value( n );

            const_node_ptr cl = node_traits::get_left_child(n);
            if( cl != nullptr ){

                cnt += node_traits::get_countable( cl );


            }
            const_node_ptr cr = node_traits::get_right_child(n);
            if( cr != nullptr ){
                cnt += node_traits::get_countable( cr );
            }

            node_traits::set_countable( n, cnt );
            n = node_traits::get_parent( n );

        }while( n != nullptr );
    }

    static node_ptr find( node_ptr root, size_type idx ){

        assert( root!= nullptr );

        node_ptr n = root;
        size_type g = idx+1;

        for( size_type b = trunc_power_of_2(g) >> 1; b > 0; b>>=1 )
        {
            CHILD t = g & b? RIGHT : LEFT;
            if( t == RIGHT )
                n = node_traits::get_right_child( n );
            else if( t==LEFT )
                n = node_traits::get_left_child( n );
            else
                assert(false);
            assert( n != nullptr );
        };


        return n;
    }

    static void push_back( node_ptr &root, node_ptr n, size_type &size ){
        if( root == nullptr )
        {
            assert( size==0 );
            root = n;
        }
        else
        {
            assert( size > 0 );
            node_ptr p = find( root, parent_idx(size)  );

            CHILD type = (size+1) & 0b1? RIGHT : LEFT;
            set_relation(p, n, type);

        }

        size++;
        cascade_update( n );
    }

    static void set_relation( node_ptr parent, node_ptr child, CHILD type){
        node_traits::set_parent( child, parent );
        if( type==RIGHT )
            node_traits::set_right_child( parent, child );
        else if( type==LEFT )
            node_traits::set_left_child( parent, child );
        else
            assert(false);
    }

    static  void swap_nodes( node_ptr n1, node_ptr n2 ){
        if( n1 == n2 )
            return;


        node_ptr p1,p2;

        p1 = node_traits::get_parent( n1 );
        p2 = node_traits::get_parent( n2 );

        CHILD t1 = node_traits::get_left_child( p1 ) == n1? LEFT : RIGHT;
        CHILD t2 = node_traits::get_left_child( p2 ) == n2? LEFT : RIGHT;

        set_relation( p1, n2, t1 );
        set_relation( p2, n1, t2 );

        node_ptr l1,l2,r1,r2;

        l1 = node_traits::get_left_child( n1 );
        l2 = node_traits::get_left_child( n2 );
        r1 = node_traits::get_right_child( n1 );
        r2 = node_traits::get_right_child( n2 );

        set_relation( n1, l2, LEFT );
        set_relation( n2, l1, LEFT );
        set_relation( n1, r2, RIGHT );
        set_relation( n2, r1, RIGHT );
    }

    static node_ptr pop_back( node_ptr &root, size_type &size ){
        assert( size > 0 );

        if( size == 1 ){
            node_ptr tmp = root;
            root = nullptr;
            size--;
            return tmp;
        }

        node_ptr n = find( root, size-1 );

        assert( node_traits::get_left_child(n) == nullptr );
        assert( node_traits::get_right_child(n) == nullptr );

        node_ptr p = node_traits::get_parent( n );

        CHILD type = node_traits::get_left_child( p ) == n? LEFT : RIGHT;

        if( type == LEFT )
            node_traits::set_left_child( p, nullptr );
        else
            node_traits::set_right_child( p, nullptr );

        node_traits::set_parent( n, nullptr );
        size--;

        return n;

    }

    static void remove( node_ptr &root, node_ptr n, size_type &size ){
        node_ptr last = pop_back( root, size );
        swap_nodes( n, last );
    }

    static node_ptr count_search( node_ptr root, countable_type count){

        countable_type target = count;
        node_ptr n = root;

        while(true)
        {
            countable_type total = node_traits::get_countable( n );
            assert(target < total );


            const countable_type sval =  node_traits::get_self_value(n);
            countable_type val = sval;
            if( target < sval )
                return n;


            node_ptr lc = node_traits::get_left_child( n );
            if( lc != nullptr )
            {
                val = sval + node_traits::get_countable( lc );

                if( target < val )
                {
                    n = lc;
                    target = target - sval;
                    continue;
                }
            }
            node_ptr rc = node_traits::get_right_child( n );
            assert( rc != nullptr );
            n = rc;
            target = target - val;



        }
    }

    static size_type parent_idx( size_type idx ){
        return (idx + 1)/2 - 1;
    }

    static size_type trunc_power_of_2( size_type v ){
        v |= v >> 1;
        v |= v >> 2;
        v |= v >> 4;
        v |= v >> 8;
        v |= v >> 16;
        v |= v >> 32;
        v++;
        v >>= 1;
        return v;
    }
    static bool validate_node( const_node_ptr const n ){
        if( n==nullptr )
            return false;
        const_node_ptr p = node_traits::get_parent( n );

        if( p!=nullptr ){
             bool l = n!= node_traits::get_left_child(p);
             bool r = n!= node_traits::get_right_child(p);
             if( !l && !r )
                 return false;
        }


        const_node_ptr lc = node_traits::get_left_child( n );
        const_node_ptr rc = node_traits::get_right_child( n );

        if( lc != nullptr && node_traits::get_parent(lc) != n )
            return false;
        if( rc != nullptr && node_traits::get_parent(rc) != n )
            return false;

        return true;

    }

    static bool cascade_validate( const_node_ptr r ){
        std::vector<const_node_ptr> stack;
        stack.reserve(64);
        stack.push_back(r);



        do
        {
            const_node_ptr n = stack.back();
            stack.pop_back();

            bool ok = validate_node( n );
            if( !ok )
                return false;

            const_node_ptr lc = node_traits::get_left_child( n );
            const_node_ptr rc = node_traits::get_right_child( n );

            if( lc != nullptr )
                stack.push_back( lc );
            if( rc != nullptr )
                stack.push_back( rc );


        }while( !stack.empty() );

        return true;
    }
};




template<class Value, class CountTraits_>
class CountingTree{
    // public types
public:
    ~CountingTree(){
        if( m_size > 0 )
            assert(false);
        if( m_root != nullptr )
            assert( false );
    }

    typedef CountTraits_ count_traits;

    typedef typename count_traits::countable_type countable_type;

    typedef Value value_type;
    typedef node_traits_templ<Value,count_traits> node_traits;

    typedef typename node_traits::node_ptr pointer_type;
    typedef typename node_traits::const_node_ptr const_pointer_type;

    typedef typename node_traits::node_reference reference_type;

    typedef counting_tree_algorithms<node_traits, count_traits> algorithms;
    typedef typename algorithms::size_type size_type;

    struct DoNothing{ template<class D> void operator()(D const & ){} };

    // constructors
    CountingTree()
        : m_root( nullptr )
        , m_size( 0 )
    {}


    // public member funcions
    void push_back( reference_type v ){
        algorithms::push_back(m_root, &v, m_size );
    }
    void remove( reference_type v){
        algorithms::remove(m_root, &v, m_size );
    }
    template< class Disposer>
    void removeAndDispose( reference_type v, Disposer d = Disposer() ){
        algorithms::remove( m_root, &v, m_size );
        d( &v );
    }

    void update( reference_type v ){
        algorithms::cascade_update( &v );
    }

    reference_type count_search( countable_type count ){
        return *algorithms::count_search( m_root, count );
    }
    void clear(){
        clearAndDispose(DoNothing() );
    }

    template<class Disposer>
    void clearAndDispose(Disposer d=Disposer() ){
        while( m_root != nullptr ){
            pointer_type ptr = algorithms::pop_back(m_root, m_size);
            d(&ptr);
        }
    }

protected:
    pointer_type m_root;
    size_type m_size;
};

}











#endif // COUNTINGTREE_H
