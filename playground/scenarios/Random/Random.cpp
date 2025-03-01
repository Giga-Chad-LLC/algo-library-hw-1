//
// Created by Vladislav Artiukhov on 25.02.25.
//
#include "Random.h"


namespace playground {

    Random::Random(
        const std::size_t size,
        const uint32_t seed,
        const std::pair<std::int32_t, std::int32_t> &distribution_range
    ) : m_size(size), m_rng(seed), m_distribution(distribution_range.first, distribution_range.second) {}

    trees::testing::Report Random::run(const std::string& tag, [[maybe_unused]] std::unique_ptr<trees::BSTree<int32_t>>& tree) {
        trees::testing::ExecutionTimer timer;
        trees::testing::Report report;

        const std::string insertion_tag = "INSERTION";
        const std::string removal_tag = "REMOVAL";

        long double elapsed_insertion = 0;
        long double elapsed_removal = 0;

        // start benchmarking
        timer.start(tag);

        // 1. insert values
        for (size_t i = 0; i < m_size; ++i) {
            int32_t value = m_distribution(m_rng);

            timer.start(insertion_tag);
            tree->insert(value);
            elapsed_insertion += timer.finish(insertion_tag);
        }

        // 2.remove values
        size_t removals_succeeded = 0;
        for (size_t i = 0; i < m_size; ++i) {
            int32_t value = m_distribution(m_rng);

            timer.start(removal_tag);
            removals_succeeded += tree->remove(value);
            elapsed_removal += timer.finish(removal_tag);
        }

        // reporting time
        const long double elapsed_total = timer.finish(tag);

        report << "=== Report for '" << tag << "', scenario '" << "RANDOM" << "' ===\n";
        // operations
        // insertion
        report << insertion_tag << ":"
            << "\n\tOperations executed: " << m_size
            << "\n\tTotal time elapsed: " << elapsed_insertion << "ms"
            << "\n\tAvg. time per operation: " << elapsed_insertion / static_cast<long double>(m_size) << "ms\n";
        // removal
        report << removal_tag << ":"
            << "\n\tOperations executed: " << m_size
            << "\n\tSucceeded (removed an element): " << removals_succeeded
            << "\n\tTotal time elapsed: " << elapsed_removal << "ms"
            << "\n\tAvg. time per operation: " << elapsed_removal / static_cast<long double>(m_size) << "ms\n";

        report << "Total time: " << elapsed_total << "ms\n\n";

        return report;
    }

}