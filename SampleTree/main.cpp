#include "mainwindow.h"
#include <QApplication>
#include <iostream>

#include "treenode.h"

int main(int argc, char *argv[])
{
    TreeNode<int> a1(1);
    TreeNode<int> b(10);
    TreeNode<int> c(7);
    TreeNode<int> d(5);
    TreeNode<int> e(5);
    TreeNode<int> f(2);
    TreeNode<int> g(3);

    a1.addChild(b);
    a1.addChild(c);
    a1.addChild(d);
    a1.addChild(e);
    a1.addChild(f);
    a1.addChild(g);
    a1.printChildren();

    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
