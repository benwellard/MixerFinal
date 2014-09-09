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

mixer_main::mixer_main() :QWidget()
{
    //Palette Things
    QPalette pal(palette());
    pal.setColor(QPalette::Background,Qt::white);
    this->setPalette(pal);
    
    //Controller Initialization and Device Connection
    int ex = controller.initController();
    //Program Start
    this->constructWindow();
}

mixer_main::~mixer_main()
{
    controller.deinitController();
}

void mixer_main::constructWindow()
{
    Controller *contP = &controller;
    QDesktopWidget *desktop = new QDesktopWidget();
    QRect screen = desktop->availableGeometry(this);
    this->setMaximumSize(screen.width()-50,screen.height()-50);
    this->setMinimumSize(screen.width()-50, screen.height() - 50);
    QVBoxLayout *fullLayout = new QVBoxLayout(this);

    //Tab Object
    QTabWidget *tabs = new QTabWidget(this);
    tabs->setMinimumSize(screen.width()-50, screen.height()-100);
    tabs->setMaximumSize(screen.width()-50,screen.height() - 100);
    tabs->setStyleSheet("QTabBar::tab { background: white; border: 2px solid black; } QTabBar::tab:selected { background: qlineargradient(x1: 0, y1: 1, x2: 0, y2: 0,"
                        "stop: 0 #FF0000, stop: 0.8 #FFFFFF); } QTabWidget::pane {border: 10px solid black; }");

    for(int i = 0; i < usb_mixer_get_num_outputs(0);i++)
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
    logo->move((screen.width()- 50) - logo->width() - 10, (screen.height()-50) - logo->height() - 10);

}






