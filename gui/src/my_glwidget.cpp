
//#include <QtWidgets>
//#include <QtOpenGL>
//#include <QtCore>
//#include <GL/gl.h>
//#include <GL/glaux.h>
//#include <GL/glcorearb.h>
//#include <GL/glu.h>
//#include <GL/glext.h>

//#include <QtWidgets>
//#include <QtOpenGL>
#include "my_glwidget.h"
//#include "GL/gl.h"

//#ifdef GL_DEPTH_TEST
//#include "QtGui/"
My_GLWidget::My_GLWidget(QWidget *parent) :
    QGLWidget(parent)
{
}

void My_GLWidget::initializeGL()
{
    qglClearColor(Qt::black);
//   glEnable(GL_DEPTH);
//  glClearColor(0.0, 0.0, 102.0/255.0, 0.0);


//    glEnable();
//      glEnable(GL_DEPTH_TEST);

    //    glEnable(GL_CULL_FACE);
//    glShadeModel(GL_SMOOTH);
//    glEnable(GL_LIGHTING);
//    glEnable(GL_LIGHT0);

//    static GLfloat lightPosition[4] = { 0, 0, 10, 1.0 };
//    glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);

//  glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);

//  glClearColor(1.0, 1.0, 1.0, 0.0);


//	glMatrixMode(GL_PROJECTION);

//	glLoadIdentity();
//	glOrtho(0.0, 1.0, 0.0, 1.0, -1.0, 1.0);



}
void My_GLWidget::paintGL()
{
 //   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//    glLoadIdentity();
//    glTranslatef(0.0, 0.0, -10.0);
//    glRotatef(xRot / 16.0, 1.0, 0.0, 0.0);
//    glRotatef(yRot / 16.0, 0.0, 1.0, 0.0);
//    glRotatef(zRot / 16.0, 0.0, 0.0, 1.0);
//    draw();



//    glClear(GL_COLOR_BUFFER_BIT);
//	  glColor3f(1.0, 1.0, 1.0);



    //	glutSwapBuffers();
//	glFlush();



}
void My_GLWidget::resize(int w,int h)
{
//    int side = qMin(width, height);
//    glViewport((width - side) / 2, (height - side) / 2, side, side);

//    glMatrixMode(GL_PROJECTION);
//    glLoadIdentity();
//#ifdef QT_OPENGL_ES_1
//    glOrthof(-2, +2, -2, +2, 1.0, 15.0);
//#else
//    glOrtho(-2, +2, -2, +2, 1.0, 15.0);
//#endif
//    glMatrixMode(GL_MODELVIEW);
}
