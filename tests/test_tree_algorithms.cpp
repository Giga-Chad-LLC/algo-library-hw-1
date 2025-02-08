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

TYPED_TEST(ParameterizedTreeTester, BuildTreeFromPointer) {
    trees::test::TreeTester<TestFixture::template TreeType>::BuildTreeFromPointer();
}


TYPED_TEST(ParameterizedTreeTester, InsertIntoEmptyTree) {
    trees::test::TreeTester<TestFixture::template TreeType>::InsertIntoEmptyTree();
}


TYPED_TEST(ParameterizedTreeTester, InsertItems) {
    trees::test::TreeTester<TestFixture::template TreeType>::InsertItems();
}


TYPED_TEST(ParameterizedTreeTester, InsertDuplicates) {
    trees::test::TreeTester<TestFixture::template TreeType>::InsertDuplicates();
}

TYPED_TEST(ParameterizedTreeTester, ClearTree) {
    trees::test::TreeTester<TestFixture::template TreeType>::ClearTree();
}


TYPED_TEST(ParameterizedTreeTester, MultipleClear) {
    trees::test::TreeTester<TestFixture::template TreeType>::MultipleClear();
}


TYPED_TEST(ParameterizedTreeTester, RemoveItems) {
    trees::test::TreeTester<TestFixture::template TreeType>::RemoveItems();
}


TYPED_TEST(ParameterizedTreeTester, RemoveFromEmptyTree) {
    trees::test::TreeTester<TestFixture::template TreeType>::RemoveFromEmptyTree();
}


TYPED_TEST(ParameterizedTreeTester, RemoveDuplicates) {
    trees::test::TreeTester<TestFixture::template TreeType>::RemoveDuplicates();
}

