module;

#include <functional>
#include <memory>
#include <vector>

export module tree_algorithms;
import :nodes;



export namespace trees {

using namespace nodes;

template<typename T, typename Comp = std::less<T>, typename Alloc = std::allocator<T>>
class BSTree {
public:
    virtual void build(const T* items, size_t n) = 0;

    virtual void insert(const T&) = 0;
    virtual size_t count(const T&) = 0;
    virtual bool remove(const T&) = 0;
    virtual void clear() = 0;

    // BSTree<T, Comp, Alloc>::Iterator begin() const
    // BSTree<T, Comp, Alloc>::Iterator end() const

    virtual ~BSTree() = default;

protected:
    size_t m_size = 0;
    TreeNode<T> *m_root = nullptr;
};




/**
 * Maintains the invariant at each node that the absolute difference in height between subtrees is at most 1.
 * Search, insertion, and deletion operations are O(log n).
 *
 * Implementation taken from: https://github.com/KadirEmreOto/AVL-Tree
 */
template<typename T, typename Comp = std::less<T>, typename Alloc = std::allocator<T>>
class AVLTree : public BSTree<T, Comp, Alloc> {
public:
    void build(const T* items, size_t n) final override {
        for (size_t i = 0; i < n; i++) {
            insert(items[i]);
        }
    }

    void insert(const T& value) final override {
        TreeNode<T> **indirect = &(this->m_root);
        std::vector<TreeNode<T>**> path;

        while (*indirect != nullptr) {
            path.push_back(indirect);

            if ((*indirect)->value > value) {
                indirect = &((*indirect)->left);
            }
            else {
                indirect = &((*indirect)->right);
            }
        }

        *indirect = new AVLTreeNode<T>(value);
        path.push_back(indirect);

        balance(path);
        this->m_size++;
    }

    size_t count(const T&) final override {
        return 0;
    }

    bool remove(const T&) final override {
        return false;
    }

    void clear() final override {
        std::vector<TreeNode<T>*> stack;

        if (this->m_root != nullptr) {
            stack.push_back(this->m_root);
        }

        while (!stack.empty()) {
            TreeNode<T> *node = stack.back();
            stack.pop_back();

            if (node->left != nullptr) {
                stack.push_back(node->left);
            }

            if (node->right != nullptr) {
                stack.push_back(node->right);
            }

            this->m_size--;
            delete node;
        }

        this->m_root = nullptr;
    }

    ~AVLTree() {
        clear();
    }

private:
    void balance(std::vector<TreeNode<T>**> path) {
    std::reverse(path.begin(), path.end());

    for (TreeNode<T>** indirect : path) {
        AVLTreeNode<T>* node = static_cast<AVLTreeNode<T>*>(*indirect);

        AVLTreeNode<T>* left = static_cast<AVLTreeNode<T>*>(node->left);
        AVLTreeNode<T>* right = static_cast<AVLTreeNode<T>*>(node->right);

        node->updateValues();

        if (node->balanceFactor() >= 2 && left->balanceFactor() >= 0) {
            // left - left
            *indirect = node->rightRotate();
        }
        else if (node->balanceFactor() >= 2) {
            // left - right
            node->left = left->leftRotate();
            *indirect = node->rightRotate();
        }
        else if (node->balanceFactor() <= -2 && right->balanceFactor() <= 0) {
            // right - right
            *indirect = node->leftRotate();
        }
        else if (node->balanceFactor() <= -2) {
            // right - left
            node->right = right->rightRotate();
            *indirect = node->leftRotate();
        }
    }
}


};

}