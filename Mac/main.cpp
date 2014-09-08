#include "mixer_main.h"
#include <QApplication>
#include <usb_mixer.h>
#include <controller.h>
#include <QProxyStyle>
#include <qscreen.h>

class MyProxyStyle : public QProxyStyle
{
  public:
    int pixelMetric ( PixelMetric metric, const QStyleOption * option = 0, const QWidget * widget = 0 ) const
    {
        switch(metric) {
          case PM_SliderLength            : return 500;
          case PM_SliderThickness         : return 500;
          default                         : return (QProxyStyle::pixelMetric(metric,option,widget));
        }
    }
};



int main(int argc, char *argv[])
{
    QApplication a(argc, argv);




    QScreen *screen = a.screens().at(0);
    MyProxyStyle *sliderStyle = new MyProxyStyle();
    a.setStyle(sliderStyle);
    mixer_main *w = new mixer_main(screen);
    w->show();



    return a.exec();


}
