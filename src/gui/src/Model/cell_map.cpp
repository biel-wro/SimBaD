#include "cell_map.h"
#include "iostream"
#include "points_of_model.h"

simbad::gui::Cell_Map::Cell_Map() {}

void simbad::gui::Cell_Map::setInitConfiguration(Points_of_model &Model_P)
{
  // cout << "simbad::gui::Cell_Map::setInitConfiguration(Points_of_model &
  // Model_P)" << endl;

  //    cout << y << endl;
  // reserving spase for Array_of_cells
  Array_of_cells.resize(range_of_call_array);

  for(int i = 0; i < range_of_call_array; i++)
  {
    Array_of_cells[i].resize(Model_P.Number_of_types);
  };

  for(int k = 0; k < Model_P.Number_of_types; k++)
  {
    for(int i = 0; i < range_of_call_array; i++)
      Array_of_cells[i][k].reserve(10);
  };
  //    cout << "1" << endl;

  // Next init for correct work of algorithm-------------------

  Types_Array_numbers_of_sizes_of_cells.resize(Model_P.Number_of_types);

  for(int k = 0; k < Model_P.Number_of_types; k++)
    Types_Array_numbers_of_sizes_of_cells[k].resize(1);

  //-------------------------------------------------------

  for(int k = 0; k < Model_P.Number_of_types; k++)
  {
    for(int i = 0;
        i < Model_P.Vector_of_types[k].Number_of_points_in_SpacePointArray; i++)
    {
      int addtional_variable =
          (int)floorf(
              10 * Model_P.Vector_of_types[k].SpacePointArray[i].Xcoordinate +
              matrix_scaling_factor * 5) +
          matrix_scaling_factor * 10 *
              floorf(10 * Model_P.Vector_of_types[k]
                              .SpacePointArray[i]
                              .Ycoordinate +
                     matrix_scaling_factor * 5);

      //добавить индекс элемента в список
      Array_of_cells[addtional_variable][k].push_back(i);

      //добавить номер элемента в списке
      Model_P.Vector_of_types[k].SpacePointArray[i].number_of_point_in_cell =
          Array_of_cells[addtional_variable][k].size() - 1;
    };
  };

  for(int i = 0; i < 10; ++i)
    for(int j = 0; j < 10; ++j)
    {
      for(int k = 0; k < Model_P.Number_of_types; k++)
      {
        int addtional_variable = (int)floorf(
            matrix_scaling_factor * 5 + j - 5 +
            matrix_scaling_factor * 10 * (matrix_scaling_factor * 5 + i - 5));

        if(Array_of_cells[addtional_variable][k].size() >
           (Types_Array_numbers_of_sizes_of_cells[k].size() - 1))
        {
          int size_last = Types_Array_numbers_of_sizes_of_cells[k].size();
          for(int m = size_last;
              m < Array_of_cells[addtional_variable][k].size(); m++)
            Types_Array_numbers_of_sizes_of_cells[k].push_back(0);

          Types_Array_numbers_of_sizes_of_cells[k].push_back(1);
        }
        else
        {
          //нужно резервировать первый элемент (нулевой в массиве) с 0, иначе
          //вылет (это сделано в начале этой функции)
          Types_Array_numbers_of_sizes_of_cells
              [k][Array_of_cells[addtional_variable][k].size()] =
                  Types_Array_numbers_of_sizes_of_cells
                      [k][Array_of_cells[addtional_variable][k].size()] +
                  1;
        };
      };
    };

  // for (int k=0; k<Model_P.Number_of_types; k++)

  //  for (int i=0; i<
  //  Model_P.Vector_of_types[k].Number_of_points_in_SpacePointArray;i++)
  // cout <<i <<","<<
  //     Model_P.Vector_of_types[k].SpacePointArray[i].BirthTime
  //     <<","<<
  //     Model_P.Vector_of_types[k].SpacePointArray[i].number_of_point_in_cell
  //     <<","<<
  //     Model_P.Vector_of_types[k].SpacePointArray[i].Xcoordinate
  //     <<","<<
  //     Model_P.Vector_of_types[k].SpacePointArray[i].Ycoordinate
  //     <<","<<
  //        endl;

  // for (int k=0; k<Model_P.Number_of_types; k++)

  // for (int i=0;i<Types_Array_numbers_of_sizes_of_cells[k].size() ;i++)
  // cout <<"Types_Array_numbers_of_sizes_of_cells[k][i]="<<
  //       Types_Array_numbers_of_sizes_of_cells[k][i]<<endl;
}
