#include "mainwindow.h"
#include "nodetester.h"
#include "treenode.h"

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

    TreeNode<int> a1(1, "Root");
    TreeNode<int>* ptr = nullptr;
    a1.addChild(std::make_shared<TreeNode<int>>(10));
    a1.addChild(std::make_shared<TreeNode<int>>(2));
    a1.addChild(std::make_shared<TreeNode<int>>(3));

    ptr = a1.getChild(0);
    ptr -> addChild(std::make_shared<TreeNode<int>>(4));
    ptr -> addChild(std::make_shared<TreeNode<int>>(5));

    ptr = a1.getChild(1);
    ptr -> addChild(std::make_shared<TreeNode<int>>(6));
    ptr -> addChild(std::make_shared<TreeNode<int>>(7));
    ptr -> addChild(std::make_shared<TreeNode<int>>(7));

    ptr = a1.getChild(0);

    ptr = ptr -> getChild(0);
    ptr -> addChild(std::make_shared<TreeNode<int>>(8));
    ptr -> addChild(std::make_shared<TreeNode<int>>(9));

    ptr = ptr -> getChild(1);
    ptr -> addChild(std::make_shared<TreeNode<int>>(10));

    std::cout << "Prepared tree. Serialized tree looks like:\n" << ptr -> serializeTree() << std::endl;
    std::cout << "Rendering tree.";

    QApplication a(argc, argv);
    MainWindow w;
    w.acceptSerializedTree(ptr -> serializeTree());
    w.show();
    return a.exec();
}
