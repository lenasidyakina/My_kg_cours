#include <QApplication>
#include "MainWindow.h"


int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    MainWindow mainWin;
    mainWin.show();
    return QApplication::exec();
}
