#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>

#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, version, kernel;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
}

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

// TODO: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() {
    string line, key;
    float total_memory = 0.0, free_memory = 0.0, value;
    std::ifstream filestream(kProcDirectory + kMeminfoFilename);
    if (filestream.is_open()) {
        while (std::getline(filestream, line)) {
            std::istringstream linestream(line);
            linestream >> key >> value;
            if (key == "MemTotal:") total_memory = value;
            if (key == "MemFree:") free_memory = value;
        }
    }
    return (total_memory - free_memory) / total_memory;
}

// TODO: Read and return the system uptime
long LinuxParser::UpTime() {
    string line;
    long uptime;
    std::ifstream filestream(kProcDirectory + kUptimeFilename);
    if (filestream.is_open()) {
        std::getline(filestream, line);
        std::istringstream linestream(line);
        linestream >> uptime;
    }
    return uptime;
}

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() {
    string line;
    long jiffies;
    std::ifstream filestream(kProcDirectory + kStatFilename);
    if (filestream.is_open()) {
        std::getline(filestream, line);
        std::istringstream linestream(line);
        string cpu;
        linestream >> cpu >> jiffies;
    }
    return jiffies;
}

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid) {
    string line;
    long active_jiffies = 0;
    std::ifstream filestream(kProcDirectory + to_string(pid) + kStatFilename);
    if (filestream.is_open()) {
        std::getline(filestream, line);
        std::istringstream linestream(line);
        string token;
        for (int i = 0; i < 13; ++i) linestream >> token; // Skip to CPU time fields
        long utime, stime, cutime, cstime;
        linestream >> utime >> stime >> cutime >> cstime;
        active_jiffies = utime + stime + cutime + cstime;
    }
    return active_jiffies;
}

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() {
    string line;
    long active_jiffies = 0;
    std::ifstream filestream(kProcDirectory + kStatFilename);
    if (filestream.is_open()) {
        while (std::getline(filestream, line)) {
            std::istringstream linestream(line);
            string cpu;
            linestream >> cpu; // Read the "cpu" label

            // Check if the line corresponds to the "cpu" stats (not "cpu0", "cpu1", etc.)
            if (cpu.substr(0, 3) == "cpu") {
                // Declare variables for each CPU state based on the enum
                vector<long> values(kGuestNice_ + 1, 0);  // There are 10 values per CPU line in /proc/stat
                for (int i = 0; i < kGuestNice_ ; ++i) {
                    linestream >> values[i];
                }

                // Sum the active jiffies based on the CPU states:
                // User, Nice, System, IRQ, SoftIRQ, Steal are the active states
               active_jiffies += values[kUser_] + values[kNice_] + values[kSystem_] + values[kIRQ_] + values[kSoftIRQ_] + values[kSteal_];  
            }
        }
    }
    return active_jiffies;
}

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() {
    string line;
    long idle_jiffies = 0;
    std::ifstream filestream(kProcDirectory + kStatFilename);
    if (filestream.is_open()) {
        std::getline(filestream, line); // Read the first line
        std::istringstream linestream(line);
        string cpu;
        linestream >> cpu;
        
      	// Create a vector to store CPU state values
      	vector<long> values(kGuestNice_ + 1, 0); // Allocate based on the enum size
        for (int i = 0; i <= kGuestNice_; ++i) {
                    linestream >> values[i];
                }        
        idle_jiffies = values[kIdle_] + values[kIOwait_];
    }
    return idle_jiffies;
}

vector<string> LinuxParser::CpuUtilization() {
    vector<string> cpu_utilization;
    string line;
    std::ifstream filestream(kProcDirectory + kStatFilename);
    double total_utilization = 0.0;
    int core_count = 0;

    if (filestream.is_open()) {
        string line;
        while (std::getline(filestream, line)) {
            if (line.substr(0, 3) == "cpu") {  // Skip the "cpu" line and process "cpu0", "cpu1", etc.
                std::istringstream linestream(line);
                string cpu;
                linestream >> cpu;  // Skip the "cpu" label

                // Allocate a vector for CPU states and initialize with 0
                vector<long> values(kGuestNice_ + 1, 0);
                for (int i = 0; i <= kGuestNice_; ++i) {
                    linestream >> values[i];
                }

                // Calculate total and active jiffies
                long total = 0;
                for (const auto& value : values) {
                    total += value;  // Sum all CPU states for the total jiffies
                }

                long active = values[kUser_] + values[kNice_] + values[kSystem_] +
                              values[kIRQ_] + values[kSoftIRQ_] + values[kSteal_];

                // Calculate utilization as a ratio of active to total jiffies
                double utilization = total > 0 ? static_cast<double>(active) / total : 0.0;

                total_utilization += utilization;
                core_count++;
            }
        }
    }

    if (core_count > 0) {
        double avg_utilization = total_utilization / core_count;
        cpu_utilization.push_back(to_string(avg_utilization));
    }

    return cpu_utilization;
}

// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() {
    string line;
    int total_processes = 0;
    std::ifstream filestream(kProcDirectory + kStatFilename);
    if (filestream.is_open()) {
        while (std::getline(filestream, line)) {
            std::istringstream linestream(line);
            string token;
            linestream >> token;
            if (token == "processes") {
                linestream >> total_processes;
                break;
            }
        }
    }
    return total_processes;
}

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() {
    string line;
    int running_processes = 0;
    std::ifstream filestream(kProcDirectory + kStatFilename);
    if (filestream.is_open()) {
        while (std::getline(filestream, line)) {
            std::istringstream linestream(line);
            string token;
            linestream >> token;
            if (token == "procs_running") {
                linestream >> running_processes;
                break;
            }
        }
    }
    return running_processes;
}

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid) {
    string command;
    std::ifstream filestream(kProcDirectory + to_string(pid) + kCmdlineFilename);
    if (filestream.is_open()) {
        std::getline(filestream, command);
    }
    return command;
}

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid) {
    string line;
    string label, ram;
    std::ifstream filestream(kProcDirectory + to_string(pid) + kStatusFilename);
    if (filestream.is_open()) {
        while (std::getline(filestream, line)) {
            if (line.substr(0, 6) == "VmRSS:") {
                std::istringstream linestream(line);
                linestream >> label >> ram;
                break;
            }
        }
    }
    return ram;
}

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid) {
    string line;
    string label, uid;
    std::ifstream filestream(kProcDirectory + to_string(pid) + kStatusFilename);
    
    if (filestream.is_open()) {
        while (std::getline(filestream, line)) {
            if (line.rfind("Uid:", 0) == 0) {  // checks if the line starts with "Uid:"
                std::istringstream linestream(line);
                linestream >> label >> uid;
                return uid;  // return UID immediately once found
            }
        }
    }
    return "";  // return empty string if UID is not found or file can't be read
}

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid) {
    string uid = Uid(pid);  // Get the UID of the process
    string line, user, x, userUid;

    std::ifstream filestream(kPasswordPath);  // Open the /etc/passwd file to read user info
    if (filestream.is_open()) {  // Check if the file was opened successfully
        while (std::getline(filestream, line)) {  // Read the /etc/passwd file line by line
            // Replace all colons with spaces to make it easier to split the line into tokens
            std::replace(line.begin(), line.end(), ':', ' ');

            std::istringstream linestream(line);  // Create a stream from the line
            linestream >> user >> x >> userUid;  // Extract the user, password placeholder (x), and UID

            // Check if the UID from the /etc/passwd file matches the UID of the process
            if (userUid == uid) {
                return user;  // Return the username associated with the matching UID
            }
        }
    }
    return "";  // Return an empty string if no match is found
}

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid) {
    string line;
    long uptime = 0;
    std::ifstream filestream(kProcDirectory + to_string(pid) + kStatFilename);
    if (filestream.is_open()) {
        std::getline(filestream, line);
        std::istringstream linestream(line);
        string token;
        for (int i = 0; i < 21; ++i) linestream >> token; // Skip to uptime field
        linestream >> uptime;
    }
    return uptime;
}
