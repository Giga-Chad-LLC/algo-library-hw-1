#include <iostream>
#include <random>
#include <string>

#include "components/TreeTester.h"
#include "components/ParameterizedTreeTests.h"

#include <gtest/gtest.h>
#include <__random/random_device.h>

#include "components/ExecutionTimer/ExecutionTimer.h"

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


// TODO: find better place
TEST(TreesBenchmark, MeasureExecutionTimes) {
    using Tree = std::unique_ptr<trees::BSTree<int>>;

    std::vector<std::pair<std::string, Tree>> trees;
    trees.emplace_back("AVLTree<int>", std::make_unique<trees::AVLTree<int>>());
    trees.emplace_back("SplayTree<int>", std::make_unique<trees::SplayTree<int>>());


    constexpr int seed = 42;
    std::mt19937 rng(seed);
    std::uniform_int_distribution distribution(1, 1'000'000);

    constexpr size_t n = 100'000;
    std::vector<int> numbers(n);

    for (size_t i = 0; i < n; ++i) {
        numbers[i] = distribution(rng);
    }


    trees::testing::ExecutionTimer timer;

    // Insert/Remove operations based on parity of generated numbers
    for (auto& [name, tree] : trees) {
        long double elapsedInsert = 0.;
        long double elapsedRemove = 0.;
        long insertCount = 0;
        long removeCount = 0;

        timer.start(name);
        for (const int num : numbers) {
            if (num % 2 == 0) {
                ++insertCount;
                // Even: Insert into the tree
                timer.start("insert");
                tree->insert(num);
                elapsedInsert += timer.finish("insert");
            } else {
                ++removeCount;
                // Odd: Remove from the tree
                timer.start("remove");
                tree->remove(num);
                elapsedRemove += timer.finish("remove");
            }
        }
        const long double elapsedTotal = timer.finish(name);

        std::cout << "=== Report for '" << name << "' ===" << std::endl;
        std::cout << "Operations total: " << n << std::endl;
        std::cout << "\tInsert: " << insertCount << std::endl;
        std::cout << "\tRemove: " << removeCount << std::endl;

        std::cout << "Total time: " << elapsedTotal << "ms" << std::endl;

        std::cout << "Total insert time: " << elapsedInsert << "ms, "
                  << "Average insert time: " << elapsedInsert / static_cast<double>(insertCount) << "ms" << std::endl;

        std::cout << "Total remove time: " << elapsedRemove << "ms, "
                  << "Average remove time: " << elapsedRemove / static_cast<double>(removeCount) << "ms" << std::endl;
        std::cout << std::endl;
    }
}
