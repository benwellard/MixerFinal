#include "interface.h"
#include <iostream>

Interface::Interface(int outchannel, int mixChannel, Controller *controller, int mixRefer)
    :QObject()
{
    outputChannel = outchannel;
    prevChannel = 0;
    prevChannel2 = 0;
    mixerChannel = mixChannel;
    controlleri = controller;
    mixRef = mixRefer;
}

unsigned short* Interface::getLevels(int inout)
{
    levelThings = controlleri->getLevels(inout);

    return levelThings;
}

void Interface::ChangeVolume(int channel, int volume)
{

    //Calculates node from output(mixer) channel and input channel
    int node = outputChannel + (channel * 8);

    //If the slider for an input channel is below 1, it sets the relevant volume level to -127
    //Otherwise sets the node to whatever value it is passed in volume
    if(volume < 1)
        volume = -127;

    controlleri->changeVolume(node, -100 + volume);


}

void Interface::addChannel(int channel)
{
    //Calculate node, and retrieve volume level from device
    int node = (channel * 8) + outputChannel;
    controlleri->changeVolume(node, controlleri->getValue(node));
    //controlleri->changeVolume(node + 1, controlleri->getValue(node));
}

void Interface::setOutputChannel(int channel)
{
    controlleri->mapChannelToMix(channel,mixerChannel);
}

void Interface::setChannel(int index, int channel)
{
    channels.removeAt(index);
    channels.insert(index, channel);
    controlleri->mapMixInput(0,index,channel, mixRef);
}

int Interface::getValue(int channel)
{
    int node = (channel * 8) + outputChannel;
    return controlleri->getValue(node);
}

unsigned char Interface::getInputMap(int channel)
{
    return controlleri->getMixerMap(channel, mixRef);
}

void Interface::updateChannelMap(int channel)
{
    channels.append(channel);
}

int Interface::readChannelMap(int index)
{
   return channels.at(index);
}

