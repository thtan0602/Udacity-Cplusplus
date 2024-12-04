#include "processor.h"
#include <fstream>
#include <sstream>
#include <string>
#include <iostream>

float Processor::Utilization() {
    std::string line;
    std::ifstream filestream("/proc/stat");

    // Check if the file is open
    if (filestream.is_open()) {
        std::getline(filestream, line);  // Read the first line (total CPU usage)
        std::istringstream linestream(line);

        std::string cpu;
        long user, nice, system, idle, iowait, irq, softirq, steal, guest, guest_nice;
        linestream >> cpu >> user >> nice >> system >> idle >> iowait >> irq >> softirq >> steal >> guest >> guest_nice;

        // Calculate total time and idle time
        long total_time = user + nice + system + idle + iowait + irq + softirq + steal + guest + guest_nice;
        long idle_time = idle + iowait;

        // Assuming previous values (previous_total and previous_idle) are stored in private variables
        long total_diff = total_time - previous_total;
        long idle_diff = idle_time - previous_idle;

        // Update the previous values for the next calculation
        previous_total = total_time;
        previous_idle = idle_time;

        // Return CPU utilization as the fraction of used CPU time
        return (total_diff - idle_diff) / static_cast<float>(total_diff);
    }
    return 0.0;
}
