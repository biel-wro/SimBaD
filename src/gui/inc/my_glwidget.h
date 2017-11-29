#ifndef MY_GLWIDGET_H
#define MY_GLWIDGET_H


#include <QGLWidget>
#include <QtOpenGL>

#include "qslider.h"
#include "model_of_space.h"
#include <QTime>


class My_GLWidget : public QWidget
{

public:
    float scaling_factor;
    float shift_X;
    float shift_Y;
    explicit My_GLWidget(QWidget *parent = 0);
    simbad::gui::Model_of_space* Big_model;



    void drawSpacePoints(QPainter &painter);

protected:

    int X_Parent_Widjet_pos;
    int Y_Parent_Widjet_pos;

    void paintEvent (QPaintEvent *event);
signals:

public slots:
    void setParentWidjetpos (QPoint A);
    void animate();
};

#endif // MY_GLWIDGET_H
