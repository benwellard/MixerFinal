#include "controller.h"
#include <QApplication>
#include <iostream>
#include <QFuture>
#include <QtConcurrent/QtConcurrent>
#include <QMutex>
#include<interface.h>






Controller::Controller()
    : QObject()
{

}

int Controller::initController()
{
    //Dud file descriptor. If using a custom library, retrieve the file descriptor from device and pass to fd
    int fd = 10;
    //int g = usb_mixer_connect(fd);
    return fd;
}

void Controller::deinitController()
{
    //int g = usb_mixer_disconnect();
}

unsigned short* Controller::getLevels(int inout)
{
    //int y = usb_mixer_mem_get(0,inout,levels);
    //unsigned short * levelData = (unsigned short *)levels;
    unsigned short * levelData = 0;
    return levelData;
}

void Controller::mapChannelToMix(int channel, int mixerChannel)
{
   //int b = usb_set_aud_channel_map(channel ,mixerChannel);
}

unsigned char Controller::getMixerMap(int mixerChannel, int mixRef)
{
    unsigned char g = 'g';
    return g;
    //return usb_mixsel_get_state(0, mixerChannel, mixRef);
}

void Controller::mapMixInput(int mixerChannel, int channel, int mixRef)
{
    //usb_mixsel_set_state(0, mixerChannel, channel, mixRef);
}

void Controller::changeVolume(int node, int volume)
{
    //int x = usb_mixer_set_value(0, node, volume);
}

int Controller::getMap(int channel)
{
    //int x = usb_get_aud_channel_map(channel);
    int x = 1;
    return x;
}

int Controller::getValue(int node)
{
    //int y = usb_mixer_get_value(0,node);
    int y = 5;
    return y;
}


