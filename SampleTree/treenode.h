#ifndef TREENODE_H
#define TREENODE_H

#include <iostream>
#include <memory>
#include <set>
#include <vector>
#include <string>

//This should make our class to accept only arithmetic types.
template <typename T,
          typename = typename std::enable_if<std::is_arithmetic<T>::value, T>::type>
class TreeNode
{
public:
    TreeNode(const T &val, const std::string &name = "")
    {
        m_id = m_nodesCounter;
        m_nodesCounter++;
        value = val;

        if (name.empty())
            objectName = "Node_" + std::to_string(m_id);

        else
            changeObjectName(name);

        m_objectNameList.push_back(objectName);
        std::cout << "Created node " << objectName << std::endl;
    }

    ~TreeNode()
    {
        std::cout << "Destroyed node " << objectName << std::endl;
    }

    void addChild(std::shared_ptr<TreeNode<T>> &&child)
    {
        m_children.insert(std::move(child));
    }

    void changeObjectName(const std::string &name)
    {
        if (isObjectNameUnique(name))
            objectName = name;

        else
            objectName = name + "_" + std::to_string(m_id);
    }

    std::string getObjectName() const
    {
        return objectName;
    }

    T getValue() const
    {
        return value;
    }

  //  void pointParent(const TreeNode &parent)
 //   {
  //      m_parent = std::make_shared<TreeNode>(parent);
  //  }

    void printChildren() const
    {
        if (m_children.empty())
            std::cout << "This node doesn't have children!\n";

        else
        {
            std::cout << "Children of node: " << objectName << std::endl;
            for (const auto& node : m_children)
                std::cout << "Object name: " << node -> getObjectName() << ", value:" << node -> getValue() << std::endl;
        }
    }

    std::string serialize() const
    {
        return "";
    }

private:
    struct MyComparator
    {
        bool operator()(const std::shared_ptr<TreeNode> lhs, const std::shared_ptr<TreeNode> rhs)
        {
            return lhs -> getValue() < rhs -> getValue();
        }
    };

    bool isObjectNameUnique(std::string name) const
    {
        for (unsigned i = 0; i < m_objectNameList.size(); i++)
        {
            if (name == m_objectNameList[i])
                return false;
        }

        return true;
    }

    static std::vector<std::string> m_objectNameList;
    static int m_nodesCounter;
  //  std::shared_ptr<TreeNode> m_parent;
    std::set<std::shared_ptr<TreeNode>, MyComparator> m_children;
    int m_id;
    T value;
    std::string objectName;
};

template <typename T, typename T2>
int TreeNode<T, T2>::m_nodesCounter = 0;

template <typename T, typename T2>
std::vector<std::string> TreeNode<T, T2>::m_objectNameList = {};

#endif // TREENODE_H
