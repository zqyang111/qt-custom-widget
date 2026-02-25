#include "ComponentDemoWindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    
    ComponentDemoWindow window;
    window.show();
    
    return app.exec();
}








