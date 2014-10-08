
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
#include <QTime>
//#include "GL/gl.h"

//#ifdef GL_DEPTH_TEST
//#include "QtGui/"


My_GLWidget::My_GLWidget(QWidget *parent) :
    QWidget(parent)
{
    scaling_factor=1;
    shift_X=0.0;
    shift_Y=0.0;

}

void My_GLWidget::animate()
{
    repaint();
}

void My_GLWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter;
    painter.begin(this);

    painter.setRenderHint(QPainter::Antialiasing);

    painter.fillRect(event->rect(), QBrush(Qt::white));
 //   painter.setPen(Qt::red);
 //   painter.save();
 //   for(int i=0;i<500;i++){
 //     painter.drawPoint(i,i);
 //   };

    painter.setPen(Qt::blue);

    //Lines for coord. sistem
    if ((256 - (int) (512 * (shift_Y)) >=0) &&(256 - (int) (512 * (shift_Y)) <=512)   ) painter.drawLine(0,256 - (int) (512 * (shift_Y)),512,256 - (int) (512 * (shift_Y)));
    if ((256 - (int) (512 * (shift_X)) >=0) &&(256 - (int) (512 * (shift_X)) <=512)   ) painter.drawLine(256 - (int) (512 * (shift_X)),512,256 - (int) (512 * (shift_X)),0);


    QPoint CursorPoint;
    QCursor Cursor;
    Cursor=this->cursor();
    CursorPoint=Cursor.pos();
//    WidjetPoint=this->pos();

 //   painter.drawEllipse(CursorPoint.x()-X_Parent_Widjet_pos - BrushRadius*0.5-7,
  //                      CursorPoint.y()-Y_Parent_Widjet_pos - BrushRadius*0.5-30,BrushRadius,BrushRadius);

    drawSpacePoints(painter);

//    painter.restore();

//drawOutline(painter);
   painter.end();


   update();


}


void My_GLWidget::setParentWidjetpos (QPoint A)
{
    X_Parent_Widjet_pos=A.x();
    Y_Parent_Widjet_pos=A.y();
}

void My_GLWidget::drawSpacePoints(QPainter &painter)
{
    for(int i=0;i<Big_model->ModelPoints.Number_of_types;i++)
    {

        for(int j=0; j<Big_model->ModelPoints.Vector_of_types[i].Number_of_points_in_SpacePointArray; j++)
        {
//+++
 //           painter.drawPoint(256+(int) (512*Big_model->ModelPoints.Vector_of_types[i].SpacePointArray[j].getXcoordinate()),
 //                            256+(int) (512*Big_model->ModelPoints.Vector_of_types[i].SpacePointArray[j].getYcoordinate()));
//+++
            if (Big_model->ModelPoints.Vector_of_types[i].SpacePointArray[j].BirthTime == 0)
                painter.setPen(Big_model->ModelPoints.Vector_of_types[i].InitialConfigurationColor);
            else painter.setPen(Big_model->ModelPoints.Vector_of_types[i].DynamicSimulationColor);

            painter.drawPoint(256 + (int) (512 * (- shift_X + scaling_factor * Big_model->ModelPoints.Vector_of_types[i].SpacePointArray[j].Xcoordinate)),
                              256 - (int) (512 * ( shift_Y + scaling_factor * Big_model->ModelPoints.Vector_of_types[i].SpacePointArray[j].Ycoordinate)));

        }

    };


}
