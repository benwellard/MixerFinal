#include "masterslider.h"
#include <QStyleOption>


MasterSlider::MasterSlider(int outs, Controller *controller, QWidget *parent) :
    QGroupBox(parent)
{
    this->setStyleSheet("background-color: white; border: 5px solid red; border-radius: 10px;");
    numOuts = outs;
    controllerm = controller;
    refresh = new QTimer();
    connect(refresh,SIGNAL(timeout()),this,SLOT(setOutputLevels()));
}


void MasterSlider::startRefresh()
{
    refresh->start(100);
}

void MasterSlider::setOutputLevels()
{
    update();
}

void MasterSlider::paintEvent(QPaintEvent *event)
{
    QPainter p(this);
    p.setPen(Qt::black);
    p.rotate(180);
    p.setRenderHint(QPainter::Antialiasing);
    QStyleOption opt;
    opt.init(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
    QColor myPenColor = QColor(0,0,0,255);
    QBrush *brush = new QBrush(myPenColor,Qt::SolidPattern);
    p.setPen(myPenColor);
    p.setBrush(*brush);

    unsigned short * levels = controllerm->getLevels(1);
    y = -300;
    int g = 0;
    for(int i = 0;i < numOuts;i++)
    {
        if(i == 4)
        {
            g = 0;
            y = -600;
        }
        x = -40 * (g + 1);
        unsigned short lax = levels[i];
        volRect = new QRect(x,y,30,lax/400);

        p.drawRect(*volRect);
        p.rotate(180);

        QString text = "Mix: ";
        QString text2 = QString::number(i + 1);

        p.drawText(-x - 27,-y + 20, text.append(text2));
        p.rotate(180);
        g++;

    }
    p.rotate(180);
}
