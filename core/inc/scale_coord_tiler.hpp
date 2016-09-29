#ifndef SCALE_COORD_TILER_HPP
#define SCALE_COORD_TILER_HPP
#include "core_def.hpp"
#include <algorithm>

#include "coordinates.hpp"

BEGIN_NAMESPACE_CORE

template <class space_scalar_type, class board_scalar_type, size_t DIM>
class scale_coord_tiler
{
public:
  using board_coords = coordinates<board_scalar_type, DIM>;
  using space_coords = coordinates<space_scalar_type, DIM>;

  scale_coord_tiler(board_scalar_type tile_size = 1) : m_tile_size(tile_size) {}

  board_coords operator()(space_coords const &sc) const
  {
    board_coords bc;
    std::transform(sc.begin(), sc.end(), bc.begin(),
                   [this](space_scalar_type const &sc) {
                     return std::floor(sc / m_tile_size);
                   });
    return bc;
  }

  board_scalar_type tile_size() const { return m_tile_size; }
  void set_tile_size(board_scalar_type ts) { m_tile_size = ts; }

private:
  board_scalar_type m_tile_size;
};

END_NAMESPACE_CORE

#endif
