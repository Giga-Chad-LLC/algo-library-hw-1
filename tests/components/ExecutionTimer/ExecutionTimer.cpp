//
// Created by Vladislav.Artiukhov on 08.02.25.
//

#include "ExecutionTimer.h"

namespace trees::testing {

void ExecutionTimer::start(const std::string& name) {
    if (sessions.contains(name)) {
        throw std::runtime_error("Session \"" + name + "\" has already been started!");
    }
    sessions[name] = Clock::now(); // Record the start time
}


double ExecutionTimer::finish(const std::string& name) {
    const auto session = sessions.find(name);
    if (session == sessions.end()) {
        throw std::runtime_error("No active session found for name \"" + name + "\".");
    }
    const auto endTime = Clock::now(); // Get end time
    const auto startTime = session->second;

    // Calculate the duration in milliseconds
    const std::chrono::duration<double, std::milli> duration = endTime - startTime;

    // Erase the session from the map
    sessions.erase(session);

    return duration.count(); // Return the elapsed time
}

}