//
// Created by Vladislav.Artiukhov on 08.02.25.
//

#ifndef REPORT_H
#define REPORT_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>


namespace trees::testing {

class Report {
public:
    template <typename T>
    Report& operator<<(const T& data) {
        buffer << data;
        return *this;
    }

    void dump(const std::string& filepath) const;

    void clear();

    // Allow accessing the content as a string (optional)
    std::string str() const;

private:
    std::ostringstream buffer;
};

}

#endif //REPORT_H
