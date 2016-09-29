#ifndef LAZY_SET_IMPL_H
#define LAZY_SET_IMPL_H

#include <boost/intrusive/avl_set.hpp>
#include <boost/intrusive/list.hpp>

#include <algorithm>
#include <assert.h>
#include <cstddef>
#include <type_traits>

namespace simbad
{
namespace core
{

/*
 * Class that wraps chunks as nodes of list and search tree
 */
template <class Chunk>
struct LazySetNode : public Chunk,
                     public boost::intrusive::avl_set_base_hook<>,
                     public boost::intrusive::list_base_hook<>
{
};

/*
 * Template of an iterator over LazySet
 */
template <class D_, class Node>
class LazySetIterator_
    : public boost::iterator_facade<
          LazySetIterator_<D_, Node>,        // itself
          D_,                                // value
          boost::bidirectional_traversal_tag // travelsal type
          >
{
  using value_type = typename std::remove_const<D_>::type;
  static constexpr bool is_const = std::is_const<D_>::value;
  using node_type = Node;
  using inner_iter =
      typename std::conditional<is_const, typename node_type::const_iterator,
                                typename node_type::iterator>::type;
  using list_type = typename boost::intrusive::list<node_type>;
  using outer_iter =
      typename std::conditional<is_const, typename list_type::const_iterator,
                                typename list_type::iterator>::type;

  using size_type = size_t;

  struct enabler
  {
  }; // for std::enable_if
public:
  LazySetIterator_() : oit(), idx(0) {}

  LazySetIterator_(outer_iter oit, size_type idx) : oit(oit), idx(idx) {}
  LazySetIterator_(outer_iter oit, inner_iter iit)
      : oit(oit), idx(iit - oit->begin())
  {
  }

  template <class OtherD_>
  LazySetIterator_(
      LazySetIterator_<OtherD_, Node> const &o,
      typename std::enable_if<std::is_convertible<OtherD_ *, D_ *>::value,
                              enabler>::type = enabler())
      : oit(o.oit), idx(o.idx)
  {
  }

  inner_iter get_inner() const { return oit->begin() + idx; }

  outer_iter get_outer() const { return oit; }

protected:
  friend class boost::iterator_core_access; // required by Boost
  template <class, class>
  friend class LazySetIterator_; // allows conversions

  template <class OtherD_>
  bool equal(LazySetIterator_<OtherD_, Node> const &o) const
  {
    return idx == o.idx && oit == o.oit;
  }

  void increment()
  {
    ++idx;
    if (idx >= node_type::length())
    {
      ++oit;
      idx = 0;
    }
  }
  void decrement()
  {
    if (idx == 0)
    {
      --oit;
      idx = node_type::length() - 1;
    }
    else
    {
      --idx;
    }
  }
  D_ &dereference() const { return oit->at(idx); }

private:
  outer_iter oit;
  size_type idx; // TODO: change to  inner_iter iit;
};

/*
 * Comparator used for ordering collections' elements by addres,
 * used in search tree for fast search for the chunk which contains
 * specified element
 */
struct LowerAddr
{
  template <class C>
  bool operator()(LazySetNode<C> const &o1, LazySetNode<C> const &o2) const
  {
    return std::less<const void *>()(static_cast<const void *>(&o1),
                                     static_cast<const void *>(&o2));
  }
};

struct LowerAddrThanEnd
{
  template <class C, class D>
  bool operator()(LazySetNode<C> const &n, D &v) const
  {
    return std::less<const void *>()(
        static_cast<const void *>(reinterpret_cast<const char *>(&n) +
                                  sizeof(LazySetNode<C>)),
        static_cast<const void *>(&v));
  }
};

/*
 * Disposer which deletes the node in boost::intrusive
 */
struct DeleteNodeDisposer
{
  template <class Node>
  void operator()(Node *delete_this)
  {
    delete delete_this;
  }
};

template <class SetChunk>
class LazySet_impl
{

public:
  /*
   * Types
   */
  typedef SetChunk chunk_type;
  typedef typename chunk_type::value_type value_type;
  typedef value_type &reference_type;
  typedef value_type const &const_reference_type;
  typedef LazySetNode<chunk_type> node_type;
  typedef size_t size_type;

  typedef boost::intrusive::list<node_type> list_type;
  typedef boost::intrusive::avl_set<node_type,
                                    boost::intrusive::compare<LowerAddr>>
      tree_type;

  typedef LazySetIterator_<value_type, node_type> iterator;
  typedef LazySetIterator_<const value_type, node_type> const_iterator;

  /*
   * Constructors, destructors and assignments
   */
  LazySet_impl() noexcept : size_(0)
  {
    pushChunk();
    itEnd = iterator(list.begin(), list.begin()->begin());
  }
  LazySet_impl(LazySet_impl const &) = delete;
  LazySet_impl(LazySet_impl &&) = default;

  ~LazySet_impl() noexcept
  {
    tree.clear();
    list.clear_and_dispose(DeleteNodeDisposer());
  }

  LazySet_impl &operator=(LazySet_impl const &) = delete;
  LazySet_impl &operator=(LazySet_impl &&) = default;
  /*
   * Status accessors
   */
  size_type size() const { return size_; }

  bool isEmpty() const { return list.front().isEmpty(); }

  /*
   * Modifiers
   */
  template <class... Args>
  iterator emplace_back(Args &&... args)
  {

    typename list_type::iterator itOuter = itEnd.get_outer();
    typename chunk_type::iterator itInner = itEnd.get_inner();

    itOuter->emplace_back(std::forward<Args>(args)...);

    if (++(itInner) == itEnd.get_outer()->end())
      pushChunk();
    iterator it = itEnd;
    ++itEnd;

    ++size_;

    return it;
  }

  void pop_back()
  {
    assert(!isEmpty());

    --itEnd; // now it points to the last element
    typename list_type::iterator itNode = itEnd.get_outer();
    itNode->pop_back();

    if (itNode->isEmpty())
    {
      // just made one of the nodes empty
      // check if there is another one empty
      ++itNode;
      if (itNode != list.end() && itNode->isEmpty())
        popChunk(); // and release it
    }

    --size_;
  }
  void clear()
  {
    *this = LazySet_impl();
  }

  /*
   * Visitors
   */

  template <class Self, class Visitor>
  static void s_visit(Self &self_ref, Visitor v)
  {
    std::for_each(self_ref.begin(), self_ref.end(), v);
  }

  template <class Visitor>
  void visit(Visitor v = Visitor())
  {
    s_visit(*this, v);
  }

  template <class ConstVisitor>
  void visit(ConstVisitor v = ConstVisitor()) const
  {
    s_visit(*this, v);
  }
  /*
   * Iterator related
   */

  iterator begin()
  {
    typename list_type::iterator oi = list.begin();
    typename chunk_type::iterator ii = list.begin()->begin();

    return iterator(oi, ii);
  }
  iterator end() { return itEnd; }

  const_iterator begin() const
  {
    typename list_type::const_iterator oi = list.begin();
    typename chunk_type::const_iterator ii = list.begin()->begin();

    return const_iterator(oi, ii);
  }
  const_iterator end() const { return itEnd; }

  iterator find(const_reference_type v)
  {
    typename tree_type::iterator tit;
    tit = tree.lower_bound(v, LowerAddrThanEnd());
    if (tit == tree.end())
      return end();

    typename chunk_type::iterator cit = tit->find(v);
    if (tit->end() == cit)
      return end();

    typename list_type::iterator lit = list_type::s_iterator_to(*tit);

    return iterator(lit, cit);
  }

  const_iterator find(const_reference_type v) const
  {
    typename tree_type::const_iterator tit;
    tit = tree.lower_bound(v, LowerAddrThanEnd());
    if (tit == tree.end())
      return end();

    typename chunk_type::const_iterator cit = tit->find(v);
    if (tit->end() == cit)
      return end();

    typename list_type::const_iterator lit = list_type::s_iterator_to(*tit);

    return const_iterator(lit, cit);
  }

  /*
   * Internal use only
   */
protected:
  void pushChunk()
  {
    node_type *pNode = new node_type;
    list.push_back(*pNode);
    tree.insert(*pNode);
  }

  void popChunk()
  {
    typename tree_type::iterator tit = tree.find(list.back());
    tree.erase(tit);
    list.pop_back_and_dispose(DeleteNodeDisposer());
  }

private:
  size_type size_; // counts value_type elements
  list_type list;  // keeps the order of chunks
  tree_type tree;  // allows search for particular address
  iterator itEnd;  // points past the last element

  //    template<class,class> friend class LazySetIterator_;
  //    friend class ::HashGridQuickTest;
};
}
}
#endif
