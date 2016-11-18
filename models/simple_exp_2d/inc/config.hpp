#ifndef SIMPLE_EXP_2D_CONFIG_HPP
#define SIMPLE_EXP_2D_CONFIG_HPP

#include "simple_exp_2d_def.hpp"

#include "coordinates/coordinates.hpp"
#include "coordinates/scale_coord_tiler.hpp"

BEGIN_NAMESPACE_SIMPLE_EXP_2D
struct config
{
  using board_coord_scalar = int;
  using space_coord_scalar = float;

  static constexpr size_t dimension = 2;
  static constexpr size_t chunk_size = 1000;

  using space_coords = simbad::core::coordinates<space_coord_scalar, dimension>;
  using board_coords = simbad::core::coordinates<board_coord_scalar, dimension>;

  using coord_tiler =
      simbad::core::scale_coord_tiler<space_coord_scalar, board_coord_scalar,
                                      dimension>;
};

END_NAMESPACE_SIMPLE_EXP_2D

#endif
