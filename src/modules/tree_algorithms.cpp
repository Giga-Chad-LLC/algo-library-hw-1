module;

#include <functional>
#include <memory>

export module tree_algorithms;


export namespace trees {


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
    TreeNode<T> *m_root = nullptr;
    size_t m_size = 0;
};




/**
 * Maintains the invariant at each node that the absolute difference in height between subtrees is at most 1.
 * Search, insertion, and deletion operations are O(log n).
 */
template<typename T, typename Comp = std::less<T>, typename Alloc = std::allocator<T>>
class AVLTree : public BSTree<T, Comp, Alloc> {
public:
    void build(const T* items, size_t n) final override {

    }

    void insert(const T&) final override {

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
    void balance(std::vector<AVLTreeNode<T>**> path) {

    }
};

}