#include <QMainWindow>
#include <QApplication>
#include "../ui_builder.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QMainWindow w;
    UiBuilder uiBuilder(&w);

    w.setWindowTitle("UiBuilder test");

    uiBuilder.init();

    uiBuilder.executeFile("src/uibuilder/test/test.lua");

    w.show();

    return a.exec();
}
