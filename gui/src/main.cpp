#include "core.h"
#include <boost/program_options.hpp>
//#include <QtCore>
//#include <QtOpenGL>
#include "mainwindow.h"
#include <QApplication>


int main(int argc, char** argv)
{
    hello();
    QApplication app(argc,argv);
    simbad::gui::MainWindow mw;
    mw.show();
    app.exec();

}
