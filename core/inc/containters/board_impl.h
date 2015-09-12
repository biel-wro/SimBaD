#ifndef BOARD_IMPL_H
#define BOARD_IMPL_H

#include "board_tile.h"

#include <boost/intrusive/unordered_set.hpp>

#include <array>
#include <cmath>
#include <initializer_list>

namespace simbad
{
namespace core
{

/*
 * Implementation of the iterator over elements in some region of the board
 */
template <class D_, class itercfg>
class board_region_iterator
    : public boost::iterator_facade<
          board_region_iterator<D_, itercfg>, // itself
          D_,                                 // value
          boost::bidirectional_traversal_tag  // travelsal type
          >
{
    static constexpr bool is_const = std::is_const<D_>::value;

    using board_type =
        typename std::conditional<is_const, typename itercfg::board_type const,
                                  typename itercfg::board_type>::type;

    using tile_type = typename std::conditional<
        is_const, typename board_type::tile_type const,
        typename board_type::tile_type>::type;

    using tile_iter =
        typename std::conditional<is_const, typename tile_type::const_iterator,
                                  typename tile_type::iterator>::type;

    using board_coord = typename tile_type::board_coord;
    using region_coord = typename itercfg::region_coord;
    using limit_coord = typename itercfg::limit_coord;

    struct enabler
    {
    }; // for std::enable_if
  public:
    board_region_iterator() : p_board(nullptr) {}

    board_region_iterator(board_type &board, bool begin_not_end,
                          region_coord rmin, region_coord rmax,
                          limit_coord bmin = limit_coord(),
                          limit_coord bmax = limit_coord())
        : p_board(&board),
          rmin(std::move(rmin)),
          rmax(std::move(rmax)),
          bmin(std::move(bmin)),
          bmax(std::move(bmax)),
          valid(false)
    {
        if (begin_not_end)
            increment();
        else
            decrement();
    }

    template <class OtherD_>
    board_region_iterator(
        board_region_iterator<OtherD_, itercfg> const &o,
        typename std::enable_if<std::is_convertible<OtherD_ *, D_ *>::value,
                                enabler>::type = enabler())
        : p_board(o.p_board),
          it(o.it),
          p_tile(o.p_tile),
          rmax(o.rmax),
          rmin(o.rmin),
          bmax(o.bmax),
          bmin(o.bmin),
          valid(o.valid)
    {
    }

  protected:
    friend class boost::iterator_core_access;
    template <class, class> friend class board_region_iterator;

    template <class OtherD_>
    inline bool equal(board_region_iterator<OtherD_, itercfg> const &o) const
    {
        if (!valid)
            return !o.valid;
        else
            return it == o.it && p_tile == o.p_tile;
    }

    inline void increment()
    {
        if (valid) // including into the while condition complicates too much
            ++it;

        while (!valid ||
               p_tile->end() == it) // if done iterating over current tile
        {
            board_coord c = p_tile->get_coords();

            valid = increment_to_valid(c);

            if (!valid)
                return; // iterator is now end()
            else
            {
                p_tile = p_board->get_tile(c); // iterator goes to next tile
                it = p_tile->begin();
            }
        }
    }
    inline void decrement()
    {
        while (!valid || p_tile->begin() == it)
        {
            board_coord c = p_tile->get_coords();
            valid = decrement_to_valid(c);
            if (!valid)
                return;
            else
            {
                p_tile = &p_board->get_tile(c);
                it = p_tile->end();
            }
        }
        --it;
    }
    inline D_ &dereference() const { return *it; }

  private:
    inline bool increment_to_valid(board_coord &c)
    {
        while (true)
        {
            bool has_next = board_coord::next(c, rmin, rmax, bmin, bmax);
            if (has_next)
                return true;
            else if (p_board->tile_exists(c))
                return false;
        }
    }

    inline bool decrement_to_valid(board_coord &c)
    {
        while (true)
        {
            bool has_prev = board_coord::prev(c, rmin, rmax, bmin, bmax);
            if (!has_prev)
                return false;
            else if (p_board->tile_exists(c))
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
            else
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

struct tile_disposer
{
    template <class tile> void operator()(tile *ptr) { delete ptr; }
};

/*
 * Implementation of the main template class
 */
template <class T, class config> class board_impl
{
  public:
    using self_type = board_impl<T, config>;
    using value_type = T;
    using reference_type = T &;
    using const_refernce_type = T const &;
    using coordinates_type = board_coordinates<int, 2>;
    static constexpr size_t CHUNK_SIZE = 1000;

    using tile_type = board_tile<T, coordinates_type, CHUNK_SIZE>;

    using set_type = boost::intrusive::unordered_set<
        tile_type, boost::intrusive::equal<tile_comparator>,
        boost::intrusive::hash<tile_hasher>>;

    using bucket_type = typename set_type::bucket_type;
    using bucket_traits = typename set_type::bucket_traits;

    static constexpr const int DEFAULT_BUCKET_COUNT = 1024;

    struct linreg_itercfg
    {
        typedef board_impl<T, config> board_type;
        typedef coordinates_type board_coords;
    };

    using iterator = board_iterator<T, self_type>;
    using const_iterator = board_iterator<T const, self_type>;

    // typedef board_region_iterator<T, iter_config>           region_iterator;
    // typedef board_region_iterator<T const, iter_config>
    // const_region_iterator;

    using outer_iterator = typename set_type::iterator;
    using const_outer_iterator = typename set_type::const_iterator;
    using inner_iterator = typename tile_type::iterator;
    using const_inner_iterator = typename tile_type::const_iterator;
    using size_type = size_t;

  private:
    std::unique_ptr<bucket_type[]> buckets;
    set_type tile_set;
    size_type count;

  public:
    board_impl(size_t bucket_count = DEFAULT_BUCKET_COUNT)
        : buckets(new bucket_type[bucket_count]),
          tile_set(bucket_traits(buckets.get(), bucket_count))
    {
    }
    board_impl(board_impl const &) = delete;
    board_impl(board_impl &&) = default;
    board_impl &operator=(board_impl const &) = delete;
    board_impl &operator=(board_impl &&) = default;
    ~board_impl()
    {
        tile_set.clear_and_dispose(std::default_delete<tile_type>());
    }

    void rehash(size_t new_count)
    {
        bucket_type *new_buckets = new bucket_type[new_count];
        tile_set.rehash(bucket_traits(new_buckets, new_count));
        buckets.reset(new_buckets);
    }
    template <class... Args>
    iterator emplace(coordinates_type const &cs, Args... args)
    {
        outer_iterator oit = find_tile_or_create(cs);
        inner_iterator iit = oit->emplace_back(std::forward<Args>(args)...);
        iterator it(tile_set, oit, iit);
        ++count;
        return it;
    }

    void remove(coordinates_type const &c, T &v)
    {
        outer_iterator oit = find_tile_or_die(c);
        assert(oit->end() != oit->find(v));
        oit->swap_and_delete(v);
        --count;
    }

    size_type size() const { return count; }

    iterator iterator_to(coordinates_type const &cs, T const &v)
    {
        outer_iterator oit = find_tile_or_die(cs);
        inner_iterator iit = oit->find(v);
        return iterator(tile_set, oit, iit);
    }

    const_iterator iterator_to(coordinates_type const &cs, T const &v) const
    {
        const_outer_iterator oit = find_tile_or_die(cs);
        const_inner_iterator iit = oit->find(v);
        return const_iterator(tile_set, oit, iit);
    }

    iterator begin() { return iterator(tile_set); }

    const_iterator begin() const { return const_iterator(tile_set); }

    iterator end() { return iterator(false); }

    const_iterator end() const { return const_iterator(false); }
    /*
    region_iterator begin_region(){
      return region_iterator();
    }
  */

  protected:
    outer_iterator find_tile_or_die(coordinates_type const &cs)
    {
        return tile_set.find(cs, coord_hasher(), tile_comparator());
    }
    const_outer_iterator find_tile_or_die(coordinates_type const &cs) const
    {
        return tile_set.find(cs, coord_hasher(), tile_comparator());
    }

    outer_iterator find_tile_or_create(coordinates_type const &cs)
    {
        typename set_type::insert_commit_data cdata;
        std::pair<outer_iterator, bool> result;
        result =
            tile_set.insert_check(cs, coord_hasher(), tile_comparator(), cdata);
        if (result.second)
        {
            tile_type *p_tile = new tile_type(cs);
            result.first = tile_set.insert_commit(*p_tile, cdata);
        }
        return result.first;
    }
};
}
}
#endif // TILE_IMPL_H
