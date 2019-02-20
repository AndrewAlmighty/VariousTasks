#include "treenode.h"

template<typename type>
TreeNode<type>::TreeNode(const type &val, const std::string &name)
{
    value = val;
    objectName = name;
}

template<typename type>
void TreeNode<type>::pointParent(const TreeNode &parent)
{
    m_parent = parent;
}

template<typename type>
void TreeNode<type>::addChild(const TreeNode &child)
{

}

template<typename type>
std::string TreeNode<type>::serialize() const
{

}
