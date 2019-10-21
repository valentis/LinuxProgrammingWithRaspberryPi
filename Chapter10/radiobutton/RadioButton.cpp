#include <QApplication>
#include <QWidget>
#include <QRadioButton>
#include <QButtonGroup>

int main(int argc, char** argv)
{
    QApplication app(argc, argv);

    QWidget *widget = new QWidget(0);
    QButtonGroup *bg = new QButtonGroup(widget);			/* 버튼 그룹 객체 생성 */
    
    QRadioButton *rb1 = new QRadioButton("Option &1", widget);		/* 라디오 버튼 객체 생성 */
    rb1->move(10, 10);
    bg->addButton(rb1);							/* 버튼 그룹에 추가 */
    
    QRadioButton *rb2 = new QRadioButton("Option &2", widget);
    rb2->move(10, 30);
    bg->addButton(rb2);
    
    QRadioButton *rb3 = new QRadioButton("Option &3", widget);
    rb3->move(10, 50);
    bg->addButton(rb3);
    
    QRadioButton *rb4 = new QRadioButton("Option &4", widget);
    rb4->move(10, 70);
    bg->addButton(rb4);
    
    widget->resize(140, 110);						/* 위젯의 크기 설정 */
    widget->show();							/* 위젯을 화면에 표시 */
    
    return app.exec();
}
