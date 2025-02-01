module;

#include <functional>
#include <memory>
#include <vector>
#include <algorithm>

export module tree_algorithms:AVLTree;
import :BSTree;
// nodes
import :TreeNode;
import :AVLTreeNode;



namespace trees {

using namespace nodes;

/**
 * Maintains the invariant at each node that the absolute difference in height between subtrees is at most 1.
 * Search, insertion, and deletion operations are O(log n).
 *
 * Implementation taken from: https://github.com/KadirEmreOto/AVL-Tree
 */
export template<typename T, typename Comp = std::less<T>, typename Alloc = std::allocator<T>>
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

    bool remove(const T& value) final override {
        TreeNode<T> **indirect = &(this->m_root);
        std::vector<TreeNode<T>**> path;

        while ((*indirect != nullptr) && (*indirect)->value != value) {
            path.push_back(indirect);

            if ((*indirect)->value > value) {
                indirect = &((*indirect)->left);
            }
            else {
                indirect = &((*indirect)->right);
            }
        }

        if (*indirect == nullptr) {
            // the value does not exist in tree
            return false;
        }
        else {
            path.push_back(indirect);
        }

        std::size_t index = path.size();

        if (((*indirect)->left == nullptr) && ((*indirect)->right == nullptr)) {
            // the node is leaf
            delete *indirect;
            *indirect = nullptr;
            // pop the deleted node from path
            path.pop_back();
        }
        else if ((*indirect)->right == nullptr) {
            // only left child exists
            TreeNode<T> *toRemove = *indirect;

            (*indirect) = (*indirect)->left;
            delete toRemove;

            path.pop_back();
        }
        else {
            // right child exists
            TreeNode<T> **successor = &((*indirect)->right);

            while ((*successor)->left != nullptr) {
                path.push_back(successor);
                successor = &((*successor)->left);
            }

            if (*successor == (*indirect)->right) {
                (*successor)->left = (*indirect)->left;

                TreeNode<T> *toRemove = *indirect;
                *indirect = *successor;
                delete toRemove;
            }
            else {
                TreeNode<T> *tmp = *path.back(), *suc = *successor;

                tmp->left = (*successor)->right;
                suc->left = (*indirect)->left;
                suc->right = (*indirect)->right;

                delete *indirect;
                *indirect = suc;
                path[index] = &(suc->right);
            }
        }

        balance(path);
        this->m_size--;
        return true;
    }

    ~AVLTree() {
        this->clear();
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