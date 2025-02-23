//
// Created by Vladislav.Artiukhov on 08.02.25.
//

module;

#include <functional>
#include <vector>

export module tree_algorithms:SplayTree;
import :BSTree;
// nodes
import :TreeNode;
import :SplayTreeNode;

namespace trees {

using namespace nodes;


/**
 * An efficient self-balancing implementation of a binary search tree.
 * Complexity: `O(log N)` amortized for all operations.
 *
 * Credits: https://github.com/PetarV-/Algorithms/blob/master/Data%20Structures/Splay%20Tree.cpp
 */
export template<typename T, typename Comp = std::less<T>, typename Alloc = std::allocator<T>>
class SplayTree final : public BSTree<T, Comp, Alloc> {
  using NodeAllocatorType = typename std::allocator_traits<Alloc>::template rebind_alloc<SplayTreeNode<T>>;

public:
  explicit SplayTree(Comp comparator = Comp{}, Alloc allocator = Alloc{})
    : BSTree<T, Comp, Alloc>(comparator, allocator),
    m_node_allocator(this->m_allocator) {
    this->m_root = nullptr;
  }

  void insert(const T& value) override {
    if (this->m_root == nullptr) {
      this->m_root = create(value);
      return;
    }

    auto* curr = static_cast<SplayTreeNode<T>*>(this->m_root);

    while (curr != nullptr) {
      // value <= curr.value
      if (const bool greater = this->m_comparator(curr->value, value); !greater) {
        if (curr->left == nullptr) {
          auto* newNode = static_cast<SplayTreeNode<T>*>(create(value));

          curr->left = newNode;
          newNode->parent = curr;

          splay(newNode);
          return;
        }

        curr = static_cast<SplayTreeNode<T>*>(curr->left);
      }
      // value > curr.value
      else {
        if (curr->right == nullptr) {
          auto* newNode = static_cast<SplayTreeNode<T>*>(create(value));

          curr->right = newNode;
          newNode->parent = curr;

          splay(newNode);
          return;
        }

        curr = static_cast<SplayTreeNode<T>*>(curr->right);
      }
    }
  }

  bool remove(const T& value) override {
    SplayTreeNode<T>* del = find(value);
    if (del == nullptr) {
      return false;
    }

    auto* L = static_cast<SplayTreeNode<T>*>(del->left);
    auto* R = static_cast<SplayTreeNode<T>*>(del->right);

    if (L == nullptr && R == nullptr) {
      this->m_root = nullptr;
    }
    else if (L == nullptr) {
      SplayTreeNode<T>* M = subtree_min(R);
      splay(M);
    }
    else if (R == nullptr) {
      SplayTreeNode<T>* M = subtree_max(L);
      splay(M);
    }
    else {
      SplayTreeNode<T>* M = subtree_max(L);
      splay(M);

      M->right = R;
      R->parent = M;
    }

    // detach `del` from its parent if it wasn't root
    if (del->parent != nullptr) {
      if (del->parent->left == del) {
        del->parent->left = nullptr;
      }
      else if (del->parent->right == del) {
        del->parent->right = nullptr;
      }
      del->parent = nullptr;
    }

    destroy(del);
    return true;
  }


  ~SplayTree() override {
    this->clear();
  }

private:
  SplayTreeNode<T>* find(const T& value) {
    SplayTreeNode<T>* ret = nullptr;
    auto* curr = static_cast<SplayTreeNode<T>*>(this->m_root);
    SplayTreeNode<T>* prev = nullptr;

    while (curr != nullptr) {
      prev = curr;

      // value <= curr.value
      if (const bool greater = this->m_comparator(curr->value, value); !greater) {
        // value >= curr.value
        if (const bool less = this->m_comparator(value, curr->value); !less) {
          // implies: value == curr.value
          ret = curr;
          break;
        }
        curr = static_cast<SplayTreeNode<T>*>(curr->left);
      }
      // value > curr.value
      else {
        curr = static_cast<SplayTreeNode<T>*>(curr->right);
      }
    }

    if (ret != nullptr) {
      splay(ret);
    }
    else if (prev != nullptr) {
      splay(prev);
    }

    return ret;
  }

  SplayTreeNode<T>* subtree_max(SplayTreeNode<T>* subRoot) {
    SplayTreeNode<T>* curr = subRoot;
    while (curr->right != nullptr) {
      curr = static_cast<SplayTreeNode<T>*>(curr->right);
    }
    return curr;
  }

  SplayTreeNode<T>* subtree_min(SplayTreeNode<T>* subRoot) {
    SplayTreeNode<T>* curr = subRoot;
    while (curr->left != nullptr) {
      curr = static_cast<SplayTreeNode<T>*>(curr->left);
    }
    return curr;
  }

  void zig(SplayTreeNode<T>* x) {
    auto* p = static_cast<SplayTreeNode<T>*>(x->parent);

    if (p->left == x) {
      auto* x_right = static_cast<SplayTreeNode<T>*>(x->right);

      x->parent = nullptr;
      x->right = p;

      p->parent = x;
      p->left = x_right;

      if (x_right != nullptr) {
        x_right->parent = p;
      }
    }
    else {
      auto* x_left = static_cast<SplayTreeNode<T>*>(x->left);

      x->parent = nullptr;
      x->left = p;

      p->parent = x;
      p->right = x_left;

      if (x_left != nullptr) {
        x_left->parent = p;
      }
    }
  }

  void zig_zig(SplayTreeNode<T>* x) {
      SplayTreeNode<T>* p = x->parent;
      SplayTreeNode<T>* g = p->parent;

      if (p->left == x) {
        auto* x_right = static_cast<SplayTreeNode<T>*>(x->right);
        auto* p_right = static_cast<SplayTreeNode<T>*>(p->right);

        x->parent = g->parent;
        x->right = p;

        p->parent = x;
        p->left = x_right;
        p->right = g;

        g->parent = p;
        g->left = p_right;

        if (x->parent != nullptr) {
          if (x->parent->left == g) {
            x->parent->left = x;
          }
          else {
            x->parent->right = x;
          }
        }

        if (x_right != nullptr) {
          x_right->parent = p;
        }

        if (p_right != nullptr) {
          p_right->parent = g;
        }
      }
      else {
        auto* p_left = static_cast<SplayTreeNode<T>*>(p->left);
        auto* x_left = static_cast<SplayTreeNode<T>*>(x->left);

        x->parent = g->parent;
        x->left = p;

        p->parent = x;
        p->left = g;
        p->right = x_left;

        g->parent = p;
        g->right = p_left;

        if (x->parent != nullptr) {
          if (x->parent->left == g) {
            x->parent->left = x;
          }
          else {
            x->parent->right = x;
          }
        }

        if (p_left != nullptr) {
          p_left->parent = g;
        }

        if (x_left != nullptr) {
          x_left->parent = p;
        }
      }
  }

  void zig_zag(SplayTreeNode<T>* x) {
    auto* p = static_cast<SplayTreeNode<T>*>(x->parent);
    auto* g = static_cast<SplayTreeNode<T>*>(p->parent);

    if (p->right == x) {
      auto* x_left = static_cast<SplayTreeNode<T>*>(x->left);
      auto* x_right = static_cast<SplayTreeNode<T>*>(x->right);

      x->parent = g->parent;
      x->left = p;
      x->right = g;

      p->parent = x;
      p->right = x_left;

      g->parent = x;
      g->left = x_right;

      if (x->parent != nullptr) {
        if (x->parent->left == g) {
          x->parent->left = x;
        }
        else {
          x->parent->right = x;
        }
      }

      if (x_left != nullptr) {
        x_left->parent = p;
      }

      if (x_right != nullptr) {
        x_right->parent = g;
      }
    }
    else {
      auto* x_left = static_cast<SplayTreeNode<T>*>(x->left);
      auto* x_right = static_cast<SplayTreeNode<T>*>(x->right);

      x->parent = g->parent;
      x->left = g;
      x->right = p;

      p->parent = x;
      p->left = x_right;

      g->parent = x;
      g->right = x_left;

      if (x->parent != nullptr) {
        if (x->parent->left == g) {
          x->parent->left = x;
        }
        else {
          x->parent->right = x;
        }
      }

      if (x_left != nullptr) {
        x_left->parent = g;
      }
      if (x_right != nullptr) {
        x_right->parent = p;
      }
    }
  }

  void splay(SplayTreeNode<T>* x) {
    while (x->parent != nullptr) {
      auto* p = static_cast<SplayTreeNode<T>*>(x->parent);
      auto* g = static_cast<SplayTreeNode<T>*>(p->parent);

      if (g == nullptr) {
        zig(x);
      }
      else if (g->left == p && p->left == x) {
        zig_zig(x);
      }
      else if (g->right == p && p->right == x) {
        zig_zig(x);
      }
      else {
        zig_zag(x);
      }
    }

    this->m_root = x;
  }

  TreeNode<T>* create(const T& value) override {
    return this->BSTree<T, Comp, Alloc>::template create<SplayTreeNode<T>>(m_node_allocator, value);
  }

  void destroy(TreeNode<T>* node) override {
    this->BSTree<T, Comp, Alloc>::template destroy<SplayTreeNode<T>>(m_node_allocator, static_cast<SplayTreeNode<T>*>(node));
  }

  NodeAllocatorType m_node_allocator;
};


}