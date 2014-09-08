#include "mixer_main.h"
#include "ui_mixer_main.h"
#include <QString>
#include <QTabWidget>
#include <mixer.h>
#include <volumelevel.h>
#include <iostream>
#include <VolumeLevelintermixer.h>
#include <controller.h>
#include <interface.h>
#include <QLabel>
#include <libusb.h>
#include <usb_mixer.h>
#include <qfingerscrollarea.h>
#include <QScrollBar>
#include <QDesktopWidget>
#include <QVBoxLayout>
#include <QImage>
#include <QScreen>

mixer_main::mixer_main(QScreen *screen) :QWidget()
{
    //Palette Things
    QPalette pal(palette());
    pal.setColor(QPalette::Background,Qt::white);
    this->setPalette(pal);
    
    //Controller Initialization and Device Connection
    int ex = controller.initController();
    QLabel *testthing = new QLabel();
    testthing->setText(QString::number(ex));
    testthing->setFixedSize(1000,1000);
    testthing->show();
    //Program Start
    this->constructWindow(screen);
}

mixer_main::~mixer_main()
{
    controller.deinitController();
}

void mixer_main::constructWindow(QScreen *screen)
{
    Controller *contP = &controller;
    //QDesktopWidget *desktop = new QDesktopWidget();
    //QRect screen = desktop->availableGeometry(this);


    int sWidth =screen->size().width();
    int sHeight =screen->size().height();


    this->setMaximumSize(sWidth, sHeight);
    this->setMinimumSize(sWidth,sHeight);
    QVBoxLayout *fullLayout = new QVBoxLayout(this);

    //Tab Object
    QTabWidget *tabs = new QTabWidget(this);
    int tHeight = sHeight;
    tabs->setMinimumSize(sWidth, tHeight);
    tabs->setMaximumSize(sWidth, tHeight);
    tabs->setStyleSheet("QTabBar::tab { background: white; border: 2px solid black; } QTabBar::tab:selected { background: qlineargradient(x1: 0, y1: 1, x2: 0, y2: 0,"
                        "stop: 0 #FF0000, stop: 0.8 #FFFFFF); } QTabWidget::pane {border: 10px solid black; }");

    for(int i = 0; i < 8;i++)
    {
        QHBoxLayout *testLayout = new QHBoxLayout();
        QWidget *main1 = new QWidget();
        Mixer *mix = new Mixer(i, 20 + i, contP, i + 1);

        main1->setLayout(testLayout);
        testLayout->addWidget(mix);
        tabs->addTab(main1, "Mix " + QString::number(i + 1));
    }

    QWidget *logo = new QWidget(this);
    logo->setStyleSheet("image: url(:/new/prefix1/Logo.png);");
    logo->move(sWidth - logo->width() - 10, sHeight - logo->height() - 5);

}






