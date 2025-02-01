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
    size_t count = 0;
    size_t height = 0;
};



template <class T>
struct AVLTreeNode : public TreeNode<T> {
    void updateValues() {

    }

    int balanceFactor() {

    }

    AVLTreeNode* leftRotate() {

    }

    AVLTreeNode* rightRotate() {

    }
};

}
