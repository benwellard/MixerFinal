#include "volumelevel.h"
#include <QPainter>
#include <QRect>
#include <iostream>
#include <QPaintEvent>
#include <decibelworker.h>
#include <controller.h>
#include <QThread>
//#include <VolumeLevelintermixer.h>
#include <QStyleOption>


VolumeLevel::VolumeLevel(Interface * interfacevl, int channels, QWidget *parent) : QGroupBox(parent)
{
    interface = interfacevl;
    noChannels = channels;
    x = 30;
    h = 200;
    vlim = new VolumeLevelInterMixer(interfacevl);
    refresh = new QTimer();
    connect(refresh,SIGNAL(timeout()),this,SLOT(setInputLevels()));

}


void VolumeLevel::startTimer()
{
    refresh->start(25);
}

void VolumeLevel::paintEvent(QPaintEvent *event)
{
    p.begin(this);
    p.setRenderHint(QPainter::Antialiasing);
    QStyleOption opt;
    opt.init(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
    QColor myPenColor = QColor(255,255,255,255);
    QBrush *brush = new QBrush(myPenColor,Qt::SolidPattern);
    p.setPen(myPenColor);
    p.setBrush(*brush);


    for(int i = 0;i < noChannels; i++)
    {
        x = - 65 - (i * 177);
        p.rotate(180);
        hArray = vlim->getLevels(interface->readChannelMap(i));
        unsigned short larry = hArray;
        if(larry < 1)
            larry = 1;
        QRect *vol = new QRect(x - 25 ,-250,30,(larry)/400);
        p.drawRect(*vol);
        p.rotate(180);
    }

    p.end();
}

void VolumeLevel::setInputLevels()
{
    update();
}




