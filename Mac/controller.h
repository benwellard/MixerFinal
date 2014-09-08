#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QObject>
#include <QMutex>

class Controller : public QObject
{
    Q_OBJECT
public:
    Controller();

    //Returns levels as an unsigned short array. 0 = input levels, 1 = output levels
    unsigned short *getLevels(int inout);

    //Maps a mixer channel to an output channel
    void mapChannelToMix(int channel, int mixerChannel);

    //Maps an input to a Mixer. mixRef = mix object reference (1-8)
    void mapMixInput(int mixerChannel, int channel, int mixRef);

    //Changes a particular node's volume
    void changeVolume(int node, int volume);

    //Retrieves the mixer Channel going to specified output
    int getMap(int channel);

    //returns whatever input is assigned to slot specified in mixerChannel
    unsigned char getMixerMap(int mixerChannel, int mixRef);

    //Gets a node's volume level. Used on initialization to set sliders
    int getValue(int node);

    //calls to the mixer and connects or disconnects the controller
    int initController();
    void deinitController();

    //The array containing level information
    unsigned char levels[64];


private:
public slots:
signals:

};

#endif // CONTROLLER_H
