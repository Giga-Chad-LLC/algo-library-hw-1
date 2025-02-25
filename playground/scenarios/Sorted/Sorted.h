//
// Created by Vladislav Artiukhov on 25.02.25.
//

#pragma once

#include <random>

#include "../Scenario/Scenario.h"
#include "../../components/Report/Report.h"
#include "../../components/ExecutionTimer/ExecutionTimer.h"

namespace playground {
    class Sorted final : public IScenario {
    public:
        explicit Sorted(
            std::int32_t start = 0,
            std::size_t size = 100'000,
            std::uint32_t seed = 0
        );

        trees::testing::Report run(const std::string &tag, std::unique_ptr<trees::BSTree<int32_t>> &tree) override;
    private:
        const std::int32_t m_start;
        const std::size_t m_size;
        std::mt19937 m_rng;
    };
}
