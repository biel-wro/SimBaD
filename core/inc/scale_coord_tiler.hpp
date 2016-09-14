#ifndef SCALE_COORD_TILER_HPP
#define SCALE_COORD_TILER_HPP
#include "core_def.hpp"
#include <algorithm>

#include "coordinates.hpp"

BEGIN_NAMESPACE_CORE

template <class space_coord_type, class board_coord_type, size_t DIM>
class scale_coord_tiler
{
public:
  using board_coords = coordinates<board_coord_type, DIM>;
  using space_coords = coordinates<space_coord_type, DIM>;

  scale_coord_tiler(board_coord_type tile_size = 1) : m_tile_size(tile_size) {}

  board_coords operator()(space_coords const &sc) const
  {
    board_coords bc;
    std::transform(sc.begin(), sc.end(), bc.begin(),
                   [this](space_coord_type const &sc) {
                     return std::floor(sc / m_tile_size);
                   });
    return bc;
  }

  board_coord_type tile_size() const { return m_tile_size; }
  void set_tile_size(board_coord_type ts) { m_tile_size = ts; }

private:
  board_coord_type m_tile_size;
};

END_NAMESPACE_CORE

#endif
