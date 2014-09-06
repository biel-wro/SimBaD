#ifndef MODEL_OF_SPACE_H
#define MODEL_OF_SPACE_H
#include <QtCore>
#include"event_of_model.h"


namespace Ui {
    class Model_of_space;
}


namespace simbad{
namespace gui{


class Model_of_space
{
    QString Name_of_model;
    int dimention_of_model;
    int number_of_types_in_model;
//    QVector <Event_of_model> List_of_events_of_model;

public:
    Model_of_space();
    ~Model_of_space();


// functions for changing the Model_of_space
    void set_name_of_model(QString Name_of_the_model);
    void set_number_of_types(int Number);
    // it should be done in *.cpp
    void download_model_of_space_from_file(QFile file_sim);



// functions for examining the Model_of_space
    QString get_name_of_model();
    int get_number_of_types();

};//class Model_of_space


}// namespace gui

}// namespace simbad

#endif // MODEL_OF_SPACE_H
