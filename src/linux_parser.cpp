#include "linux_parser.h"

#include <dirent.h>
#include <unistd.h>

#include <string>
#include <vector>

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
  string os, kernel;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> kernel;
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

// DONE: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() {
  string line;
  string name;
  float memtotal;
  float memfree;
  float buffers;
  std::ifstream filestream(kProcDirectory + kMeminfoFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      linestream >> name;
      if (name == "MemTotal:")
        linestream >> memtotal;
      else if (name == "MemFree:")
        linestream >> memfree;
      else if (name == "Buffers:")
        linestream >> buffers;
      else
        break;
    }
  }
  return 1.0 - memfree / (memtotal - buffers);
}

// DONE: Read and return the system uptime
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

// DONE: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() {
  return LinuxParser::ActiveJiffies() + LinuxParser::IdleJiffies();
}

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid[[maybe_unused]]) { return 0; }

// DONE: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() {
  vector<string> values = LinuxParser::CpuUtilization();
  // long kuser = values[kUser_] != "" ? stol(values[kUser_]) : 0;
  return stol(values[kUser_]) + stol(values[kNice_]) + stol(values[kSystem_]) +
         stol(values[kIdle_]) + stol(values[kSoftIRQ_]) +
         stol(values[kSteal_]) + stol(values[kGuest_]) +
         stol(values[kGuestNice_]);
  ;
}

// DONE: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() {
  vector<string> values = LinuxParser::CpuUtilization();
  return stol(values[kIdle_]) + stol(values[kIOwait_]);
}

// DONE: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() {
  string line;
  string value;
  vector<string> values;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);
    linestream >> value;
    for (size_t i = 0; i < 10; i++) {
      linestream >> value;
      values.push_back(value);
    }
  }
  return values;
}

// DONE: Read and return the total number of processes
int LinuxParser::TotalProcesses() {
  string line;
  string name;
  int processes;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (getline(filestream, line)) {
      std::istringstream linestream(line);
      linestream >> name;
      if (name == "processes") {
        linestream >> processes;
        break;
      }
    }
  }
  return processes;
}

// DONE: Read and return the number of running processes
int LinuxParser::RunningProcesses() {
  string line;
  string name;
  int procs_running;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (getline(filestream, line)) {
      std::istringstream linestream(line);
      linestream >> name;
      if (name == "procs_running") {
        linestream >> procs_running;
        break;
      }
    }
  }
  return procs_running;
}

// DONE: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid) {
  string command;
  std::ifstream filestream(kProcDirectory + to_string(pid) + kCmdlineFilename);
  getline(filestream, command);
  return command;
}

// DONE: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid) {
  string line;
  string name;
  string ram;
  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatusFilename);
  if (filestream.is_open()) {
    while (getline(filestream, line)) {
      std::istringstream linestream(line);
      linestream >> name;
      if (name == "VmSize:") {
        linestream >> ram;
        break;
      }
    }
  }
  return ram;
}

// DONE: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid) {
  string line;
  string name;
  string uid;
  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatusFilename);
  if (filestream.is_open()) {
    while (getline(filestream, line)) {
      std::istringstream linestream(line);
      linestream >> name;
      if (name == "Uid:") {
        linestream >> uid;
        break;
      }
    }
  }
  return uid;
}

// DONE: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid) {
  string line;
  string name;
  string user;
  string x;
  string temp_uid;
  string uid = LinuxParser::Uid(pid);
  std::ifstream filestream(kPasswordPath);
  if (filestream.is_open()) {
    while (getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      linestream >> user >> x >> temp_uid;
      if (temp_uid == uid) break;
    }
  }
  return user;
}

// DONE: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid) {
  string line;
  string value;
  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatFilename);
  if (filestream.is_open()) {
    getline(filestream, line);
    std::istringstream linestream(line);
    for (size_t i = 0; i <= 21; i++) {
      linestream >> value;
      if (i == 21) {
        return UpTime() - std::stol(value) / sysconf(_SC_CLK_TCK);
      }
    }
  }
}