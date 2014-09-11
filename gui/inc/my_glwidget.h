#ifndef MY_GLWIDGET_H
#define MY_GLWIDGET_H

//#include <QGLWidget>
//#include <qgl.h>
#include <QGLWidget>



class My_GLWidget : public QGLWidget
{
    Q_OBJECT
public:
    explicit My_GLWidget(QWidget *parent = 0);

    void initializeGL();
    void paintGL();
    void resize(int w,int h);


signals:

public slots:

};

#endif // MY_GLWIDGET_H
