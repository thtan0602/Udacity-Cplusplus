#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "process.h"
#include "linux_parser.h"

using std::string;
using std::to_string;
using std::vector;

// TODO: Return this process's ID
int Process::Pid() { return pid_; }

// TODO: Return this process's CPU utilization
#include <fstream>
#include <sstream>
#include <string>
#include <iostream>
#include <unistd.h>  // For sysconf to get _SC_CLK_TCK

// Function to calculate CPU usage of a process
float Process::CpuUtilization() const {
    // Get system uptime (in seconds)
    long uptime = LinuxParser::UpTime();

    // Get process times (these values are from the /proc/[pid]/stat file)
    long total_time = utime_ + stime_;  // User time + System time

    // Get the process start time (in jiffies)
    long starttime = start_time_;

    // Calculate the total elapsed time in seconds
    long seconds = uptime - (starttime / sysconf(_SC_CLK_TCK));

    // Calculate CPU usage percentage
    if (seconds > 0) {
        float cpu_usage = 100.0 * ((total_time / sysconf(_SC_CLK_TCK)) / seconds);
        return cpu_usage;
    } else {
        return 0.0;  // If seconds is zero or less, return 0%
    }
}


// TODO: Return the command that generated this process
string Process::Command() {
    string command;
    std::ifstream file("/proc/" + to_string(pid_) + "/cmdline");
    if (file.is_open()) {
        std::getline(file, command);
    }
    return command;
}

// TODO: Return this process's memory utilization
string Process::Ram() { return LinuxParser::Ram(pid_); }

// TODO: Return the user (name) that generated this process
string Process::User() { return LinuxParser::User(pid_); }

// TODO: Return the age of this process (in seconds)
long int Process::UpTime() { return LinuxParser::UpTime(pid_); }

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const& a) const {
    return this->CpuUtilization() < a.CpuUtilization();  // Comparing based on CPU usage
}
