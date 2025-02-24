module;

#include <vector>
#include <algorithm>

export module tree_algorithms:RBTree;
import :BSTree;
import :TreeNode;
import :RBTreeNode;

namespace trees {

    using namespace nodes;

    export template<typename T, typename Comp = std::less<T>, typename Alloc = std::allocator<T>>
    class RBTree final : public BSTree<T, Comp, Alloc> {
        // Используем rebinding для управления памятью узлов типа RBTreeNode<T>
        using NodeAllocatorType = typename std::allocator_traits<Alloc>::template rebind_alloc<RBTreeNode<T>>;

    public:
        explicit RBTree(Comp comparator = Comp{}, Alloc allocator = Alloc{})
                : BSTree<T, Comp, Alloc>(comparator, allocator),
                  m_node_allocator(this->m_allocator)
        {}

        // Вставка элемента с последующей балансировкой по алгоритму RB-дерева
        void insert(const T& value) override {
            TreeNode<T> **indirect = &(this->m_root);
            RBTreeNode<T>* parent = nullptr;
            while (*indirect != nullptr) {
                parent = static_cast<RBTreeNode<T>*>(*indirect);
                if (this->m_comparator(parent->value, value))
                    indirect = &((*indirect)->right);
                else
                    indirect = &((*indirect)->left);
            }
            RBTreeNode<T>* newNode = static_cast<RBTreeNode<T>*>(create(value));
            newNode->color = RED;    // Новый узел вставляется красным
            newNode->parent = parent;
            *indirect = newNode;
            fixInsert(newNode);
            ++this->m_size;
        }

        // Удаление элемента с последующей балансировкой
        bool remove(const T& value) override {
            TreeNode<T> **indirect = &(this->m_root);
            RBTreeNode<T>* node = nullptr;
            while (*indirect != nullptr) {
                node = static_cast<RBTreeNode<T>*>(*indirect);
                if (this->m_comparator(node->value, value))
                    indirect = &((*indirect)->right);
                else if (this->m_comparator(value, node->value))
                    indirect = &((*indirect)->left);
                else
                    break;
            }
            if (*indirect == nullptr)
                return false; // Элемент не найден

            RBTreeNode<T>* target = node;
            Color originalColor = target->color;
            RBTreeNode<T>* x = nullptr;
            RBTreeNode<T>* xParent = nullptr;

            if (node->left == nullptr) {
                x = static_cast<RBTreeNode<T>*>(node->right);
                xParent = node->parent;
                transplant(node, static_cast<RBTreeNode<T>*>(node->right));
            } else if (node->right == nullptr) {
                x = static_cast<RBTreeNode<T>*>(node->left);
                xParent = node->parent;
                transplant(node, static_cast<RBTreeNode<T>*>(node->left));
            } else {
                RBTreeNode<T>* y = minimum(static_cast<RBTreeNode<T>*>(node->right));
                originalColor = y->color;
                x = static_cast<RBTreeNode<T>*>(y->right);
                if (y->parent == node) {
                    if (x)
                        x->parent = y;
                    xParent = y;
                } else {
                    transplant(y, static_cast<RBTreeNode<T>*>(y->right));
                    y->right = node->right;
                    if (y->right != nullptr)
                        static_cast<RBTreeNode<T>*>(y->right)->parent = y;
                    xParent = y->parent;
                }
                transplant(node, y);
                y->left = node->left;
                if (y->left != nullptr)
                    static_cast<RBTreeNode<T>*>(y->left)->parent = y;
                y->color = node->color;
            }

            destroy(node);
            --this->m_size;

            if (originalColor == BLACK) {
                fixDelete(x, xParent);
            }

            return true;
        }

        ~RBTree() override {
            this->clear();
        }

    private:
        // Корректировка после вставки
        void fixInsert(RBTreeNode<T>* node) {
            while (node != this->m_root && static_cast<RBTreeNode<T>*>(node->parent)->color == RED) {
                RBTreeNode<T>* parent = static_cast<RBTreeNode<T>*>(node->parent);
                RBTreeNode<T>* grandparent = static_cast<RBTreeNode<T>*>(parent->parent);
                if (parent == static_cast<RBTreeNode<T>*>(grandparent->left)) {
                    RBTreeNode<T>* uncle = static_cast<RBTreeNode<T>*>(grandparent->right);
                    if (uncle != nullptr && uncle->color == RED) {
                        parent->color = BLACK;
                        uncle->color = BLACK;
                        grandparent->color = RED;
                        node = grandparent;
                    } else {
                        if (node == static_cast<RBTreeNode<T>*>(parent->right)) {
                            node = parent;
                            parent = parent->leftRotate();
                            if (parent->parent == nullptr)
                                this->m_root = parent;
                        }
                        parent->color = BLACK;
                        grandparent->color = RED;
                        RBTreeNode<T>* newRoot = grandparent->rightRotate();
                        if (newRoot->parent == nullptr)
                            this->m_root = newRoot;
                    }
                } else {
                    RBTreeNode<T>* uncle = static_cast<RBTreeNode<T>*>(grandparent->left);
                    if (uncle != nullptr && uncle->color == RED) {
                        parent->color = BLACK;
                        uncle->color = BLACK;
                        grandparent->color = RED;
                        node = grandparent;
                    } else {
                        if (node == static_cast<RBTreeNode<T>*>(parent->left)) {
                            node = parent;
                            parent = parent->rightRotate();
                            if (parent->parent == nullptr)
                                this->m_root = parent;
                        }
                        parent->color = BLACK;
                        grandparent->color = RED;
                        RBTreeNode<T>* newRoot = grandparent->leftRotate();
                        if (newRoot->parent == nullptr)
                            this->m_root = newRoot;
                    }
                }
            }
            if (this->m_root != nullptr)
                static_cast<RBTreeNode<T>*>(this->m_root)->color = BLACK;
        }

        // Корректировка после удаления
        void fixDelete(RBTreeNode<T>* x, RBTreeNode<T>* xParent) {
            while ((x == nullptr || x->color == BLACK) && x != this->m_root) {
                if (x == static_cast<RBTreeNode<T>*>(xParent->left)) {
                    RBTreeNode<T>* w = static_cast<RBTreeNode<T>*>(xParent->right);
                    if (w != nullptr && w->color == RED) {
                        w->color = BLACK;
                        static_cast<RBTreeNode<T>*>(xParent)->color = RED;
                        RBTreeNode<T>* newParent = xParent->leftRotate();
                        if (newParent->parent == nullptr)
                            this->m_root = newParent;
                        w = static_cast<RBTreeNode<T>*>(xParent->right);
                    }
                    if ((w->left == nullptr || static_cast<RBTreeNode<T>*>(w->left)->color == BLACK) &&
                        (w->right == nullptr || static_cast<RBTreeNode<T>*>(w->right)->color == BLACK)) {
                        w->color = RED;
                        x = xParent;
                        xParent = static_cast<RBTreeNode<T>*>(xParent->parent);
                    } else {
                        if (w->right == nullptr || static_cast<RBTreeNode<T>*>(w->right)->color == BLACK) {
                            if (w->left != nullptr)
                                static_cast<RBTreeNode<T>*>(w->left)->color = BLACK;
                            w->color = RED;
                            RBTreeNode<T>* newW = w->rightRotate();
                            if (newW->parent == nullptr)
                                this->m_root = newW;
                            w = static_cast<RBTreeNode<T>*>(xParent->right);
                        }
                        w->color = static_cast<RBTreeNode<T>*>(xParent)->color;
                        static_cast<RBTreeNode<T>*>(xParent)->color = BLACK;
                        if (w->right != nullptr)
                            static_cast<RBTreeNode<T>*>(w->right)->color = BLACK;
                        RBTreeNode<T>* newParent = xParent->leftRotate();
                        if (newParent->parent == nullptr)
                            this->m_root = newParent;
                        x = static_cast<RBTreeNode<T>*>(this->m_root);
                        break;
                    }
                } else {
                    RBTreeNode<T>* w = static_cast<RBTreeNode<T>*>(xParent->left);
                    if (w != nullptr && w->color == RED) {
                        w->color = BLACK;
                        static_cast<RBTreeNode<T>*>(xParent)->color = RED;
                        RBTreeNode<T>* newParent = xParent->rightRotate();
                        if (newParent->parent == nullptr)
                            this->m_root = newParent;
                        w = static_cast<RBTreeNode<T>*>(xParent->left);
                    }
                    if ((w->left == nullptr || static_cast<RBTreeNode<T>*>(w->left)->color == BLACK) &&
                        (w->right == nullptr || static_cast<RBTreeNode<T>*>(w->right)->color == BLACK)) {
                        w->color = RED;
                        x = xParent;
                        xParent = static_cast<RBTreeNode<T>*>(xParent->parent);
                    } else {
                        if (w->left == nullptr || static_cast<RBTreeNode<T>*>(w->left)->color == BLACK) {
                            if (w->right != nullptr)
                                static_cast<RBTreeNode<T>*>(w->right)->color = BLACK;
                            w->color = RED;
                            RBTreeNode<T>* newW = w->leftRotate();
                            if (newW->parent == nullptr)
                                this->m_root = newW;
                            w = static_cast<RBTreeNode<T>*>(xParent->left);
                        }
                        w->color = static_cast<RBTreeNode<T>*>(xParent)->color;
                        static_cast<RBTreeNode<T>*>(xParent)->color = BLACK;
                        if (w->left != nullptr)
                            static_cast<RBTreeNode<T>*>(w->left)->color = BLACK;
                        RBTreeNode<T>* newParent = xParent->rightRotate();
                        if (newParent->parent == nullptr)
                            this->m_root = newParent;
                        x = static_cast<RBTreeNode<T>*>(this->m_root);
                        break;
                    }
                }
            }
            if (x != nullptr)
                x->color = BLACK;
        }

        // Вспомогательная функция: замена поддерева u поддеревом v
        void transplant(RBTreeNode<T>* u, RBTreeNode<T>* v) {
            if (u->parent == nullptr) {
                this->m_root = v;
            } else if (u == u->parent->left) {
                u->parent->left = v;
            } else {
                u->parent->right = v;
            }
            if (v != nullptr) {
                v->parent = u->parent;
            }
        }

        // Вспомогательная функция: поиск минимального узла в поддереве
        RBTreeNode<T>* minimum(RBTreeNode<T>* node) {
            while (node->left != nullptr) {
                node = static_cast<RBTreeNode<T>*>(node->left);
            }
            return node;
        }

        // Методы создания и уничтожения узлов с использованием rebinding аллокатора
        TreeNode<T>* create(const T& value) override {
            return this->BSTree<T, Comp, Alloc>::template create<RBTreeNode<T>>(m_node_allocator, value);
        }

        void destroy(TreeNode<T>* node) override {
            this->BSTree<T, Comp, Alloc>::template destroy<RBTreeNode<T>>(m_node_allocator, static_cast<RBTreeNode<T>*>(node));
        }

        NodeAllocatorType m_node_allocator;
    };

} // namespace trees
