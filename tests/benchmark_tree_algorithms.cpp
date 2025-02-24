//
// Created by Vladislav Artiukhov on 24.02.25.
//

#include <iostream>
#include <random>
#include <string>

#include "components/TreeTester.h"
#include "components/ParameterizedTreeTests.h"
#include "components/Report/Report.h"

#include <gtest/gtest.h>

#include "components/ExecutionTimer/ExecutionTimer.h"

import tree_algorithms;


TEST(TreesBenchmark, MeasureExecutionTimes) {
    using Tree = std::unique_ptr<trees::BSTree<int>>;

    std::vector<std::pair<std::string, Tree>> trees;
    trees.emplace_back("AVLTree<int>", std::make_unique<trees::AVLTree<int>>());
    trees.emplace_back("SplayTree<int>", std::make_unique<trees::SplayTree<int>>());
    trees.emplace_back("TreapTree<int>", std::make_unique<trees::TreapTree<int>>());
    trees.emplace_back("RBTree<int>", std::make_unique<trees::RBTree<int>>());

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