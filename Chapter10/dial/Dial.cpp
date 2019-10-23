#include <QApplication>
#include <QDial>

int main(int argc, char **argv)
{
    QApplication app(argc, argv);
    
    QDial *dl = new QDial();		/* 다이얼 객체 생성 */
    dl->setNotchesVisible(true);	/* 노치 표시 설정 */
    dl->setNotchTarget(10);		/* 노치 타겟 설정 */
    dl->setWrapping(true);		/* 원형 순환 설정 */
    dl->show();
    
    return app.exec();
}
