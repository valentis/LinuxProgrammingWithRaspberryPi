#include <QApplication>
#include <QLineEdit>

int main(int argc, char **argv)
{
    QApplication app(argc, argv);
    
    QLineEdit *LE = new QLineEdit(0);		/* 라인 에디트 객체 생성 */
    LE->setEchoMode(QLineEdit::Password);	/* 에코 모드 설정 */
    LE->show();
    
    return app.exec();
}
