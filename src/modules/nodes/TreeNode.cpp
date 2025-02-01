module;

#include <memory>

export module tree_algorithms:nodes;



namespace trees::nodes {

template <class T>
struct TreeNode {
    TreeNode(T value) : value(std::move(value)) {}

    TreeNode *left = nullptr;
    TreeNode *right = nullptr;

    const T value;
    // how many nodes are there in this subtree
    int _count = 1;
    int _height = 1;

protected:
    int height(const TreeNode<T>* node) const noexcept {
        return (node != nullptr) ? node->_height : 0;
    }

    int size(const TreeNode<T>* node) const noexcept {
        return (node != nullptr) ? node->_count : 0;
    }
};


/**
 * Implementation taken from: https://github.com/KadirEmreOto/AVL-Tree
 */
template <class T>
struct AVLTreeNode : public TreeNode<T> {
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
