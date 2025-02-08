module;

#include <functional>
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
class AVLTree final : public BSTree<T, Comp, Alloc> {
    using NodeAllocatorType = typename std::allocator_traits<Alloc>::template rebind_alloc<AVLTreeNode<T>>;

public:
    explicit AVLTree(Comp comparator = Comp{}, Alloc allocator = Alloc{})
        : BSTree<T, Comp, Alloc>(comparator, allocator),
        m_node_allocator(this->m_allocator) {}

    void insert(const T& value) override {
        TreeNode<T> **indirect = &(this->m_root);
        std::vector<TreeNode<T>**> path;

        while (*indirect != nullptr) {
            path.push_back(indirect);

            if (this->m_comparator((*indirect)->value, value)) {
                indirect = &((*indirect)->right);
            }
            else {
                indirect = &((*indirect)->left);
            }
        }

        *indirect = create(value);
        path.push_back(indirect);

        balance(path);
        ++this->m_size;
    }

    bool remove(const T& value) override {
        TreeNode<T> **indirect = &(this->m_root);
        std::vector<TreeNode<T>**> path;

        while (
            (*indirect != nullptr) &&
            (
                this->m_comparator((*indirect)->value, value) ||
                this->m_comparator(value, (*indirect)->value)
            )
        ) {
            path.push_back(indirect);

            if (this->m_comparator((*indirect)->value, value)) {
                indirect = &((*indirect)->right);
            }
            else {
                indirect = &((*indirect)->left);
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
            // the node is a leaf
            destroy(*indirect);
            *indirect = nullptr;
            // pop the deleted node from path
            path.pop_back();
        }
        else if ((*indirect)->right == nullptr) {
            // only left child exists
            TreeNode<T> *toRemove = *indirect;

            (*indirect) = (*indirect)->left;
            destroy(toRemove);
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
                destroy(toRemove);
            }
            else {
                TreeNode<T> *tmp = *path.back(), *suc = *successor;

                tmp->left = (*successor)->right;
                suc->left = (*indirect)->left;
                suc->right = (*indirect)->right;

                destroy(*indirect);
                *indirect = suc;
                path[index] = &(suc->right);
            }
        }

        balance(path);
        --this->m_size;
        return true;
    }

    ~AVLTree() override {
        this->clear();
    }

private:
    void balance(std::vector<TreeNode<T>**> path) {
        std::reverse(path.begin(), path.end());

        for (TreeNode<T>** indirect : path) {
            auto* node = static_cast<AVLTreeNode<T>*>(*indirect);

            auto* left = static_cast<AVLTreeNode<T>*>(node->left);
            auto* right = static_cast<AVLTreeNode<T>*>(node->right);

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

    TreeNode<T>* create(const T& value) override {
        return this->BSTree<T, Comp, Alloc>::template create<AVLTreeNode<T>>(m_node_allocator, value);
    }

    void destroy(TreeNode<T>* node) override {
        this->BSTree<T, Comp, Alloc>::template destroy<AVLTreeNode<T>>(m_node_allocator, static_cast<AVLTreeNode<T>*>(node));
    }

    NodeAllocatorType m_node_allocator;
};

}