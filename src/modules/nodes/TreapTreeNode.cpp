//
// Created by dmitriiart on 2/10/25.
//


module;

#include <random>
#include <limits>


export module tree_algorithms:TreapTreeNode;
import :TreeNode;

namespace trees::nodes {

    template <class T>
    struct TreapTreeNode final : public TreeNode<T> {
        explicit TreapTreeNode(const T& value): TreeNode<T>(value), rank(genRank()) {}
        int rank;

    private:
        static int genRank() {
            return distrib(gen);
        }
        inline static std::random_device rd;
        inline static std::mt19937 gen{rd()};
        inline static std::uniform_int_distribution<> distrib{
            std::numeric_limits<int>::min(),
            std::numeric_limits<int>::max()
        };
    };

}