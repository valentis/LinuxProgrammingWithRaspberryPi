#include <QApplication>
#include <QLCDNumber>

int main(int argc, char **argv)
{
    QApplication app(argc, argv);
    
    QWidget* widget = new QWidget(0);
    
    QLCDNumber *lcd1 = new QLCDNumber(widget);   	/* LCD 넘버 객체 생성 */
    lcd1->setSegmentStyle(QLCDNumber::Flat);		/* 세그먼트 스타일 설정 */
    lcd1->display(2014);				/* 표시할 값 설정 */
    lcd1->move(10, 10);
    
    QLCDNumber *lcd2 = new QLCDNumber(widget);
    lcd2->setSegmentStyle(QLCDNumber::Flat);
    lcd2->display(2014);
    lcd2->setHexMode();
    lcd2->move(10, 40);
    
    widget->resize(120, 80);
    widget->show();
    
    return app.exec();
}
