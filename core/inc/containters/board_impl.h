#ifndef BOARD_IMPL_H
#define BOARD_IMPL_H

#include "board_tile.h"
#include "board_linear_iterator.h"
#include "board_region_iterator.h"

#include <boost/intrusive/unordered_set.hpp>
#include <boost/optional.hpp>

#include <array>
#include <cmath>
#include <initializer_list>
#include <utility>

namespace simbad
{
namespace core
{

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
    /*
     * container traits
     */
    using my_type = board_impl<T, config>;
    using value_type = T;
    using reference_type = T &;
    using const_refernce_type = T const &;
    using coordinates_type = board_coordinates<int, 2>;
    static constexpr size_t CHUNK_SIZE = 1000;

    /*
     * member types
     */
    using tile_type = board_tile<T, coordinates_type, CHUNK_SIZE>;

    using set_type = boost::intrusive::unordered_set<
        tile_type, boost::intrusive::equal<tile_comparator>,
        boost::intrusive::hash<tile_hasher>>;

    using bucket_type = typename set_type::bucket_type;
    using bucket_traits = typename set_type::bucket_traits;

    static constexpr const int DEFAULT_BUCKET_COUNT = 1024;

    /*
     * iterators
     */
    // linear
    using iterator = board_iterator<T, my_type>;
    using const_iterator = board_iterator<T const, my_type>;

    // box
    using box_tile_incrementer = coord_incrementer_box<coordinates_type>;
    using box_iterator =
        board_region_iterator<T, box_tile_incrementer, my_type>;
    using const_box_iterator =
        board_region_iterator<T const, box_tile_incrementer, my_type>;

    // wrapping
    using wrapping_tile_incrementer =
        coord_incrementer_wrapped<coordinates_type>;
    using wrapping_iterator =
        board_region_iterator<T, wrapping_tile_incrementer, my_type>;
    using const_wrapping_iterator =
        board_region_iterator<T const, wrapping_tile_incrementer, my_type>;

    // inner iterators
    using tile_iterator = typename set_type::iterator;
    using const_tile_iterator = typename set_type::const_iterator;
    using inner_iterator = typename tile_type::iterator;
    using const_inner_iterator = typename tile_type::const_iterator;
    using size_type = size_t;

    /*
     * region
     */
    template <class iter> struct basic_region
    {
        iter b, e;
        basic_region(iter beg, iter end) : b(beg), e(end) {}
        iter begin() { return b; }
        iter end() { return e; }
        //        typename std::enable_if<m, box_iterator>::type begin() {
        //        return beg; }
        //        typename std::enable_if<m, box_iterator>::type end() { return
        //        end; }
        //        box_iterator begin() const { return beg; }
        //        box_iterator end() const { return end; }
    };

    using box_region = basic_region<box_iterator>;
    using const_box_region = basic_region<const_box_iterator>;

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
        tile_iterator oit = find_tile_or_create(cs);
        inner_iterator iit = oit->emplace_back(std::forward<Args>(args)...);
        iterator it(tile_set, oit, iit);
        ++count;
        return it;
    }

    void remove(coordinates_type const &c, T &v)
    {
        tile_iterator oit = find_tile(c);
        assert(oit->end() != oit->find(v));
        oit->swap_and_delete(v);
        --count;
    }

    size_type size() const { return count; }

    /*
     * Linear iterators
     */

    iterator begin() { return iterator(tile_set); }
    const_iterator begin() const { return const_iterator(tile_set); }

    iterator end() { return iterator(false); }
    const_iterator end() const { return const_iterator(false); }

    iterator iterator_to(coordinates_type const &cs, T const &v)
    {
        tile_iterator oit = find_tile(cs);
        inner_iterator iit = oit->find(v);
        return iterator(tile_set, oit, iit);
    }

    const_iterator iterator_to(coordinates_type const &cs, T const &v) const
    {
        const_tile_iterator oit = find_tile(cs);
        const_inner_iterator iit = oit->find(v);
        return const_iterator(tile_set, oit, iit);
    }

    /*
     * Region iterators
     */
    box_region get_box(coordinates_type bmin, coordinates_type bmax)
    {
        box_tile_incrementer inc(bmin, bmax);
        box_iterator it(*this, inc);
        box_iterator end;
        return box_region(it, end);
    }

    const_box_region get_box(coordinates_type bmin, coordinates_type bmax) const
    {
        box_tile_incrementer inc(bmin, bmax);
        const_box_iterator it(*this, inc);
        const_box_iterator end;
        return const_box_region(it, end);
    }

    /*
     * Tile iterators
     */
    tile_iterator begin_tiles() { return tile_set.begin(); }
    const_tile_iterator begin_tiles() const { return tile_set.begin(); }

    tile_iterator end_tiles() { return tile_set.end(); }
    const_tile_iterator end_tiles() const { return tile_set.end(); }

    tile_iterator find_tile(coordinates_type const &cs)
    {
        return tile_set.find(cs, coord_hasher(), tile_comparator());
    }
    const_tile_iterator find_tile(coordinates_type const &cs) const
    {
        return tile_set.find(cs, coord_hasher(), tile_comparator());
    }

    tile_iterator find_tile_or_create(coordinates_type const &cs)
    {
        typename set_type::insert_commit_data cdata;
        std::pair<tile_iterator, bool> result;
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
