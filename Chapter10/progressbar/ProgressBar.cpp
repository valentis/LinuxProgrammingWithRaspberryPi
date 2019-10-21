#include <QApplication>
#include <QProgressBar>

int main(int argc, char **argv)
{
    QApplication app(argc, argv);
    
    QProgressBar *pb = new QProgressBar();		/* 프로그래스바 객체 생성 */
    pb->setRange(0, 60);
    pb->setValue(20);
    pb->show();
    
    return app.exec();
}
