#include <iostream>
#include <random>
#include <string>

#include "components/TreeTester.h"
#include "components/ParameterizedTreeTests.h"
#include "components/Report/Report.h"

#include <gtest/gtest.h>

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
    trees.emplace_back("TreapTree<int>", std::make_unique<trees::TreapTree<int>>());

    constexpr int seed = 42;
    std::mt19937 rng(seed);
    std::uniform_int_distribution distribution(1, 1'000'000);

    constexpr size_t n = 1'000'000;
    std::vector<int> numbers(n);

    for (size_t i = 0; i < n; ++i) {
        numbers[i] = distribution(rng);
    }


    trees::testing::ExecutionTimer timer;
    trees::testing::Report report;

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

        report << "=== Report for '" << name << "' ===\n";
        report << "Operations total: " << n << "\n";
        report << "\tInsert: " << insertCount << "\n";
        report << "\tRemove: " << removeCount << "\n";

        report << "Total time: " << elapsedTotal << "ms\n";

        report << "Total insert time: " << elapsedInsert << "ms, "
                  << "Average insert time: " << elapsedInsert / static_cast<double>(insertCount) << "ms\n";

        report << "Total remove time: " << elapsedRemove << "ms, "
                  << "Average remove time: " << elapsedRemove / static_cast<double>(removeCount) << "ms\n";

        report << "\n";
    }

    std::cout << report.str() << std::endl;
    // report.dump("report.txt");
}
