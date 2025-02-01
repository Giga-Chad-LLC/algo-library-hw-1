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

    for (size_t i = 0; i < n; i++) {
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


TEST(AVLTreeTest, InsertItems) {
    trees::AVLTree<int> tree;

    const int n = 1000;

    for (int i = 0; i < n; i++) {
        tree.insert(i);
    }

    for (int i = 0; i < n; i++) {
        EXPECT_EQ(tree.count(i), 1);
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


TEST(AVLTreeTest, MultipleClear) {
    trees::AVLTree<int> tree;
    std::vector<int> items = {1, 2, 3};

    for (int i = 0; i < 10; i++) {
        for (int v : items) {
            tree.insert(v);
        }

        tree.clear();
        tree.clear();

        for (int v : items) {
            EXPECT_EQ(tree.count(v), 0);
        }
    }
}


TEST(AVLTreeTest, RemoveItems) {
    trees::AVLTree<int> tree;

    std::vector<int> items = {1, 2, 3};

    for (int v : items) {
        tree.insert(v);
    }

    for (int v : items) {
        EXPECT_EQ(tree.count(v), 1);
    }

    for (int v : items) {
        bool removed = tree.remove(v);
        EXPECT_TRUE(removed);
    }

    for (int v : items) {
        EXPECT_EQ(tree.count(v), 0);
    }
}


TEST(AVLTreeTest, RemoveFromEmptyTree) {
    trees::AVLTree<int> tree;
    EXPECT_FALSE(tree.remove(12));
}


TEST(AVLTreeTest, RemoveDuplicates) {
    trees::AVLTree<int> tree;

    const int n = 10;

    for (int i = 0; i < n; i++) {
        tree.insert(1);
        tree.insert(2);
    }

    EXPECT_EQ(tree.count(1), n);
    EXPECT_EQ(tree.count(2), n);

    for (int i = 0; i < n; i++) {
        EXPECT_TRUE(tree.remove(1));
        EXPECT_TRUE(tree.remove(2));

        const int expected = n - i - 1;

        EXPECT_EQ(tree.count(1), expected);
        EXPECT_EQ(tree.count(2), expected);
    }
}

