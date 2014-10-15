//#include "core.h"
//#include <boost/program_options.hpp>
//#include <QtCore>
//#include <QtOpenGL>
#include "mainwindow.h"
#include <QApplication>
#include <random>
using namespace std;

int main(int argc, char** argv)
{
  //  hello();
  //  random_device RandomSeed;
    QApplication app(argc,argv);
    simbad::gui::MainWindow mw;
  //  mw.RandomSeed=RandomSeed;

    mw.show();
    app.exec();

}
