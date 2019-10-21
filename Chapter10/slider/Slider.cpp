#include <QApplication>
#include <QSlider>

int main(int argc, char **argv)
{
    QApplication app(argc, argv);
    
    QSlider *sl = new QSlider(Qt::Horizontal, 0); 	/* 슬라이더 객체 생성 */
    sl->setTickPosition(QSlider::TicksAbove);		/* 틱 마크 표시 */
    sl->setRange(0, 100);				/* 전체 범위 설정 */
    sl->setValue(50);					/* 현재 값 설정 */
    sl->show();
    
    return app.exec();
}
