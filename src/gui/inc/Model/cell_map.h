#ifndef CELL_MAP_H
#define CELL_MAP_H
#include "QVector"
#include "points_of_model.h"

namespace simbad{
namespace gui{

typedef QVector <int> id_of_points_in_cell;
typedef QVector <id_of_points_in_cell> id_of_type;

typedef QVector <int> Array_numbers_of_sizes_of_cells;

class Cell_Map
{

    const int matrix_scaling_factor = 100;
    const int range_of_call_array=1000000;//10 * 10 * matrix_scaling_factor * matrix_scaling_factor;

public:

    Cell_Map();
    QVector<id_of_type> Array_of_cells;


    QVector <Array_numbers_of_sizes_of_cells> Types_Array_numbers_of_sizes_of_cells;

    void setInitConfiguration(Points_of_model & Model_P);
};




}//namespace simbad{
}//namespace gui{

#endif // CELL_MAP_H
