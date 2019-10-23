#include <QApplication> 		/* QApplication 클래스를 위한 헤더 파일 */

#include "widget.h"

int main(int argc, char** argv)
{
    QApplication app(argc, argv);

    Widget w;
    w.show();

    return app.exec();
}
