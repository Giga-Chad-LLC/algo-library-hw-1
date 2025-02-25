//
// Created by Vladislav Artiukhov on 25.02.25.
//
#pragma once

#include <random>

#include "../Scenario/Scenario.h"
#include "../../components/Report/Report.h"
#include "../../components/ExecutionTimer/ExecutionTimer.h"


namespace playground {

    class Random final : public IScenario {
    public:
        /**
         *
         * @param size number of elements to insert into the tree.
         * @param seed seed for the pseudo-generator of integers
         * @param distribution_range range of the min and max values for the generator
         */
        explicit Random(
            std::size_t size = 200'000,
            uint32_t seed = 0,
            const std::pair<std::int32_t, std::int32_t> &distribution_range = {0, 250}
        );

        ~Random() override = default;

        /**
         * Generates a pseudo-random datasets of elements:
         *   1. insertion (elements are inserted into the tree)
         *   2. removal (elements are removed from the tree)
         *
         * The values are bounded making the higher probability of occurrence of equal elements.
         * The datasets are not stored in the memory separately but inserted element-wise in the tree,
         * promoting precise memory consumption analysis.
         */
        trees::testing::Report run(const std::string& tag, [[maybe_unused]] std::unique_ptr<trees::BSTree<int32_t>>& tree) override;

    private:
        const std::size_t m_size;
        std::mt19937 m_rng;
        std::uniform_int_distribution<std::int32_t> m_distribution;
    };

}