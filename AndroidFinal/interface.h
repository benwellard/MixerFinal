#ifndef INTERFACE_H
#define INTERFACE_H

#include <QObject>
#include <controller.h>
#include <QList>

class Interface : public QObject
{
    Q_OBJECT
public:






    //OutputChannel = Which mixer output Channel this Interface is registered to
    int outputChannel;

    //mixChannel = Which mix Channel this interface is registered to
    int mixerChannel;

    //mixRefer = which mix object on the device this interface points to
    int mixRef;

    //controller = a pointer to the one controller object generated in mixer_main
    Controller *controlleri;

    Interface(int outputchannel, int mixChannel, Controller *controller, int mixRefer);


    void addChannel(int channel);

    //Gets volume value of a channel
    int getValue(int channel);

    //gets the volume levels
    unsigned short* getLevels(int inout);


    QList <int> channels;
    unsigned char getInputMap(int channel);
    void updateChannelMap(int channel);
    int readChannelMap(int index);


public slots:
    void ChangeVolume(int channel, int volume);
    void setOutputChannel(int channel);
    void setChannel(int index, int channel);
private:
    void deletePreviousMapping();
    unsigned short * levelThings;
    int numOutputs;
};

#endif // INTERFACE_H
