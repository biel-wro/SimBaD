#ifndef GLWIDJET_FOR_INIT_MODEL_H
#define GLWIDJET_FOR_INIT_MODEL_H

#include <QGLWidget>
#include "qslider.h"
#include "model_of_space.h"

#include <random>
using namespace std;



class GLWidjet_for_init_model : public QWidget
{
    //QTimer animationTimer;
    bool random_regime;
public:
    simbad::gui::Model_of_space* Big_model;
    GLWidjet_for_init_model(QWidget *parent = 0 );
    void setCurrent_type(int Type);
    void setCurrent_color_for_init_conf(QColor Color);
    void setCurrent_color_for_dynam_conf(QColor Color);
//default_random_engine generator;
  //  random_device RandomSeed;
//std::default_random_engine generator,generator_1;
//    int GetIntUniformRandomNamber();


public slots:
    void animate();
    void setBrushRadius(int value);
    void setIntensityValue(int value);
    void setParentWidjetpos (QPoint A);

protected:
    QColor Current_color_for_init_conf;
    QColor Current_color_for_dynam_conf;

    int BrushRadius;
    int IntensityValue;
    int X_Parent_Widjet_pos;
    int Y_Parent_Widjet_pos;
    int Current_type;
    void paintEvent (QPaintEvent *event);
    void mouseMoveEvent (QMouseEvent *event);
    void mousePressEvent ( QMouseEvent * event );

    //void actionEvent (QActionEvent * event );

//private:
public:
     void set_regime(bool random_reg);
//    void initializeGL();
//    void paintGL();
//    void resize(int w,int h);
//    void draw();
     void drawSpacePoints(QPainter &painter);
//     int GetIntRandomCoord(int maxNumber);

};

#endif // GLWIDJET_FOR_INIT_MODEL_H
