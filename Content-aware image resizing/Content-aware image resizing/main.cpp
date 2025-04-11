#include <qapplication.h>

#include "mainwindow.h"
#include "translate.h"

int main(int argc, char* argv[])
{
    QApplication a(argc, argv);

    setLanguage(currentLanguage());

    MainWindow w;
    w.show();

    return a.exec();
}
