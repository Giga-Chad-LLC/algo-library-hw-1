#include <iostream>
#include <string>

#include <gtest/gtest.h>

import tree_algorithms;



TEST(AVLTreeTest, BuildEmptyTree) {
    trees::AVLTree<int> tree;
    tree.build(nullptr, 0);

    EXPECT_EQ(tree.count(0), 0);
}


TEST(AVLTreeTest, BuildTreeFromPointer) {
    trees::AVLTree<int> tree;

    int items[] = {1, 2, 3};
    const size_t n = sizeof(items) / sizeof(items[0]);

    tree.build(items, 3);

    for (int i = 0; i < n; i++) {
        EXPECT_EQ(tree.count(items[i]), 1);
    }
}



TEST(AVLTreeTest, InsertIntoEmptyTree) {
    trees::AVLTree<int> tree;

    std::vector<int> items = {1, 2, 3};

    for (int v : items) {
        tree.insert(v);
    }

    for (int v : items) {
        EXPECT_EQ(tree.count(v), 1);
    }
}


TEST(AVLTreeTest, InsertDuplicates) {
    trees::AVLTree<int> tree;

    std::vector<int> items = {1, 1, 2, 2, 3, 3};

    for (int v : items) {
        tree.insert(v);
    }

    for (int v : items) {
        EXPECT_EQ(tree.count(v), 2);
    }
}

TEST(AVLTreeTest, ClearTree) {
    trees::AVLTree<int> tree;

    std::vector<int> items = {1, 2, 3};

    for (int v : items) {
        tree.insert(v);
    }

    for (int v : items) {
        EXPECT_EQ(tree.count(v), 1);
    }

    tree.clear();

    for (int v : items) {
        EXPECT_EQ(tree.count(v), 0);
    }
}