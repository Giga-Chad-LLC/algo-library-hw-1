//
// Created by Vladislav.Artiukhov on 08.02.25.
//

module;

#include <functional>
#include <memory>
#include <vector>
#include <algorithm>

export module tree_algorithms:SplayTree;
import :BSTree;
// nodes
import :TreeNode;
import :SplayTreeNode;

namespace trees {

using namespace nodes;

export template<typename T, typename Comp = std::less<T>, typename Alloc = std::allocator<T>>
class SplayTree final : public BSTree<T, Comp, Alloc> {
  using NodeAllocatorType = typename std::allocator_traits<Alloc>::template rebind_alloc<SplayTreeNode<T>>;

public:
  explicit SplayTree(Comp comparator = Comp{}, Alloc allocator = Alloc{})
    : BSTree<T, Comp, Alloc>(comparator, allocator),
    m_node_allocator(this->m_allocator) {
    this->m_root = nullptr;
  }

  void insert(const T& value) override {}

  bool remove(const T& value) override {
    return false;
  }


  ~SplayTree() override {
    this->clear();
  }

private:
  // TODO: too much copy-paste in if-else branches, refactor

  void zig(SplayTreeNode<T>* x) {
    auto* p = static_cast<SplayTreeNode<T>*>(x->parent);

    if (p->left == x) {
      auto* A = static_cast<SplayTreeNode<T>*>(x->left);
      auto* B = static_cast<SplayTreeNode<T>*>(x->right);
      auto* C = static_cast<SplayTreeNode<T>*>(p->right);

      x->parent = nullptr;
      x->right = p;

      p->parent = x;
      p->left = B;

      if (B != nullptr) {
        B->parent = p;
      }
    }
    else {
      auto* A = static_cast<SplayTreeNode<T>*>(p->left);
      auto* B = static_cast<SplayTreeNode<T>*>(x->left);
      auto* C = static_cast<SplayTreeNode<T>*>(x->right);

      x->parent = nullptr;
      x->left = p;

      p->parent = x;
      p->right = B;

      if (B != nullptr) {
        B->parent = p;
      }
    }
  }

  void zig_zig(SplayTreeNode<T>* x) {
      SplayTreeNode<T>* p = x->parent;
      SplayTreeNode<T>* g = p->parent;

      if (p -> left == x) {
        auto* A = static_cast<SplayTreeNode<T>*>(x->left);
        auto* B = static_cast<SplayTreeNode<T>*>(x->right);
        auto* C = static_cast<SplayTreeNode<T>*>(p->right);
        auto* D = static_cast<SplayTreeNode<T>*>(g->right);

        x->parent = g->parent;
        x->right = p;

        p->parent = x;
        p->left = B;
        p->right = g;

        g->parent = p;
        g->left = C;

        if (x->parent != nullptr) {
          if (x->parent->left == g) {
            x->parent->left = x;
          }
          else {
            x->parent->right = x;
          }
        }

        if (B != nullptr) {
          B->parent = p;
        }

        if (C != nullptr) {
          C->parent = g;
        }
      }
      else {
        auto* A = static_cast<SplayTreeNode<T>*>(g->left);
        auto* B = static_cast<SplayTreeNode<T>*>(p->left);
        auto* C = static_cast<SplayTreeNode<T>*>(x->left);
        auto* D = static_cast<SplayTreeNode<T>*>(x->right);

        x->parent = g->parent;
        x->left = p;

        p->parent = x;
        p->left = g;
        p->right = C;

        g->parent = p;
        g->right = B;

        if (x->parent != nullptr) {
          if (x->parent->left == g) {
            x->parent->left = x;
          }
          else {
            x->parent->right = x;
          }
        }

        if (B != nullptr) {
          B->parent = g;
        }

        if (C != nullptr) {
          C->parent = p;
        }
      }
  }

  void zig_zag(SplayTreeNode<T>* x) {
    auto* p = static_cast<SplayTreeNode<T>*>(x->parent);
    auto* g = static_cast<SplayTreeNode<T>*>(p->parent);

    if (p -> right == x) {
      auto* A = static_cast<SplayTreeNode<T>*>(p->left);
      auto* B = static_cast<SplayTreeNode<T>*>(x->left);
      auto* C = static_cast<SplayTreeNode<T>*>(x->right);
      auto* D = static_cast<SplayTreeNode<T>*>(g->right);

      x->parent = g->parent;
      x->left = p;
      x->right = g;

      p->parent = x;
      p->right = B;

      g->parent = x;
      g->left = C;

      if (x->parent != nullptr) {
        if (x->parent->left == g) {
          x->parent->left = x;
        }
        else {
          x->parent->right = x;
        }
      }

      if (B != nullptr) {
        B->parent = p;
      }

      if (C != nullptr) {
        C->parent = g;
      }
    }
    else {
      auto* A = static_cast<SplayTreeNode<T>*>(g->left);
      auto* B = static_cast<SplayTreeNode<T>*>(x->left);
      auto* C = static_cast<SplayTreeNode<T>*>(x->right);
      auto* D = static_cast<SplayTreeNode<T>*>(p->right);

      x->parent = g->parent;
      x->left = g;
      x->right = p;

      p->parent = x;
      p->left = C;

      g->parent = x;
      g->right = B;

      if (x->parent != nullptr) {
        if (x->parent->left == g) {
          x->parent->left = x;
        }
        else {
          x->parent->right = x;
        }
      }

      if (B != nullptr) {
        B->parent = g;
      }
      if (C != nullptr) {
        C->parent = p;
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