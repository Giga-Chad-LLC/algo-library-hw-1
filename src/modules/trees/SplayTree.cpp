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
    m_node_allocator(this->m_allocator) {}

  void insert(const T& value) override {}

  bool remove(const T& value) override {
    return false;
  }


  ~SplayTree() override {
    this->clear();
  }

private:
  void zig(SplayTreeNode<T>* x) {
    // TODO: too much copy-paste in if-else, refactor
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

  void zig_zig(SplayTreeNode<T>*) {

  }

  void zig_zag(SplayTreeNode<T>*) {

  }

  void splay(SplayTreeNode<T>*) {

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