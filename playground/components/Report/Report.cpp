//
// Created by Vladislav.Artiukhov on 08.02.25.
//

#include "Report.h"

namespace trees::testing {

void Report::dump(const std::string& filepath) const {
    std::ofstream file(filepath);
    if (!file) {
        throw std::runtime_error("Failed to open file: " + filepath);
    }
    file << buffer.str();
}

void Report::clear() {
    buffer.str("");
    buffer.clear();
}

// Allow accessing the content as a string (optional)
std::string Report::str() const {
    return buffer.str();
}

}