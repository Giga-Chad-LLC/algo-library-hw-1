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
    size_t _count = 1;
    size_t _height = 1;

protected:
    size_t height(const TreeNode<T>* node) const noexcept {
        return (node != nullptr) ? node->_height : 0;
    }

    size_t size(const TreeNode<T>* node) const noexcept {
        return (node != nullptr) ? node->_count : 0;
    }
};


/**
 * Implementation taken from: https://github.com/KadirEmreOto/AVL-Tree
 */
template <class T>
struct AVLTreeNode : public TreeNode<T> {
    void updateValues() {
        this->_count = size(this->left) + size(this->right) + 1;
        this->_height = std::max(height(this->left), height(this->right)) + 1;
    }

    int balanceFactor() {
        return height(this->left) - height(this->right);
    }

    AVLTreeNode* leftRotate() {
        AVLTreeNode* prevRight = this->right;
        this->right = this->right->left;
        prevRight->left = this;

        // the order matters
        this->updateValues();
        prevRight->updateValues();

        return prevRight;
    }

    AVLTreeNode* rightRotate() {
        AVLTreeNode* prevLeft = this->left;
        this->left = this->left->right;
        prevLeft->right = this;

        // the order matters
        this->updateValues();
        prevLeft->updateValues();

        return prevLeft;
    }
};

}
