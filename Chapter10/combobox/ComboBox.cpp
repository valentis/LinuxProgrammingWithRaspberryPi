#include <QApplication>
#include <QComboBox>

int main(int argc, char **argv)
{
    QApplication app(argc, argv);
    
    QComboBox *cb = new QComboBox();		/* 콤보 박스 객체 생성 */
    cb->addItem("KDE");				/* 아이템 추가 */
    cb->addItem("GNOME");
    cb->addItem("FVWM");
    cb->addItem("CDE");
    cb->setEditable(true);			/* 편집 가능(옵션 추가) 설정 */
    cb->show();
    
    return app.exec();
}
