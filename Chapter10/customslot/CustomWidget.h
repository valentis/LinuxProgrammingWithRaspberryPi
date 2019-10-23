#ifndef __CUSTOM_WIDGET__
#define __CUSTOM_WIDGET__

#include <QWidget>

class CustomWidget : public QWidget {
        Q_OBJECT
  public:
      CustomWidget(QWidget *parent = 0);

  signals:				/* 사용자 정의 시그널 */
      void widgetClicked();

  public slots:				/* 사용자 정의 슬롯 */
      void processClick();
};

#endif // __CUSTOM_WIDGET__
