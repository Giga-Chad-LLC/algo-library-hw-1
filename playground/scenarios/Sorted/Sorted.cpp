//
// Created by Vladislav Artiukhov on 25.02.25.
//

#include "Sorted.h"

#include <numeric>
#include <algorithm>

namespace playground {

    Sorted::Sorted(
        const std::int32_t start,
        const std::size_t size,
        const std::uint32_t seed
    ) : m_start(start), m_size(size), m_rng(seed) {}

    trees::testing::Report Sorted::run(const std::string &tag, std::unique_ptr<trees::BSTree<int32_t>> &tree) {
        trees::testing::ExecutionTimer timer;
        trees::testing::Report report;

        const std::string insertion_tag = "INSERTION";
        const std::string removal_tag = "REMOVAL";

        long double elapsed_insertion = 0;
        long double elapsed_removal = 0;

        std::vector<int32_t> values(m_size);
        std::iota(std::begin(values), std::end(values), m_start);

        std::vector<int32_t> shuffled = values;
        std::ranges::shuffle(shuffled, m_rng);

        // start benchmarking
        timer.start(tag);

        // 1. insert values
        for (int32_t value : values) {
            timer.start(insertion_tag);
            tree->insert(value);
            elapsed_insertion += timer.finish(insertion_tag);
        }

        // 2.remove values
        size_t removals_succeeded = 0;
        for (int32_t value : shuffled) {
            timer.start(removal_tag);
            removals_succeeded += tree->remove(value);
            elapsed_removal += timer.finish(removal_tag);
        }

        // reporting time
        const long double elapsed_total = timer.finish(tag);

        report << "=== Report for '" << tag << "' ===\n";
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
