#include <QApplication>
#include <QLabel>

int main(int argc, char **argv)
{
    QApplication app(argc, argv);

    QLabel *hello = new QLabel("<font color=blue>Hello <i>World!</i></font>", 0);
    hello->show(); 		/* 위젯의 화면 표시 */

    return app.exec();          /* Qt의 이벤트 루프 시작 */
}
