#include <QApplication>
#include <QWidget>
#include <QRadioButton>
#include <QButtonGroup>
#include <QVBoxLayout>

int main(int argc, char** argv)
{
    QApplication app(argc, argv);
    
    QWidget *widget = new QWidget(0);
    
    QButtonGroup *bg = new QButtonGroup(widget);
    
    QVBoxLayout *vbl = new QVBoxLayout;		/* QVBoxLayout 객체 생성 */
    vbl->setSpacing(6);				/* 레이아웃의 간격 설정 */
    vbl->setMargin(3);				/* 레이아웃의 마진 설정 */
    
    QRadioButton *rb1 = new QRadioButton("Option 1", widget);
    bg->addButton(rb1);
    vbl->addWidget(rb1);
    
    QRadioButton *rb2 = new QRadioButton("Option 2", widget);
    bg->addButton(rb2);
    vbl->addWidget(rb2);
    
    QRadioButton *rb3 = new QRadioButton("Option 3", widget);
    bg->addButton(rb3);
    vbl->addWidget(rb3);
    
    QRadioButton *rb4 = new QRadioButton("Option 4", widget);
    bg->addButton(rb4);
    vbl->addWidget(rb4);
    
    widget->setLayout(vbl);			/* 위젯의 레이아웃으로 설정 */
    widget->show();
    
    return app.exec();
}
