//
// Created by Vladislav Artiukhov on 25.02.25.
//
#pragma once

import tree_algorithms;

#include <string>
#include <memory>

#include "../../components/Report/Report.h"

namespace playground {

    class IScenario {
    public:
        virtual ~IScenario() = default;

        virtual trees::testing::Report run(const std::string& tag, std::unique_ptr<trees::BSTree<int>>& tree) = 0;
    };

}