#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    // Initialize the QApplication
    QApplication a(argc, argv);

    // Create and show the main window
    MainWindow w;
    w.show();

    // Execute the application event loop
    return a.exec();
}
