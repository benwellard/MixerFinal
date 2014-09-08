#ifndef VOLUMELEVEL_H
#define VOLUMELEVEL_H

#include <QWidget>
#include <QRect>
#include <QTimer>
#include <VolumeLevelintermixer.h>
#include <QGroupBox>
#include <QPainter>
#include <QList>
#include <interface.h>

class VolumeLevel : public QGroupBox
{
    Q_OBJECT

public:
    explicit VolumeLevel(Interface *interfacevl, int channels, QWidget *parent = 0);
    void paintEvent(QPaintEvent *event);
    void startTimer();
    int noChannels;

signals:

private:

    int x,y,h;
    QTimer *refresh;
    unsigned short hArray;
    Interface *interface;
    QRect vol;
    QPainter p;
    VolumeLevelInterMixer *vlim;

public slots:
    void setInputLevels();
};

#endif // VOLUMELEVEL_H
