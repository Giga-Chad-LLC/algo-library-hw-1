#pragma once

#include <gtest/gtest.h>

#include "SimpleAllocator/SimpleAllocator.h"

import tree_algorithms;

namespace trees::test {
    using namespace trees;

template<template <typename T, typename Comp = std::less<T>, typename Alloc = std::allocator<T>> class TreeType>
class TreeTester {
public:
    static void BuildEmptyTree() {
        TreeType<int> tree;
        tree.build(nullptr, 0);

        EXPECT_EQ(tree.count(0), 0);
    }

    static void BuildTreeFromPointer() {
        TreeType<int> tree;

        int items[] = {1, 2, 3};
        constexpr size_t n = std::size(items);

        tree.build(items, 3);

        for (size_t i = 0; i < n; i++) {
            EXPECT_EQ(tree.count(items[i]), 1);
        }
    }

    static void InsertIntoEmptyTree() {
        TreeType<int> tree;

        std::vector<int> items = {1, 2, 3};

        for (int v : items) {
            tree.insert(v);
        }

        for (int v : items) {
            EXPECT_EQ(tree.count(v), 1);
        }
    }

    static void InsertItems() {
        TreeType<int> tree;

        const int n = 1000;

        for (int i = 0; i < n; i++) {
            tree.insert(i);
        }

        for (int i = 0; i < n; i++) {
            EXPECT_EQ(tree.count(i), 1);
        }
    }

    static void InsertDuplicates() {
        TreeType<int> tree;

        const std::vector items = {1, 1, 2, 2, 3, 3};

        for (int v : items) {
            tree.insert(v);
        }

        for (int v : items) {
            EXPECT_EQ(tree.count(v), 2);
        }
    }

    static void ClearTree() {
        TreeType<int> tree;

        const std::vector items = {1, 2, 3};

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

    static void MultipleClear() {
        TreeType<int> tree;
        const std::vector<int> items = {1, 2, 3};

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

    static void RemoveItems() {
        TreeType<int> tree;

        const std::vector<int> items = {1, 2, 3};

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

    static void RemoveFromEmptyTree() {
        TreeType<int> tree;
        EXPECT_FALSE(tree.remove(12));
    }

    static void RemoveDuplicates() {
        TreeType<int> tree;

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

    static void UseCustomAllocator() {
        // SimpleAllocatorCounter::count is a static variable, which tracks the allocations done by SimpleAllocator<>
        // with any template type. It must be zero on the test entrances, otherwise
        // it would mean that the allocator does deallocate some tree nodes, which would cause
        // memory leaks.
        EXPECT_EQ(testing::SimpleAllocatorCounter::count, 0);

        TreeType<int, std::less<int>, testing::SimpleAllocator<int>> tree{
            std::less<int>(),
            testing::SimpleAllocator<int>()
        };

        const int n = 1'000;

        for (int i = 0; i < n; i++) {
            tree.insert(i);
        }

        EXPECT_EQ(testing::SimpleAllocatorCounter::count, n);

        for (int i = 0; i < n; i++) {
            EXPECT_EQ(tree.count(i), 1);
            EXPECT_TRUE(tree.remove(i));
        }

        EXPECT_EQ(testing::SimpleAllocatorCounter::count, 0);
    }
};

}