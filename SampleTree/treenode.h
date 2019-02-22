#ifndef TREENODE_H
#define TREENODE_H

#include <algorithm>
#include <iostream>
#include <memory>
#include <set>
#include <list>
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
        child -> pointParent(this);
        m_children.insert(std::move(child));
    }

    void changeObjectName(const std::string &name)
    {
        if (isObjectNameUnique(name))
            objectName = name;

        else
            objectName = name + "_" + std::to_string(m_id);
    }

    int getNumberOfChildren() const
    {
        return m_children.size();
    }

    TreeNode* getChild(int idx)
    {
        int i = 0;
        for (auto &node : m_children)
        {
            if (i == idx)
                return node.get();

            i++;
        }

        return nullptr;
    }

    std::string getObjectName() const
    {
        return objectName;
    }

    T getValue() const
    {
        return value;
    }

    void pointParent(TreeNode *parent)
    {
        m_parent = parent;
    }

    std::string serializeNode() const
    {
        std::string output;
        output = objectName + ":" + std::to_string(value);

        if (m_parent == nullptr)
            output += ",NoParent";

        else
            output += ",Parent:" + m_parent -> getObjectName() + ":" + std::to_string(m_parent -> value);

        if (m_children.empty())
            output += ",NoChildren";

        else
        {
            output += ",Children:";
            for (const auto& node : m_children)
                output += node -> getObjectName() + ":" + std::to_string(node -> getValue()) + ",";

            output = output.substr(0, output.size() - 1); //remove last ','
        }

        output += ";";
        return output;
    }

private:
    struct MyComparator
    {
        bool operator()(const std::shared_ptr<TreeNode> lhs, const std::shared_ptr<TreeNode> rhs)
        {
            return lhs -> getValue() <= rhs -> getValue();
        }
    };

    bool isObjectNameUnique(std::string name) const
    {
        auto findIter = std::find(m_objectNameList.begin(), m_objectNameList.end(), name);
        if (findIter != m_objectNameList.end())
            return false;

        return true;
    }

    static std::list<std::string> m_objectNameList;
    static int m_nodesCounter;
    TreeNode* m_parent = nullptr;
    std::set<std::shared_ptr<TreeNode>, MyComparator> m_children;
    int m_id;
    std::string objectName;
    T value;    
};

template <typename T, typename T2>
int TreeNode<T, T2>::m_nodesCounter = 0;

template <typename T, typename T2>
std::list<std::string> TreeNode<T, T2>::m_objectNameList = {};

#endif // TREENODE_H
