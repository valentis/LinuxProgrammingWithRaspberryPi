#include <QApplication>
#include <QSpinBox>

int main(int argc, char **argv)
{
    QApplication app(argc, argv);
    
    QSpinBox *sp = new QSpinBox();		/* 스핀 박스 객체 생성 */
    sp->setRange(0, 10);			/* 전체 범위 설정 */
    sp->setValue(5);				/* 현재 값 설정 */
    sp->setWrapping(true);			/* 원형 순환 설정 */
    sp->setSpecialValueText("Value");		/* 특별 값 문자 설정 */
    sp->show();
    
    return app.exec();
}
