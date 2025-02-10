module;

#include <algorithm>

export module tree_algorithms:AVLTreeNode;
import :TreeNode;


namespace trees::nodes {

/**
 * Implementation taken from: https://github.com/KadirEmreOto/AVL-Tree
 */
template <class T>
struct AVLTreeNode final : public TreeNode<T> {
    using TreeNode<T>::TreeNode;

    void updateValues() {
        auto* leftNode = static_cast<AVLTreeNode*>(this->left);
        auto* rightNode = static_cast<AVLTreeNode*>(this->right);

        this->_count = AVLTreeNode::size(leftNode) + AVLTreeNode::size(rightNode) + 1;
        this->_height = std::max(AVLTreeNode::height(leftNode), AVLTreeNode::height(rightNode)) + 1;
    }

    int balanceFactor() {
        auto* leftNode = static_cast<AVLTreeNode*>(this->left);
        auto* rightNode = static_cast<AVLTreeNode*>(this->right);

        return AVLTreeNode::height(leftNode) - AVLTreeNode::height(rightNode);
    }

    AVLTreeNode* leftRotate() {
        auto* prevRight = static_cast<AVLTreeNode*>(this->right);
        this->right = this->right->left;
        prevRight->left = this;

        // the order matters
        this->updateValues();
        prevRight->updateValues();

        return prevRight;
    }

    AVLTreeNode* rightRotate() {
        auto* prevLeft = static_cast<AVLTreeNode*>(this->left);
        this->left = this->left->right;
        prevLeft->right = this;

        // the order matters
        this->updateValues();
        prevLeft->updateValues();

        return prevLeft;
    }

private:
    // how many nodes are there in this subtree
    int _count = 1;
    int _height = 1;

    static int height(const AVLTreeNode<T>* node) noexcept {
        return (node != nullptr) ? node->_height : 0;
    }

    static int size(const AVLTreeNode<T>* node) noexcept {
        return (node != nullptr) ? node->_count : 0;
    }
};


}