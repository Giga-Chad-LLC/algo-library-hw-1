module;

#include <utility>

export module tree_algorithms:RBTreeNode;
import :TreeNode;

namespace trees::nodes {

    enum Color { RED, BLACK };

    template <class T>
    struct RBTreeNode final : public TreeNode<T> {
        using TreeNode<T>::TreeNode;

        Color color = RED;           // Новый узел по умолчанию красный
        RBTreeNode* parent = nullptr; // Указатель на родителя

        // Левый поворот: возвращает новый корень поддерева
        RBTreeNode* leftRotate() {
            RBTreeNode* y = static_cast<RBTreeNode*>(this->right);
            this->right = y->left;
            if (y->left != nullptr) {
                static_cast<RBTreeNode*>(y->left)->parent = this;
            }
            y->parent = this->parent;
            if (this->parent != nullptr) {
                if (this == static_cast<RBTreeNode<T>*>(this->parent->left))
                    this->parent->left = y;
                else
                    this->parent->right = y;
            }
            y->left = this;
            this->parent = y;
            return y;
        }

        // Правый поворот: возвращает новый корень поддерева
        RBTreeNode* rightRotate() {
            RBTreeNode* x = static_cast<RBTreeNode*>(this->left);
            this->left = x->right;
            if (x->right != nullptr) {
                static_cast<RBTreeNode*>(x->right)->parent = this;
            }
            x->parent = this->parent;
            if (this->parent != nullptr) {
                if (this == static_cast<RBTreeNode<T>*>(this->parent->left))
                    this->parent->left = x;
                else
                    this->parent->right = x;
            }
            x->right = this;
            this->parent = x;
            return x;
        }
    };

} // namespace trees::nodes
