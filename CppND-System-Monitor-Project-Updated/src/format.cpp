#include <string>

#include "format.h"

using std::string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds) {
    long hours = seconds / 3600;
    long minutes = (seconds % 3600) / 60;
    long secs = seconds % 60;
    return (std::to_string(hours) + ":" + 
            (minutes < 10 ? "0" : "") + std::to_string(minutes) + ":" + 
            (secs < 10 ? "0" : "") + std::to_string(secs));
}
