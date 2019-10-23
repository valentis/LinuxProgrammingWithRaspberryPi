#include <QLabel>
#include <QPushButton>
#include <QBoxLayout>
#include <opencv2/opencv.hpp>

#include "widget.h"

extern "C" int setLed(int num);

Widget::Widget(QWidget *parent) : QWidget(parent)
{
    m_resultLabel = new QLabel(this);

    QPushButton* checkButton = new QPushButton("Check", this);
    connect(checkButton, SIGNAL(clicked()), SLOT(checkImage()));

    QPushButton* cameraButton = new QPushButton("Camera", this);
    cameraButton->setCheckable(true);
    connect(cameraButton, SIGNAL(toggled(bool)), SLOT(camera(bool)));

    m_imagePad = new ImagePad(this);
    m_imagePad->resize(256, 256);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(m_imagePad);
    layout->addWidget(m_resultLabel);
    layout->addWidget(checkButton);
    layout->addWidget(cameraButton);

    /* 관련 파일 초기화 */
    /* 가중치 파일 */

    string model_file = "/home/pi/caffe/examples/mnist/lenet_iter_10000.caffemodel";
    /* 네트워크 파일 */

    string proto_file = "/home/pi/caffe/examples/mnist/lenet.prototxt";
    m_classifier = Classifier(model_file, proto_file); 		/* Classifier 클래스의 초기화 */
    resize(280, 300);
}

Widget::~Widget()
{
}

void Widget::checkImage()
{
    cv::Mat img; 		/* 이미지 데이터 */
    int resultNumber; 		/* 처리 결과 */

    /* 이미지 변환 */
    img = cv::Mat(m_imagePad->width(), m_imagePad->height(), CV_8UC3,
                  m_imagePad->buffer().rgbSwapped().bits(),
                  m_imagePad->buffer().bytesPerLine()).clone();
    

    resultNumber = m_classifier.predictNumber(img); 			/* 숫자 예측 */
    setLed(resultNumber); 						/* SenseHAT의 LED 매트릭스에 숫자 표시 */
    m_resultLabel->setText("number: " + QString::number(resultNumber)); /* 결과 표시 */
    m_imagePad->setFlag(true);
}

void Widget::camera(bool flag)
{
    (flag)?m_imagePad->startCamera():m_imagePad->stopCamera();
}
