#ifndef BOARD_TILE_H
#define BOARD_TILE_H

#include "coordinates/coordinates.hpp"
#include "lazy_set.hpp"

#include <boost/intrusive/unordered_set_hook.hpp>

BEGIN_NAMESPACE_CORE

/*
 * Board Tile contains particles which belongs to some cube in space
 */
using tile_hook = boost::intrusive::unordered_set_base_hook<
    /*boost::intrusive::store_hash<true>*/>;

template <class T, class BC, size_t CHUNK_SIZE = DEFAULT_CHUNK_SIZE>
class board_tile : public LazySet<T, CHUNK_SIZE>,
                   public tile_hook,
                   boost::totally_ordered<board_tile<T, BC, CHUNK_SIZE>>

{
public:
  /*
   * types
   */
  using value_type = T;
  using coordinates_type = BC;

  /*
   * constructors, assignments, destructor
   */
  board_tile() = delete;
  board_tile(const BC &board_coords) : coords(board_coords) {}
  board_tile(const board_tile &) = delete;
  board_tile(board_tile &&) = default;
  board_tile &operator=(const board_tile &) = delete;
  board_tile &operator=(board_tile &&) = default;
  ~board_tile() {}
  /*
   * operations on coordinates
   */
  BC const &get_coords() const { return coords; }
protected:
  BC coords;
};

struct tile_comparator
{
  template <class T, class BC, size_t CHS>
  bool operator()(board_tile<T, BC, CHS> const &o1,
                  board_tile<T, BC, CHS> const &o2) const
  {
    return o1.get_coords() == o2.get_coords();
  }

  template <class T, class BC, size_t CHS>
  bool operator()(board_tile<T, BC, CHS> const &b, BC const &c) const
  {
    return b.get_coords() == c;
  }

  template <class T, class BC, size_t CHS>
  bool operator()(BC const &c, board_tile<T, BC, CHS> const &b) const
  {
    return c == b.get_coords();
  }
};

struct tile_hasher
{
  template <class T, class BC, size_t CHS>
  std::size_t operator()(board_tile<T, BC, CHS> const &v) const
  {
    return coord_hasher()(v.get_coords());
  }
  template <class Tile> std::size_t operator()(Tile const &tile) const
  {
    return coord_hasher()(tile.get_coords());
  }
};

END_NAMESPACE_CORE
#endif // BOARD_TILE_H
