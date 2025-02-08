//
// Created by Vladislav.Artiukhov on 08.02.25.
//

module;

#include <memory>

export module tree_algorithms:SplayTreeNode;
import :TreeNode;

namespace trees::nodes {

template <class T>
struct SplayTreeNode final : public TreeNode<T> {
  using TreeNode<T>::TreeNode;

  SplayTreeNode* parent = nullptr;
};

}