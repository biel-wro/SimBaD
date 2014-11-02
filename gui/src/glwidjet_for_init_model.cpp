#include "glwidjet_for_init_model.h"
#include <random>
#include <math.h>
#include "QMouseEvent"
//#include "point_of_space.h"
#include <chrono>
using namespace std;

GLWidjet_for_init_model::GLWidjet_for_init_model( QWidget *parent)
    : QWidget(parent){

    BrushRadius = 10;
    //this->setWindowFlags(Qt::  WStaticContents|WRepaintNoErase|WResizeNoErase);
    Big_model=NULL;
    Current_type=0;
    IntensityValue=10;
    Current_color_for_init_conf=Qt::black;
    Current_color_for_dynam_conf=Qt::gray;

}



void GLWidjet_for_init_model::animate()
{
//    elapsed = (elapsed + qobject_cast<QTimer*>(sender())->interval()) % 1000;
    repaint();
}


void GLWidjet_for_init_model::paintEvent(QPaintEvent *event)
{
    QPainter painter;
    painter.begin(this);


    painter.fillRect(event->rect(), QBrush(Qt::white));
    painter.setRenderHint(QPainter::Antialiasing);
 //   painter.setPen(Qt::red);
 //   painter.save();
 //   for(int i=0;i<500;i++){
 //     painter.drawPoint(i,i);
 //   };

    painter.setPen(Qt::blue);

    //Lines for coord. sistem
    painter.drawLine(0,256,512,256);
    painter.drawLine(256,512,256,0);


    QPoint CursorPoint;
    QCursor Cursor;
    Cursor=this->cursor();
    CursorPoint=Cursor.pos();
//    WidjetPoint=this->pos();

    painter.drawEllipse(CursorPoint.x()-X_Parent_Widjet_pos - BrushRadius*0.5-7,
                        CursorPoint.y()-Y_Parent_Widjet_pos - BrushRadius*0.5-30,BrushRadius,BrushRadius);

    drawSpacePoints(painter);

//    painter.restore();

//drawOutline(painter);
   painter.end();


   update();


}


void GLWidjet_for_init_model::mouseMoveEvent ( QMouseEvent * event)
{
    repaint();



}


void GLWidjet_for_init_model::setBrushRadius(int value)
{
    BrushRadius = value;
}

void GLWidjet_for_init_model::setIntensityValue(int value)
{
    IntensityValue=value;
}


void GLWidjet_for_init_model::setParentWidjetpos (QPoint A)
{
    X_Parent_Widjet_pos=A.x();
    Y_Parent_Widjet_pos=A.y();
}

void GLWidjet_for_init_model::drawSpacePoints(QPainter &painter)
{
    for(int i=0;i<Big_model->ModelPoints.Number_of_types;i++)
    {
        painter.setPen(Big_model->ModelPoints.Vector_of_types[i].InitialConfigurationColor);

        for(int j=0; j<Big_model->ModelPoints.Vector_of_types[i].Number_of_points_in_SpacePointArray; j++)
        {
//+++
 //           painter.drawPoint(256+(int) (512*Big_model->ModelPoints.Vector_of_types[i].SpacePointArray[j].getXcoordinate()),
 //                            256+(int) (512*Big_model->ModelPoints.Vector_of_types[i].SpacePointArray[j].getYcoordinate()));
            painter.drawPoint(256+(int) (512*Big_model->ModelPoints.Vector_of_types[i].SpacePointArray[j].Xcoordinate),
                              256-(int) (512*Big_model->ModelPoints.Vector_of_types[i].SpacePointArray[j].Ycoordinate));

        }

    };


}
void GLWidjet_for_init_model::setCurrent_type(int Type)
{
    Current_type=Type;
}

void GLWidjet_for_init_model::set_regime(bool random_reg)
{
    random_regime=random_reg;
}

void GLWidjet_for_init_model::mousePressEvent( QMouseEvent * event )
{
    QPoint CursorPoint;
    QCursor Cursor;
    Cursor=this->cursor();
    CursorPoint=Cursor.pos();

    int XCoord_Widjet = CursorPoint.x()-X_Parent_Widjet_pos-7;
    int YCoord_Widjet =  CursorPoint.y()-Y_Parent_Widjet_pos -30;

    int j=0;
    QVector <simbad::gui::Point_of_space> Vector_of_points;

    if (random_regime){
        // obtain a seed from the system clock:
        unsigned seed_coord = std::chrono::system_clock::now().time_since_epoch().count();
        //unsigned seedYcoord = std::chrono::system_clock::now().time_since_epoch().count()+3;
        unsigned seedPoissonDistribution = std::chrono::system_clock::now().time_since_epoch().count()+5;



        std::poisson_distribution<int> PoissonDistribution((int)(IntensityValue*4.0/3.1415926535897932384626433832795));
        //std::uniform_real_distribution <float> uniform_real_distributionX(0,BrushRadius);
        std::uniform_real_distribution <float> uniform_real_distribution_coord(0,BrushRadius);



        mt19937 mtPoissonDistribution(seedPoissonDistribution);
        int Nuber_of_points_in_square = PoissonDistribution(mtPoissonDistribution);



        // set the beginning number of points



        mt19937 mt(seed_coord);


        for(int i = 0;i<Nuber_of_points_in_square; i++){

            float X_probable_point = XCoord_Widjet + uniform_real_distribution_coord(mt) - BrushRadius/2;
            float Y_probable_point = YCoord_Widjet + uniform_real_distribution_coord(mt) - BrushRadius/2;
            if ((X_probable_point<=512) and (X_probable_point>=0) and
                (Y_probable_point<=512) and (Y_probable_point>=0) and
                (((X_probable_point-XCoord_Widjet)*(X_probable_point-XCoord_Widjet)+
                    (Y_probable_point-YCoord_Widjet)*(Y_probable_point-YCoord_Widjet))<=
                        BrushRadius*BrushRadius/4)

                ){
                    simbad::gui::Point_of_space CurrentPoint;
//+++
//            CurrentPoint.setXcoordinate((float)(X_probable_point)*1.0/512.0 - 0.5);
//            CurrentPoint.setYcoordinate((float)(Y_probable_point)*1.0/512.0 - 0.5);
//+++
                    CurrentPoint.Xcoordinate=X_probable_point*1.0/512.0 - 0.5;
                    CurrentPoint.Ycoordinate=-Y_probable_point*1.0/512.0 + 0.5;
                    Vector_of_points.push_back(CurrentPoint);
                    j=j+1;
                };
        };
    } else {
        int k;
        for (int i=1; i<25 ; i++) if (((i*i)<=IntensityValue) && (((i+1)*(i+1))>IntensityValue)) k=i;
        k=int(k/2.0) + 1;
        for (int i=(-k+1); i<k;   i++)
            for(int m=(-k+1); m<k; m++){
                float X_probable_point = XCoord_Widjet + i*(int(BrushRadius/(k*1.0))+1);
                float Y_probable_point = YCoord_Widjet + m*(int(BrushRadius/(k*1.0))+1);


                if ((X_probable_point<=512) and (X_probable_point>=0) and
                    (Y_probable_point<=512) and (Y_probable_point>=0) and
                    (((X_probable_point-XCoord_Widjet)*(X_probable_point-XCoord_Widjet)+
                        (Y_probable_point-YCoord_Widjet)*(Y_probable_point-YCoord_Widjet))<=
                            (int((BrushRadius*BrushRadius)/4.0)))

                    ){
                        simbad::gui::Point_of_space CurrentPoint;
    //+++
    //            CurrentPoint.setXcoordinate((float)(X_probable_point)*1.0/512.0 - 0.5);
    //            CurrentPoint.setYcoordinate((float)(Y_probable_point)*1.0/512.0 - 0.5);
    //+++
                        CurrentPoint.Xcoordinate=X_probable_point*1.0/512.0 - 0.5;
                        CurrentPoint.Ycoordinate=-Y_probable_point*1.0/512.0 + 0.5;
                        Vector_of_points.push_back(CurrentPoint);
                        j=j+1;
                    };

            }



    };
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    if (j-1 >= 0) Big_model->ModelPoints.Vector_of_types[Current_type].
                 Changing_of_color_and_points(j, Current_color_for_init_conf,
                                         Current_color_for_dynam_conf,
                                         Vector_of_points);









    update();

}

void GLWidjet_for_init_model::setCurrent_color_for_init_conf(QColor Color)
{
    Current_color_for_init_conf= Color;
}
void GLWidjet_for_init_model::setCurrent_color_for_dynam_conf(QColor Color)
{
    Current_color_for_dynam_conf = Color;

}

