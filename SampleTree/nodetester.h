#ifndef NODETESTER_H
#define NODETESTER_H


class NodeTester
{
public:
    NodeTester();
    bool runTests();

private:
    bool uniqueObjectNameTest();
    bool sortingTest();
    bool serializeNodeTest();
    bool serializeTreeTest();
};

#endif // NODETESTER_H
