#include <QLabel> 			/* Qt의 클래스들을 위한 헤더 파일 */
#include <QTimer>
#include <QTextBrowser>
#include <QPushButton>
#include <QBoxLayout>
#include <QFileDialog>
#include <QImage>

#include "widget.h"

Widget::Widget(QWidget *parent) : QWidget(parent)
{
    m_imageDisp = new QLabel(this); 			/* 이미지 표시를 위한 QLabel 객체 */
    m_imageDisp->setMinimumSize(320, 240);
    m_resultDisp = new QTextBrowser(this); 		/* 이미지 분류 결과 표시를 위한 위젯 */

    /* 이미지 분류를 위한 버튼 설정 */
    QPushButton* button = new QPushButton("Check", this);
    connect(button, SIGNAL(clicked()), SLOT(checkImage()));

    /* 카메라 사용을 위한 버튼 설정 */
    QPushButton* cameraButton = new QPushButton("Camera", this);
    cameraButton->setCheckable(true);
    connect(cameraButton, SIGNAL(toggled(bool)), SLOT(camera(bool)));

    /* 이미지 파일을 불러오기 위한 버튼 설정 */
    QPushButton* fileButton = new QPushButton("Image File", this);
    connect(fileButton, SIGNAL(clicked()), SLOT(openFile()));

    /* 위젯 배치를 위한 QVBoxLayout 클래스 사용 */
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(m_imageDisp);
    layout->addWidget(m_resultDisp);
    layout->addWidget(button);
    layout->addWidget(cameraButton);
    layout->addWidget(fileButton);

    /* 캡처한 카메라를 지정 */
    m_capture = cv::VideoCapture(0);
    if(!m_capture.isOpened()) {
        perror("OpenCV : open WebCam\n");
        return;
    }

    /* 캡처할 영상의 속성을 설정 */
    m_capture.set(CV_CAP_PROP_FRAME_WIDTH, width());
    m_capture.set(CV_CAP_PROP_FRAME_HEIGHT, width());

    /* 연속적인 이미지 캡쳐를 위한 타이머 설정 */
    m_cameraTimer = new QTimer(this);
    connect(m_cameraTimer, SIGNAL(timeout()), SLOT(capture()));
    m_classifier = Classifier(); 		/* 이미지 분류를 위한 Classifier 클래스의 초기화 */
}

Widget::~Widget()
{
    m_capture.release(); 			/* 카메라 해제 */
}

void Widget::camera(bool flag) 			/* 카메라 사용을 위한 메소드 */
{
    (flag)?startCamera():stopCamera();
}

void Widget::startCamera() 			/* 카메라 시작을 위한 메소드 */
{
    if(!m_cameraTimer->isActive()) m_cameraTimer->start(1000/30);
}

void Widget::stopCamera() 			/* 카메라 종료를 위한 메소드 */
{
    if(m_cameraTimer->isActive()) m_cameraTimer->stop();
}

void Widget::capture()
{
    m_capture >> m_defaultBuffer; 		/* 카메라로부터 한 프레임의 영상을 가져옴 */
    QImage img = Mat2QImage(m_defaultBuffer); 	/* 이미지 포맷 변경 */
    m_imageDisp->setPixmap(QPixmap::fromImage(img)); 	/* 이미지를 QLabel 위젯에 표시 */
}

/* OpenCV의 Mat 클래스를 QImage 클래스로 변환 */
QImage Widget::Mat2QImage(cv::Mat const& src)
{
    QImage img = QImage((uchar*) src.data, src.cols, src.rows, src.step, QImage::Format_RGB888);
    QImage dest = img.rgbSwapped(); 		/* BGR 포맷을 RGB 포맷으로 변환 */

    return dest;
}

/* Qt의 QImage 클래스를 OpenCV의 Mat 클래스로 변환 */
cv::Mat Widget::QImage2Mat(QImage const& src)
{
    QImage temp = src.rgbSwapped().copy();
    cv::Mat res(temp.height(), temp.width(), CV_8UC3, (uchar*)temp.bits(), temp.bytesPerLine());
    return res;
}

void Widget::openFile() 			/* 파일 불러오기 */
{
    QString filename = QFileDialog::getOpenFileName(this, tr("Open Image"), ".", tr("Image Files (*.png *.jpg *.bmp)"));
    m_defaultBuffer = cv::imread(filename.toUtf8().data(), -1);
    if(!m_defaultBuffer.empty()) {
        QImage img = Mat2QImage(m_defaultBuffer);
        m_imageDisp->setPixmap(QPixmap::fromImage(img).scaled(m_imageDisp->size()));
    }
}

/* 이미지 분류를 위한 메소드 */
void Widget::checkImage()
{
    stopCamera(); 				/* 카메라 정지 */
    QString qstr = QString::fromStdString(m_classifier.checkImage(m_defaultBuffer));
    m_resultDisp->setText(qstr); 		/* 결과 표시 */
}
