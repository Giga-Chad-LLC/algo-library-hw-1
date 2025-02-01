module;

#include <functional>
#include <memory>
#include <vector>
#include <algorithm>

export module tree_algorithms:BSTree;
import :TreeNode;


namespace trees {

using namespace nodes;

export template<typename T, typename Comp = std::less<T>, typename Alloc = std::allocator<T>>
class BSTree {
public:
    virtual void build(const T* items, size_t n) = 0;

    virtual void insert(const T&) = 0;

    virtual size_t count(const T& value) {
        // TODO: this implementation is too slow!
        std::vector<TreeNode<T>*> stack;

        if (this->m_root != nullptr) {
            stack.push_back(this->m_root);
        }

        size_t answer = 0;

        while (!stack.empty()) {
            TreeNode<T> *node = stack.back();
            stack.pop_back();

            if (node->value == value) {
                ++answer;
            }

            if (node->left != nullptr) {
                stack.push_back(node->left);
            }
            if (node->right != nullptr) {
                stack.push_back(node->right);
            }
        }

        return answer;
    }

    virtual bool remove(const T&) = 0;

    virtual void clear() {
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

    // BSTree<T, Comp, Alloc>::Iterator begin() const
    // BSTree<T, Comp, Alloc>::Iterator end() const

    virtual ~BSTree() = default;

protected:
    size_t m_size = 0;
    TreeNode<T> *m_root = nullptr;
};

}