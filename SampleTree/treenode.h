#ifndef TREENODE_H
#define TREENODE_H

#include <vector>
#include <string>

template <typename type>
class TreeNode
{
public:
    TreeNode(const type &val, const std::string &name);
    void pointParent(const TreeNode &parent);
    void addChild(const TreeNode &child);
    std::string serialize() const;

private:
    TreeNode *m_parent = nullptr;
    std::vector<TreeNode*> m_children;
    type value;
    std::string objectName;
};

#endif // TREENODE_H
