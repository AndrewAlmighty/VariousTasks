#include "mainwindow.h"
#include <QApplication>
#include <iostream>
#include <memory>
#include "treenode.h"

int main(int argc, char *argv[])
{
    TreeNode<int> a1(1);
    a1.addChild(std::make_shared<TreeNode<int>>(10));
    a1.addChild(std::make_shared<TreeNode<int>>(5));
    a1.addChild(std::make_shared<TreeNode<int>>(3));
    a1.addChild(std::make_shared<TreeNode<int>>(12));
    a1.addChild(std::make_shared<TreeNode<int>>(5));
    a1.addChild(std::make_shared<TreeNode<int>>(1));
    a1.addChild(std::make_shared<TreeNode<int>>(12));
    a1.addChild(std::make_shared<TreeNode<int>>(16));
    a1.addChild(std::make_shared<TreeNode<int>>(111));
    a1.printChildren();

    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
