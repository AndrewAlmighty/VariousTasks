#include "nodetester.h"
#include "treenode.h"
#include <memory>
#include <iostream>
NodeTester::NodeTester()
{

}

bool NodeTester::runTests()
{
    if (!uniqueObjectNameTest() ||
            !sortingTest() ||
            !serializeNodeTest() ||
            !serializeTreeTest())
    return false;

    else return true;
}

bool NodeTester::uniqueObjectNameTest()
{
    std::cout << "\n----\nRunning uniqueObjectNameTest";
    TreeNode<double> main(10, "parent");
    TreeNode<double> main2(10, "parent");
    TreeNode<double> main3(10, "parent");

    if (main.getObjectName() == main2.getObjectName() ||
            main.getObjectName() == main3.getObjectName() ||
            main2.getObjectName() == main3.getObjectName())
    {
        std::cout << "\nuniqueObjectNameTest failed\n----\n";
        return false;
    }

    else
    {
        std::cout << "\nuniqueObjectNameTest passed\n----\n";
        return true;
    }
}

bool NodeTester::sortingTest()
{
    std::cout << "\n----\nRunning sortingTest";
    TreeNode<double> main(10, "parent");
    main.addChild(std::make_shared<TreeNode<double>>(1.5));
    main.addChild(std::make_shared<TreeNode<double>>(3.4));
    main.addChild(std::make_shared<TreeNode<double>>(1.1));
    main.addChild(std::make_shared<TreeNode<double>>(5.4));
    main.addChild(std::make_shared<TreeNode<double>>(3.4));

    double arr[5];
    for (int i = 0; i < 5; i ++)
        arr[i] = main.getChild(i) -> getValue();

    bool isSorted = true;
    for (int i = 1; i < 5; i++)
    {
        if (arr[i] >= arr[i - 1])
            continue;

        else isSorted = false;
    }

    if (isSorted)
    {
        std::cout << "\nsortingTest passed\n----\n";
        return true;
    }

    else
    {
        std::cout << "\nsortingTest failed\n----\n";
        return false;
    }
}

bool NodeTester::serializeNodeTest()
{
    std::cout << "\n----\nRunning serializeNodeTest";
    TreeNode<long> main(10, "parent");
    main.addChild(std::make_shared<TreeNode<long>>(1));
    main.addChild(std::make_shared<TreeNode<long>>(2));
    main.addChild(std::make_shared<TreeNode<long>>(3));

    std::string out = main.serializeNode();

    if (out == "parent:10,NoParent,Children:N_1:1,N_2:2,N_3:3;")
    {
        std::cout << "\nserializeNodeTest passed\n----\n";
        return true;
    }

    else
    {
        std::cout << "\nserializeNodeTest failed\n----\n";
        return false;
    }
}

bool NodeTester::serializeTreeTest()
{
    std::cout << "\n----\nRunning serializeTreeTest";
    TreeNode<int> a1(1, "GrandParent");
    TreeNode<int>* ptr = nullptr;
    a1.addChild(std::make_shared<TreeNode<int>>(1));
    a1.addChild(std::make_shared<TreeNode<int>>(2));
    a1.addChild(std::make_shared<TreeNode<int>>(3));

    ptr = a1.getChild(0);
    ptr -> addChild(std::make_shared<TreeNode<int>>(4));
    ptr -> addChild(std::make_shared<TreeNode<int>>(5));

    ptr = a1.getChild(1);
    ptr -> addChild(std::make_shared<TreeNode<int>>(6));

    ptr = a1.getChild(0);

    ptr = ptr -> getChild(0);
    ptr -> addChild(std::make_shared<TreeNode<int>>(7));

    std::string out = ptr -> serializeTree();

    if (out == "N:GrandParent(1)P:NULL,CH:N_1(1)N_2(2)N_3(3);N:N_1(1)P:GrandParent(1),CH:N_4(4)N_5(5);"
            "N:N_4(4)P:N_1(1),CH:N_7(7);N:N_7(7)P:N_4(4),CH:NULL;N:N_5(5)P:N_1(1),CH:NULL;"
            "N:N_2(2)P:GrandParent(1),CH:N_6(6);N:N_6(6)P:N_2(2),CH:NULL;N:N_3(3)P:GrandParent(1),CH:NULL;")
    {
        std::cout << "\nserializeTreeTest passed\n----\n";
        return true;
    }

    else
    {
        std::cout << "\nserializeTreeTest failed\n----\n";
        return false;
    }
}
