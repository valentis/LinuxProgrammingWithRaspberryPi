#include <QApplication>
#include <QLabel>
#include <QPixmap>

int main(int argc, char **argv)
{
    QApplication app(argc, argv);

    QLabel* lb = new QLabel("", 0);
    lb->setPixmap(QPixmap("mandrill.jpg"));
    lb->show();

    return app.exec();
}
