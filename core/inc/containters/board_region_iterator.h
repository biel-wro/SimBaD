#ifndef BOARD_REGION_ITERATOR
#define BOARD_REGION_ITERATOR

#include <boost/intrusive/unordered_set.hpp>
#include <boost/iterator.hpp>
#include <boost/optional.hpp>

namespace simbad
{
namespace core
{

/*
 * Implementation of the iterator over elements in some region of the board
 */
template <class D_, class INC, class BOARD>
class board_region_iterator
    : public boost::iterator_facade<
          board_region_iterator<D_, INC, BOARD>, // itself
          D_,                                    // value
          boost::forward_traversal_tag           // travelsal type
          >
{
    static constexpr bool is_const = std::is_const<D_>::value;
    using incrementer_type = INC;

    using board_type =
        typename std::conditional<is_const, BOARD const, BOARD>::type;

    using tile_type = typename std::conditional<
        is_const, typename board_type::tile_type const,
        typename board_type::tile_type>::type;

    using outer_iter = typename std::conditional<
        is_const, typename board_type::const_tile_iterator,
        typename board_type::tile_iterator>::type;
    using inner_iter =
        typename std::conditional<is_const, typename tile_type::const_iterator,
                                  typename tile_type::iterator>::type;

    using coordinates_type = typename tile_type::coordinates_type;

    struct enabler
    {
    }; // for std::enable_if
  public:
    board_region_iterator() : p_board(nullptr), p_tile(nullptr), incrementer()
    {
    }

    board_region_iterator(board_type &board, INC inc = INC())
        : p_board(&board), p_tile(nullptr), incrementer(inc)
    {
        outer_iter tit = p_board->find_tile(incrementer.first());

        if (p_board->end_tiles() == tit) // board is empty, become an end
            return;

        p_tile = &*tit; // there is at least empty tile

        it = p_tile->begin();

        if (it != p_tile->end()) // first tile is non-empty
            return;

        // first tile is empty get a nonempty one
        p_tile = next_nonempty_tile(p_tile);

        if (nullptr == p_tile)
            return; // all tiles are empty, become an end

        it = p_tile->begin(); // found an non-empty tile, point at the begininig
    }

    template <class OtherD_>
    board_region_iterator(
        board_region_iterator<OtherD_, INC, BOARD> const &o,
        typename std::enable_if<std::is_convertible<OtherD_ *, D_ *>::value,
                                enabler>::type = enabler())
        : p_board(o.p_board),
          it(o.it),
          p_tile(o.p_tile),
          incrementer(o.incrementer)
    {
    }

  protected:
    friend class boost::iterator_core_access;
    template <class, class, class> friend class board_region_iterator;

    template <class OtherD_>
    inline bool equal(board_region_iterator<OtherD_, INC, BOARD> const &o) const
    {
        return p_tile == o.p_tile && (p_tile==nullptr || it == o.it);
    }

    inline void increment()
    {
        ++it;

        if (p_tile->end() != it)
            return;

        p_tile = next_nonempty_tile(p_tile);

        if(p_tile != nullptr )
            it = p_tile->begin();
    }

    inline D_ &dereference() const { return *it; }

  protected:
    inline tile_type *next_nonempty_tile(tile_type *p_tile)
    {

        coordinates_type c = p_tile->get_coords();
        outer_iter end_tiles = p_board->end_tiles();
        while (incrementer.next(c))
        {
            outer_iter tit = p_board->find_tile(c);

            if (end_tiles != tit && tit->begin() != tit->end())
                return &*tit;
        }
        return nullptr;
    }

    inline tile_type *last_nonempty_tile()
    {
        coordinates_type c = incrementer.last();
        outer_iter end_tiles = p_board->end_tiles();
        do
        {
            outer_iter tit = p_board->find_tile(c);

            if (p_board->end_tiles != tit && tit->begin() != tit->end())
                return &*tit;

        } while (incrementer.prev(c));

        return nullptr;
    }

  private:
    board_type *p_board;
    tile_type *p_tile;
    inner_iter it;
    incrementer_type incrementer;
};
}
}
#endif
