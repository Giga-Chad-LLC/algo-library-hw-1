#include <iostream>
#include <string>

#include "TreeTester.h"
#include <gtest/gtest.h>

import tree_algorithms;


TEST(AVLTreeTest, BuildEmptyTree) {
    trees::test::TreeTester<trees::AVLTree>::BuildEmptyTree();
}


TEST(AVLTreeTest, BuildTreeFromPointer) {
    trees::test::TreeTester<trees::AVLTree>::BuildTreeFromPointer();
}


TEST(AVLTreeTest, InsertIntoEmptyTree) {
    trees::test::TreeTester<trees::AVLTree>::InsertIntoEmptyTree();
}


TEST(AVLTreeTest, InsertItems) {
    trees::test::TreeTester<trees::AVLTree>::InsertItems();
}


TEST(AVLTreeTest, InsertDuplicates) {
    trees::test::TreeTester<trees::AVLTree>::InsertDuplicates();
}

TEST(AVLTreeTest, ClearTree) {
    trees::test::TreeTester<trees::AVLTree>::ClearTree();
}


TEST(AVLTreeTest, MultipleClear) {
    trees::test::TreeTester<trees::AVLTree>::MultipleClear();
}


TEST(AVLTreeTest, RemoveItems) {
    trees::test::TreeTester<trees::AVLTree>::RemoveItems();
}


TEST(AVLTreeTest, RemoveFromEmptyTree) {
    trees::test::TreeTester<trees::AVLTree>::RemoveFromEmptyTree();
}


TEST(AVLTreeTest, RemoveDuplicates) {
    trees::test::TreeTester<trees::AVLTree>::RemoveDuplicates();
}

