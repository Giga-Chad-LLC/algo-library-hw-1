#include <iostream>
#include <string>

#include <gtest/gtest.h>

import tree_algorithms;


TEST(TreeAlgorithmsTest, PrintMessage) {
    // Capture output and validate
    testing::internal::CaptureStdout();
    trees::printMessage(20);
    std::string output = testing::internal::GetCapturedStdout();

    EXPECT_EQ(output, "Hello from tree_algorithms module! 20\n");
}

TEST(TreeAlgorithmsTest, AVLTree) {
    trees::AVLTree<int> tree;
}