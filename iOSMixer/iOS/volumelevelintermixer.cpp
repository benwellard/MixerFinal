#include "VolumeLevelintermixer.h"
#include <controller.h>
#include <iostream>
#include <QTimer>
#include <controller.h>
#include <interface.h>
#include <QtConcurrent/QtConcurrent>
#include <QtConcurrent/QtConcurrentRun>
#include <QFuture>

VolumeLevelInterMixer::VolumeLevelInterMixer(Interface *interface,QObject *parent) :
    QObject(parent)
{
    interfacev = interface;
}
unsigned short  VolumeLevelInterMixer::getLevels(int channel)
{
    levelDataX = interfacev->getLevels(0);
    return levelDataX[channel];
}

