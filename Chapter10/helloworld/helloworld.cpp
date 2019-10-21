#include <QApplication>
#include <QLabel>

int main(int argc, char **argv)
{
    QApplication app(argc, argv);

    QLabel *hello = new QLabel("<font color=blue>Hello <i>World!</i></font>", 0);
    hello->resize(80, 35); 		/* 위젯의 크기 설정 */
    hello->move(300, 300);		/* 위젯의 위치 설정 */
    hello->show(); 			/* 위젯의 화면 표시 */

    return app.exec();			/* Qt의 이벤트 루프 시작 */
}
