#include <QApplication>
#include <QMessageBox>
#include <stdio.h>

int main(int argc, char** argv)
{
    int button;
    QApplication app(argc, argv);
    
    button = QMessageBox::question(0, "Question", "Message", QMessageBox::Yes,
                                       QMessageBox::No, QMessageBox::Cancel);
    switch(button) {
        case QMessageBox::Yes:
            printf("Yes\n");
            app.quit();
            break;
        case QMessageBox::No:
            printf("No\n");
            break;
        case QMessageBox::Cancel:
            printf("Cancel\n");
            break;
    };
    
    return app.exec();
}
