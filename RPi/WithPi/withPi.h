#ifndef __CUSTOM_WIDGET__
#define __CUSTOM_WIDGET__

#include <QWidget>
class QLabel;
class QDial;

class PiWidget : public QWidget {
     Q_OBJECT
  public:
     PiWidget(QWidget *parent = 0);
  
  private:
      QLabel* imageLabel;
      QLabel* temperatureLabel;
      QLabel* humidityLabel;
      QLabel* pressureLabel;
      QDial* dial;
      int isOnOff;
    
  public slots:
      void ledOn();
      void ledOff();
      void setNum(int );
      void monitor();
};

#endif // __CUSTOM_WIDGET__
