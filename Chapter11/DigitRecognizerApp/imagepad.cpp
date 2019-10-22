#include <QPainter>
#include <QMouseEvent>

#include "imagepad.h"

#define MARGIN 80

ImagePad::ImagePad(QWidget *parent) : 
      QWidget(parent), m_pen{ Qt::black, 15, Qt::SolidLine, Qt::RoundCap }, m_bStart(true)
{
    setMinimumSize(256, 256);
    clearWidget();
}

/* 현재 위젯을 지움 */
void ImagePad::clearWidget() 
{
    m_defaultBuffer = QImage(256, 256, QImage::Format_RGB888);
    m_defaultBuffer.fill(QColor(255, 255, 255));
    m_bStart = true;
    update();
}

/* 위젯에 QImage 객체를 표시 */
void ImagePad::paintEvent(QPaintEvent *ev) 
{
    QPainter painter;
    painter.begin(this);
    painter.drawImage(0, 0, m_defaultBuffer);
    painter.end();
    QWidget::paintEvent(ev);
}

/* QImage 객체를 반환 */
const QImage ImagePad::buffer()
{
    return m_defaultBuffer;
}

/* 마우스 버튼이 눌리면 자동으로 호출됨 */
void ImagePad::mousePressEvent(QMouseEvent *ev) 
{
    m_prevPoint = ev->pos(); 		/* 선을 그릴 수 있도록 현재점의 위치를 저장 */

    if(m_bStart) clearWidget(); 	/* 플래그가 설정되어 있으면 지우고 플래그를 초기화 */
    m_bStart = false; 			/* 다시 지우지 않도록 초기화 */

    QPainter painter(&m_defaultBuffer); /* QImage 객체에 점을 그림 */
    painter.setPen(m_pen);
    painter.drawPoint(m_prevPoint);

    /* 위젯의 원하는 부분만 갱신 */
    update(ev->pos().x()-MARGIN/2, ev->pos().y()-MARGIN/2, MARGIN, MARGIN);

    QWidget::mousePressEvent(ev);
}

/* 마우스 커서가 이동할 때 불림 */
void ImagePad::mouseMoveEvent(QMouseEvent *ev) 
{
    int x1, x2, y1, y2;
    QPainter painter(&m_defaultBuffer); 	/* 이전점에서 현재점까지 선을 그림 */
    painter.setPen(m_pen);
    painter.drawLine(m_prevPoint, ev->pos());

    if(ev->pos().x() < m_prevPoint.x()) {
        x1 = ev->pos().x();
        x2 = m_prevPoint.x();
    } else {
        x2 = ev->pos().x();
        x1 = m_prevPoint.x();
    }

    if(ev->pos().y() < m_prevPoint.y()) {
        y1 = ev->pos().y();
        y2 = m_prevPoint.y();
    } else {
        y2 = ev->pos().y();
        y1 = m_prevPoint.y();
    }

    /* 그려진 범위의 화면을 갱신, 커서가 빨리 움직이는 경우를 위해 마진을 사용 */
    QRect rect(x1, y1, x2, y2);
    update(rect.adjusted(-MARGIN, -MARGIN, MARGIN, MARGIN));
    m_prevPoint = ev->pos();

    QWidget::mouseMoveEvent(ev);
}

/* 마우스 버튼을 띄는 경우 자동으로 호출됨 */
void ImagePad::mouseReleaseEvent(QMouseEvent *ev) 
{
    m_prevPoint = QPoint();

    QPainter painter(&m_defaultBuffer);
    painter.setPen(m_pen);
    update();

    QWidget::mouseReleaseEvent(ev);
}

/* 저수준 이미지 데이터 반환 */
unsigned char* ImagePad::imageData()
{
    return m_defaultBuffer.bits();
}

/* 현재 위젯에 이미지 설정 */
void ImagePad::setPixmap(QPixmap pixmap)
{
    m_defaultBuffer = pixmap.toImage();
    update();
    m_bStart = true;
}

/* 다음에 그릴 때 위젯을 지울지 플래그를 설정 */
void ImagePad::setFlag(bool flag)
{
    m_bStart = flag;
}
