#ifndef BOARD_LINEAR_ITERATOR
#define BOARD_LINEAR_ITERATOR

#include <boost/intrusive/unordered_set.hpp>
#include <boost/iterator.hpp>

namespace simbad
{
namespace core
{
/*
 * Iterator over ALL the elements in a board
 */

template <class D_, class board>
class board_iterator : public boost::iterator_facade<
                           board_iterator<D_, board>,   // itself
                           D_,                          // value
                           boost::forward_traversal_tag // travelsal type
                           >
{
    static constexpr bool is_const = std::is_const<D_>::value;
    using board_type =
        typename std::conditional<is_const, board const, board>::type;
    using set_type =
        typename std::conditional<is_const, typename board_type::set_type const,
                                  typename board_type::set_type>::type;
    using tile_type = typename std::conditional<
        is_const, typename board_type::tile_type const,
        typename board_type::tile_type>::type;
    using outer_iter =
        typename std::conditional<is_const, typename set_type::const_iterator,
                                  typename set_type::iterator>::type;
    using inner_iter =
        typename std::conditional<is_const, typename tile_type::const_iterator,
                                  typename tile_type::iterator>::type;
    struct enabler
    {
    }; // for std::enable_if
  public:
    explicit board_iterator(bool valid = false)
        : p_set(nullptr), oit(), iit(), valid(valid)
    {
    }
    explicit board_iterator(set_type &s)
        : p_set(&s), oit(s.begin()), iit(oit->begin()), valid(true)
    {
        while (iit == oit->end())
        {
            ++oit;
            if (p_set->end() == oit)
            {
                valid = false;
                return;
            }
            iit = oit->begin();
        }
    }

    template <class OtherD_>
    board_iterator(
        board_iterator<OtherD_, board> const &o,
        typename std::enable_if<std::is_convertible<OtherD_ *, D_ *>::value,
                                enabler>::type = enabler())
        : p_set(o.p_set), oit(o.oit), iit(o.iit), valid(o.valid)
    {
    }

    board_iterator(set_type &s, outer_iter o, inner_iter i, bool v = true)
        : p_set(&s), oit(o), iit(i), valid(v)
    {
    }

    bool is_valid() const { return valid; }

    outer_iter get_outer() const { return oit; }

    inner_iter get_inner() const { return iit; }

  protected:
    friend class boost::iterator_core_access;
    template <class, class> friend class board_iterator;

    template <class OtherD_>
    inline bool equal(board_iterator<OtherD_, board> const &o) const
    {
        if (!valid)
            return !o.valid;
        else
            return iit == o.iit && oit == o.oit && p_set == o.p_set;
    }

    inline void increment()
    {
        ++iit;
        while (oit->end() == iit)
        {
            ++oit;
            if (p_set->end() == oit)
            {
                valid = false;
                return;
            }
            iit = oit->begin();
        }
    }

    inline D_ &dereference() const { return *iit; }

  private:
    set_type *p_set;
    outer_iter oit;
    inner_iter iit;
    bool valid;
};
}
}
#endif
