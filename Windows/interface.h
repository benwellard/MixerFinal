#ifndef INTERFACE_H
#define INTERFACE_H

#include <QObject>
#include <controller.h>
#include <QList>

class Interface : public QObject
{
    Q_OBJECT
public:
    Interface(int outputchannel, int mixChannel, Controller *controller, int mixRefer);
    void addChannel(int channel);
    int getValue(int channel);
    unsigned short* getLevels(int inout);
    int outputChannel;
    int mixerChannel;
    int prevChannel;
    int prevChannel2;
    int mixRef;
    Controller *controlleri;
    QList <int> channels;
    unsigned char getInputMap(int channel);
    void updateChannelMap(int channel);
    int readChannelMap(int index);
    void incChannels();
    void decChannels();

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
