#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QObject>
#include <QMutex>

class Controller : public QObject
{
    Q_OBJECT
public:
    Controller();


    unsigned short *getLevels(int inout);
    void mapChannelToMix(int channel, int mixerChannel);
    void mapMixInput(int input, int mixerChannel, int channel, int mixRef);
    void changeVolume(int node, int volume);
    int getMap(int channel);
    unsigned char getMixerMap(int mixerChannel, int mixRef);
    int getValue(int node);
    int initController();
    void deinitController();
    unsigned char levels[64];
    bool isChannelFree(int channel);

private:

public slots:


signals:

};

#endif // CONTROLLER_H
