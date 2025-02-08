//
// Created by Vladislav.Artiukhov on 08.02.25.
//

#ifndef PARAMETERIZEDTREETESTS_H
#define PARAMETERIZEDTREETESTS_H

#include <gtest/gtest.h>

import tree_algorithms;


template <typename TreeType>
class ParameterizedTreeTests : public ::testing::Test {};

// Define the tree types to test (extend this list as needed)
// NOTE: extend these templates according to support testing for other tree types
template <typename T> using AVLTreeType = trees::AVLTree<T>;
template <typename T> using SplayTreeType = trees::SplayTree<T>;


template <template <typename> class TreeTemplate>
struct TreeTemplateWrapper {
    // Aliases the tree type with a specific type `T`
    template <typename T> using Type = TreeTemplate<T>;
};

using TreeTypeTemplates = ::testing::Types<
    TreeTemplateWrapper<AVLTreeType>,
    TreeTemplateWrapper<SplayTreeType>
>;

template <typename TreeWrapper>
class ParameterizedTreeTester : public ::testing::Test {
protected:
    // Define the alias for the unwrapped tree type
    // Actual tree type for testing
    template <typename T> using TreeType = typename TreeWrapper::template Type<T>;
};


#endif //PARAMETERIZEDTREETESTS_H
