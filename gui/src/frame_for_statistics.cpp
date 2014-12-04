#include "frame_for_statistics.h"
#include "ui_frame_for_statistics.h"

Frame_for_statistics::Frame_for_statistics(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::Frame_for_statistics)
{
    ui->setupUi(this);
}

Frame_for_statistics::~Frame_for_statistics()
{
    delete ui;
}
