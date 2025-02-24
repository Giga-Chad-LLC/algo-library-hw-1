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
        explicit Random(
            const std::size_t size = 1000,
            const std::int64_t seed = 0,
            const std::pair<std::int64_t, std::int64_t> &distribution_range = {0, 100}
        ) : m_size(size), m_rng(seed), m_distribution(distribution_range.first, distribution_range.second) {}

        ~Random() override = default;

        trees::testing::Report run(const std::string& tag, std::unique_ptr<trees::BSTree<int>>& tree) override {
            trees::testing::ExecutionTimer timer;
            trees::testing::Report report;

            timer.start(tag);
            // ...
            const long double elapsedTotal = timer.finish(tag);

            report << "=== Report for '" << tag << "' ===\n";
            report << "Operations total: " << m_size << "\n";
            // report << "\tInsert: " << insertCount << "\n";
            // report << "\tRemove: " << removeCount << "\n";

            report << "Total time: " << elapsedTotal << "ms\n";

            // report << "Total insert time: " << elapsedInsert << "ms, "
            //           << "Average insert time: " << elapsedInsert / static_cast<double>(insertCount) << "ms\n";
            //
            // report << "Total remove time: " << elapsedRemove << "ms, "
            //           << "Average remove time: " << elapsedRemove / static_cast<double>(removeCount) << "ms\n";

            report << "\n";

            return report;
        }

    private:
        const std::size_t m_size;
        std::mt19937 m_rng;
        const std::uniform_int_distribution<std::int64_t> m_distribution;
    };

}