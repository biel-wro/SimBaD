#ifndef FRAME_FOR_STATISTICS_H
#define FRAME_FOR_STATISTICS_H

#include <QFrame>

namespace Ui {
class Frame_for_statistics;
}

class Frame_for_statistics : public QFrame
{
    Q_OBJECT

public:
    explicit Frame_for_statistics(QWidget *parent = 0);
    ~Frame_for_statistics();

private:
    Ui::Frame_for_statistics *ui;
};

#endif // FRAME_FOR_STATISTICS_H
