#ifndef GRID_HPP
#define GRID_HPP

#include "board.hpp"

namespace simbad {
namespace core{

template<class T, int dim, class TileAssigner, class board_coord=int, int chunk_sz=1000>
class grid : public board<T, dim, board_coord, chunk_sz>
{
public:
    explicit grid(TileAssigner ta=TileAssigner()) : tile_assigner(ta){}


protected:
    TileAssigner tile_assigner;
};


}
}

#endif // GRID_HPP
