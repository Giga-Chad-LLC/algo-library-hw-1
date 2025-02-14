module;

#include <concepts>
#include <functional>
#include <memory>
#include <vector>
#include <algorithm>

export module tree_algorithms:BSTree;
import :TreeNode;


namespace trees {

namespace details {

// Define concepts
template <typename T>
concept CopyConstructible = std::copy_constructible<T>;

template <typename Comp, typename T>
concept Comparator = requires(Comp comp, const T& a, const T& b) {
    { comp(a, b) } -> std::convertible_to<bool>;
};

template <typename Alloc, typename T>
concept Allocator = requires(Alloc alloc, T* ptr, std::size_t n) {
    { alloc.allocate(n) } -> std::same_as<T*>;
    alloc.deallocate(ptr, n);
};


}

using namespace nodes;

export template<
    details::CopyConstructible T,
    details::Comparator<T> Comp = std::less<T>,
    details::Allocator<T> Alloc = std::allocator<T>
>
class BSTree {
public:
    BSTree(Comp comparator, Alloc allocator)
        : m_comparator(comparator), m_allocator(allocator) {}

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

            if (!m_comparator(node->value, value) && !m_comparator(value, node->value)) {
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
            destroy(node);
        }

        this->m_root = nullptr;
    }

    // BSTree<T, Comp, Alloc>::Iterator begin() const
    // BSTree<T, Comp, Alloc>::Iterator end() const

    virtual ~BSTree() = default;

protected:
    template<typename NodeType, typename NodeAllocType>
    NodeType* create(NodeAllocType& node_alloc, const T& value) {
        using NodeAllocTraits = std::allocator_traits<NodeAllocType>;

        NodeType* node = NodeAllocTraits::allocate(node_alloc, 1);
        NodeAllocTraits::construct(node_alloc, node, value);
        return node;
    }

    template<typename NodeType, typename NodeAllocType>
    void destroy(NodeAllocType& node_alloc, NodeType* node) {
        using NodeAllocTraits = std::allocator_traits<NodeAllocType>;

        NodeAllocTraits::destroy(node_alloc, node);
        NodeAllocTraits::deallocate(node_alloc, node, 1);
    }

    virtual TreeNode<T>* create(const T& value) = 0;
    virtual void destroy(TreeNode<T>* node) = 0;

    size_t m_size = 0;
    TreeNode<T> *m_root = nullptr;
    Comp m_comparator;
    Alloc m_allocator;
};

}