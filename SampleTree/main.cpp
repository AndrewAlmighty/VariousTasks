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
    w.acceptSerializedTree("N:GrandParent(1)P:NULL,CH:N_1(1)N_2(2)N_3(3);N:N_1(1)P:GrandParent(1),CH:N_4(4)N_5(5);N:N_4(4)P:N_1(1),CH:N_7(7);N:N_7(7)P:N_4(4),CH:NULL;N:N_5(5)P:N_1(1),CH:NULL;N:N_2(2)P:GrandParent(1),CH:N_6(6);N:N_6(6)P:N_2(2),CH:NULL;N:N_3(3)P:GrandParent(1),CH:NULL;");
    w.show();
    return a.exec();
}
