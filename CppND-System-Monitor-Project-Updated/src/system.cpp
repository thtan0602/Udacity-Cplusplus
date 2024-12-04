#include <unistd.h>
#include <cstddef>
#include <set>
#include <string>
#include <vector>

#include "linux_parser.h"
#include "process.h"
#include "processor.h"
#include "system.h"

using std::set;
using std::size_t;
using std::string;
using std::vector;

// TODO: Return the system's CPU
Processor& System::Cpu() { return cpu_; }

// TODO: Return a container composed of the system's processes
vector<Process>& System::Processes() { 
    processes_.clear();  // Clear any previous data

    // Retrieve the PIDs of active processes
    vector<int> pids = LinuxParser::Pids();

    // Reserve space for processes to avoid multiple reallocations
    processes_.reserve(pids.size());

    // Create Process objects and emplace them into the vector
    for (int pid : pids) {
        processes_.emplace_back(pid);
    }

    // Sort the processes based on CPU utilization 
    std::sort(processes_.begin(), processes_.end());

    return processes_;
}

// TODO: Return the system's kernel identifier (string)
string System::Kernel() {return LinuxParser::Kernel(); }

// TODO: Return the system's memory utilization
float System::MemoryUtilization() { return LinuxParser::MemoryUtilization(); }

// TODO: Return the operating system name
string System::OperatingSystem() {return LinuxParser::OperatingSystem(); }

// TODO: Return the number of processes actively running on the system
int System::RunningProcesses() { return LinuxParser::RunningProcesses(); }

// TODO: Return the total number of processes on the system
int System::TotalProcesses() { return LinuxParser::TotalProcesses(); }

// TODO: Return the number of seconds since the system started running
long int System::UpTime() { return LinuxParser::UpTime(); }
