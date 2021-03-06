#ifndef MIXER_H
#define MIXER_H

#include <QWidget>
#include <QHBoxLayout>
#include <QComboBox>
#include <volslider.h>
#include <QList>
#include <interface.h>
#include <controller.h>
#include <QScrollArea>
#include <QPalette>
#include <QPushButton>
#include <channelbox.h>
#include <QGroupBox>
#include <QCheckBox>
#include <QDialog>
#include <outputbox.h>
#include <volumelevel.h>

class Mixer : public QWidget
{
    Q_OBJECT
public:
    explicit Mixer(int output, int mixerOut, Controller *controller, int mixRef);
    int mixReference;
signals:
    void emitVolume(int vol, int indexNo);
    void emitChannels(int channels);
    void emitCurrentOutput(int output);

public slots:

    //Given a channel, will adjust it to whatever is specified in vol
    void changeVolume(int indexVal, int vol);


    void setChannels(int n);
    void addChannel();
    void newSlider();
    void muteChannelController(int channel);
    void unMuteChannelController(int channel, int volume);
    void setOutputs();
    void populateOutputs();
    void dePopulateOutputs();
    void deMapChannel(int index);

private:
    QPushButton *hideOutputs;
    QVBoxLayout *outputLayout;
    QList<VolSlider *> sliders;
    QList<ChannelBox *> selectors;
    QList<QPushButton *> mutes;
    QList<OutputBox *> outputs;
    QList<QGroupBox *> sliderBoxes;
    QDialog *outputFrame;
    QPalette p;
    int selectedChannel;
    int indexNo;
    int numOuts;
    int numIns;
    int mixChannel;
    QHBoxLayout *sliderLayout;
    QVBoxLayout *mainLayout;
    QWidget *channels;
    QWidget *volumes;
    QComboBox *addChannelBox();
    void constructMixer(int output);
    QComboBox *addOutputBox();
    Interface *interface;
    void constructOutputs();
    QList<int> volArray;
    bool mute;
    Controller *controllerp;
    QScrollArea *area;
    VolumeLevel *volumeLevel;
    QCheckBox *output12, *output34, *output56, *output78;
};

#endif // MIXER_H
