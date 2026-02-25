#include "ChartDemoWindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    ChartDemoWindow mainWindow;
    mainWindow.show();

    return app.exec();
}
