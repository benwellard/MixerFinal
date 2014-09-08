#ifndef MASTERSLIDER_H
#define MASTERSLIDER_H

#include <QGroupBox>
#include <QPainter>
#include <QWidget>
#include <QRect>
#include <controller.h>
#include <QTimer>

class MasterSlider : public QGroupBox
{
    Q_OBJECT

public:
    explicit MasterSlider(int outs, Controller *controller, QWidget *parent = 0);
    void paintEvent(QPaintEvent *event);
    void startRefresh();
signals:

public slots:
    void setOutputLevels();
private:
    QRect *volRect;
    int numOuts;
    int level;
    Controller *controllerm;
    int y;
    int x;
    QTimer *refresh;
};

#endif // MASTERSLIDER_H
