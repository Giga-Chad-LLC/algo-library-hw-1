module;

#include <functional>
#include <vector>
#include <algorithm>

export module tree_algorithms:TreapTree;
import :BSTree;
// nodes
import :TreeNode;
import :TreapTreeNode;



namespace trees {

using namespace nodes;

/**
 * Each node contains a pair `<x, y>`, where `x` is a value, and `y` is a randomly (uniformly)
 * distributed int (rank, so to speak).
 *
 * The treap is a BST by `x` (left subnode contains elements that are `<`,
 * right subnode those that are `>=`) and a heap by `y`.
 * Thus, the operations with it are done in `O(log n)`.
 */
export template<typename T, typename Comp = std::less<T>, typename Alloc = std::allocator<T>>
class TreapTree final : public BSTree<T, Comp, Alloc> {
    using NodeAllocatorType = typename std::allocator_traits<Alloc>::template rebind_alloc<TreapTreeNode<T>>;

public:
    explicit TreapTree(Comp comparator = Comp{}, Alloc allocator = Alloc{})
        : BSTree<T, Comp, Alloc>(comparator, allocator),
        m_node_allocator(this->m_allocator) {}

    void insert(const T& value) override {
        TreeNode<T>* node = create(value);
        TreeNode<T>* root = this->m_root;
        auto [left, right] = split(root, value);

        auto t1 = merge(left, node);
        auto t2 = merge(t1, right);

        this->m_size++;
        this->m_root = t2;
    }

    bool remove(const T& value) override {
        TreeNode<T>* root = this->m_root;
        TreeNode<T>* parent = nullptr;
        bool removed = false;

        while (root != nullptr) {
            if (this->m_comparator(root->value, value)) {
                // root->value < value, go right
                parent = root;
                root = root->right;
            }
            else if (this->m_comparator(value, root->value)) {
                // root->value > value, go left
                parent = root;
                root = root->left;
            }
            else {
                // found a matching node (`root` will be removed)
                auto* subtree = merge(root->left, root->right);
                removed = true;
                if (parent != nullptr) {
                    if (parent->left == root) parent->left = subtree;
                    else parent->right = subtree;
                }
                else {
                    // if no parent, then the matching node is the previous root, then substitute it
                    this->m_root = subtree;
                }
                break;
            }
        }

        if (removed) {
            this->m_size--;
            this->destroy(root);
        }

        return removed;
    }

    ~TreapTree() override {
        this->clear();
    }

private:
    std::pair<TreeNode<T>*, TreeNode<T>*> split(TreeNode<T>* root, const T& splitValue) const {
        if (root == nullptr) {
            return { nullptr, nullptr };
        }

        if (this->m_comparator(root->value, splitValue)) {
            // root->value < splitValue
            auto [left, right] = split(root->right, splitValue);
            root->right = left;
            return { root, right };
        }
        else {
            // root->value >= splitValue
            auto [left, right] = split(root->left, splitValue);
            root->left = right;
            return { left, root };
        }
    }

    TreeNode<T>* merge(TreeNode<T>* t1_, TreeNode<T>* t2_) const {
        auto* t1 = static_cast<TreapTreeNode<T>*>(t1_);
        auto* t2 = static_cast<TreapTreeNode<T>*>(t2_);

        if (t2 == nullptr) return t1;
        if (t1 == nullptr) return t2;

        TreeNode<T>* res = nullptr;
        if (t1->rank > t2->rank) {
            t1->right = merge(t1->right, t2);
            res = t1;
        }
        else {
            t2->left = merge(t1, t2->left);
            res = t2;
        }

        return res;
    }

    TreeNode<T>* create(const T& value) override {
        return this->BSTree<T, Comp, Alloc>::template create<TreapTreeNode<T>>(m_node_allocator, value);
    }

    void destroy(TreeNode<T>* node) override {
        this->BSTree<T, Comp, Alloc>::template destroy<TreapTreeNode<T>>(m_node_allocator, static_cast<TreapTreeNode<T>*>(node));
    }

    NodeAllocatorType m_node_allocator;
};

}