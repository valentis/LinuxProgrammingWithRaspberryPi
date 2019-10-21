#include <QApplication>
#include <QTextBrowser>
#include <QUrl>

int main(int argc, char **argv)
{
    QApplication app(argc, argv);
    QUrl url("index.html");
    QTextBrowser *tb = new QTextBrowser();		/* 텍스트 브라우저 객체 생성 */
    
    tb->setSource(url);
    tb->show();
    
    return app.exec();
}
