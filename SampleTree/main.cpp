#include "mainwindow.h"
#include <QApplication>
#include <iostream>

#include "treenode.h"

int main(int argc, char *argv[])
{
    TreeNode <std::string> drzewo("asdf", "moje");
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
