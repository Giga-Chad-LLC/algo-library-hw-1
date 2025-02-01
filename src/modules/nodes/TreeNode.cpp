module;

#include <memory>

export module tree_algorithms:TreeNode;



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

}
