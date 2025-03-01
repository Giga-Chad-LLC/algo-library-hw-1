//
// Created by Vladislav.Artiukhov on 08.02.25.
//

#ifndef EXECUTIONTIMER_H
#define EXECUTIONTIMER_H


#include <iostream>
#include <string>
#include <unordered_map>
#include <chrono>
#include <stdexcept>



namespace trees::testing {

class ExecutionTimer {
public:
    using Clock = std::chrono::high_resolution_clock;
    // Starts a new named measuring session
    void start(const std::string& name);
    // Finishes the measuring session and returns the elapsed time in milliseconds
    double finish(const std::string& name);

private:
    std::unordered_map<std::string, Clock::time_point> sessions; // Stores session start times
};

}


#endif //EXECUTIONTIMER_H
