#ifndef TREENODE_H
#define TREENODE_H

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
    }

    void addChild(const TreeNode &child)
    {
        int idx = findPlaceForNewChild(child);
        m_children.insert(idx, child);
    }

    void changeObjectName(const std::string &name)
    {
        if (isObjectNameUnique(name))
            objectName = name;

        else
            objectName = name + "_" + std::to_string(m_id);
    }

    T getValue() const
    {
        return value;
    }

    void pointParent(const TreeNode &parent)
    {
        m_parent = parent;
    }

    std::string serialize() const
    {
        return "";
    }

private:
    bool isObjectNameUnique(std::string name) const
    {
        for (unsigned i = 0; i < m_objectNameList.size(); i++)
        {
            if (name == m_objectNameList[i])
                return false;
        }

        return true;
    }

    int findPlaceForNewChild(const T &val)
    {
        if (m_children.empty())
            return 0;

        int idx;

        for (idx = 0; idx < m_children.size(); idx++)
        {
            if (val <= m_children[idx])
                break;
        }

        return idx;
    }

    static std::vector<std::string> m_objectNameList;
    static int m_nodesCounter;
    TreeNode *m_parent = nullptr;
    std::vector<TreeNode*> m_children;
    int m_id;
    T value;
    std::string objectName;
};

template <typename T, typename T2>
int TreeNode<T, T2>::m_nodesCounter = 0;

template <typename T, typename T2>
std::vector<std::string> TreeNode<T, T2>::m_objectNameList = {};

#endif // TREENODE_H
