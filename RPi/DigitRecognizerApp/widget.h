#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include "classification.h"

#include "imagepad.h"

class QLabel;

class Widget : public QWidget
{
      Q_OBJECT
  public:
      Widget(QWidget *parent = nullptr);
      ~Widget();

  private slots:
      void checkImage();
      void camera(bool);

  private:
      Classifier m_classifier; 		/* 분류를 위한 객체 */
      ImagePad* m_imagePad; 		/* 숫자 드로잉을 위한 객체 */
      QLabel* m_resultLabel; 		/* 인식된 숫자 표시를 위한 레이블 객체 */
};

#endif /* WIDGET_H */
