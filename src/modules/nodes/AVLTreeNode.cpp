module;

#include <memory>

export module tree_algorithms:AVLTreeNode;
import :TreeNode;


namespace trees::nodes {

/**
 * Implementation taken from: https://github.com/KadirEmreOto/AVL-Tree
 */
template <class T>
struct AVLTreeNode : public TreeNode<T> {
    using TreeNode<T>::TreeNode;

    void updateValues() {
        this->_count = this->size(this->left) + this->size(this->right) + 1;
        this->_height = std::max(this->height(this->left), this->height(this->right)) + 1;
    }

    int balanceFactor() {
        return this->height(this->left) - this->height(this->right);
    }

    AVLTreeNode* leftRotate() {
        AVLTreeNode* prevRight = static_cast<AVLTreeNode*>(this->right);
        this->right = this->right->left;
        prevRight->left = this;

        // the order matters
        this->updateValues();
        prevRight->updateValues();

        return prevRight;
    }

    AVLTreeNode* rightRotate() {
        AVLTreeNode* prevLeft = static_cast<AVLTreeNode*>(this->left);
        this->left = this->left->right;
        prevLeft->right = this;

        // the order matters
        this->updateValues();
        prevLeft->updateValues();

        return prevLeft;
    }
};


}