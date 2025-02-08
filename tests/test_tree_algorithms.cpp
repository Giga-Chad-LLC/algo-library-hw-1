#include <iostream>
#include <string>

#include "TreeTester.h"
#include "ParameterizedTreeTests.h"

#include <gtest/gtest.h>

import tree_algorithms;


// define test suite parameterized by tree type `TreeImpl<T>`
TYPED_TEST_SUITE(ParameterizedTreeTester, TreeTypeTemplates);


TYPED_TEST(ParameterizedTreeTester, BuildEmptyTree) {
    // Select the tree type and execute the test
    trees::test::TreeTester<TestFixture::template TreeType>::BuildEmptyTree();
}








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

