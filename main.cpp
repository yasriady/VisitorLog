#include "mainwindow.h"
#include <QApplication>
#include "formoperation.h"
#include "runguard.h"

int main(int argc, char *argv[])
{
    RunGuard guard(APPKEY);
    if (!guard.tryToRun())
        return 0;

    QApplication a(argc, argv);
    // MainWindow w;
    FormOperation f;
    // w.show();
    f.show();

    return a.exec();
}
