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

/**
 * BSTree -- *balanced* search tree. We know that BST refers to binary search tree,
 * but in our case we planned to have B-tree as well, which is not binary, obviously, so naming clash happened here.
 * @tparam T stored type, which must be `CopyConstructible`.
 * @tparam Comp functor (implements `bool operator()(const T&, const T&) const`), default is `std::less<T>`.
 * @tparam Alloc allocator, default is `std::allocator<T>`. Users gives use allocator which manages the `T`, but we
 * want ot be able to allocate with it also the nodes of the tree and not only the `T`. For this the `rebind_alloc` is used (https://en.cppreference.com/w/cpp/memory/allocator_traits),
 * see `AVLTree<>` as an example of how to use it.
 */
export template<
    details::CopyConstructible T,
    details::Comparator<T> Comp = std::less<T>,
    details::Allocator<T> Alloc = std::allocator<T>
>
class BSTree {
public:
    BSTree(Comp comparator, Alloc allocator)
        : m_comparator(comparator), m_allocator(allocator) {}

    virtual void build(const T* items, const size_t n) {
        for (size_t i = 0; i < n; i++) {
            insert(items[i]);
        }
    }

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

            // equivalent to: node.value == value
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

            --this->m_size;
            destroy(node);
        }

        this->m_root = nullptr;
    }

    // Decided not to implement....
    // BSTree<T, Comp, Alloc>::Iterator begin() const
    // BSTree<T, Comp, Alloc>::Iterator end() const

    virtual ~BSTree() = default;

protected:
    /**
     * Subclasses of `BSTTree<...>` will call this method from overriden `TreeNode<T>* create(const T&)`
     * with their rebinded allocator for instantiation of their nodes (e.g. AVLTree<...> uses allocator which
     * manages AVLTreeNode<...> nodes, etc.)
     */
    template<typename NodeType, typename NodeAllocType>
    NodeType* create(NodeAllocType& node_alloc, const T& value) {
        using NodeAllocTraits = std::allocator_traits<NodeAllocType>;

        NodeType* node = NodeAllocTraits::allocate(node_alloc, 1);
        NodeAllocTraits::construct(node_alloc, node, value);
        return node;
    }

    /**
     * Subclasses of `BSTTree<...>` will call this method from overriden `TreeNode<T>* destroy(const T&)`
     * with their rebinded allocator for deletion of their nodes (e.g. AVLTree<...> uses allocator which
     * manages AVLTreeNode<...> nodes, etc.)
     */
    template<typename NodeType, typename NodeAllocType>
    void destroy(NodeAllocType& node_alloc, NodeType* node) {
        using NodeAllocTraits = std::allocator_traits<NodeAllocType>;

        NodeAllocTraits::destroy(node_alloc, node);
        NodeAllocTraits::deallocate(node_alloc, node, 1);
    }

    /**
     * Used by each `BSTTree` implementor in order to allocate memory for nodes and `T`.
     */
    virtual TreeNode<T>* create(const T& value) = 0;
    /**
     * Used by each `BSTTree` implementor in order to deallocate memory of nodes and `T`.
     */
    virtual void destroy(TreeNode<T>* node) = 0;

    size_t m_size = 0;
    TreeNode<T> *m_root = nullptr;
    Comp m_comparator;
    Alloc m_allocator;
};

}