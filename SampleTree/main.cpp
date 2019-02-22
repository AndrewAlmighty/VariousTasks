#include "mainwindow.h"
#include "nodetester.h"

#include <QApplication>
#include <iostream>
#include <memory>

int main(int argc, char *argv[])
{
    {
        NodeTester tester;
        if (!tester.runTests())
        {
            std::cout << "Tests didn't pass, terminating";
            return 0;
        }
    }

    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
