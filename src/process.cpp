#include "process.h"

#include <unistd.h>

#include <cctype>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "linux_parser.h"

using std::string;
using std::to_string;
using std::vector;

// DONE: Return this process's ID
int Process::Pid() { return pid_; }

// DONE: Return this process's CPU utilization
// NOTE: Inspired from https://stackoverflow.com/a/16736599
float Process::CpuUtilization() const {
  string value, line, utime, stime, cutime, cstime;
  std::ifstream filestream(LinuxParser::kProcDirectory + std::to_string(pid_) +
                           LinuxParser::kStatFilename);
  if (filestream.is_open()) {
    getline(filestream, line);
    std::istringstream linestream(line);
    for (size_t i = 0; i <= 21; i++) {
      linestream >> value;
      if (i == 13) {
        utime = value;
      } else if (i == 14) {
        stime = value;
      } else if (i == 15) {
        cutime = value;
      } else if (i == 16) {
        cstime = value;
      } else if (i > 16) {
        break;
      } else {
        continue;
      }
    }

    float total_time = std::stof(utime) + std::stof(stime) + std::stof(cutime) +
                       std::stof(cstime);
    long uptime = this->UpTime();
    return (total_time / sysconf(_SC_CLK_TCK)) / uptime;
  }
}

// DONE: Return the command that generated this process
string Process::Command() { return LinuxParser::Command(pid_); }

// DONE: Return this process's memory utilization
string Process::Ram() { return LinuxParser::Ram(pid_); }

// DONE: Return the user (name) that generated this process
string Process::User() { return LinuxParser::User(pid_); }

// DONE: Return the age of this process (in seconds)
long int Process::UpTime() const { return LinuxParser::UpTime(pid_); }

// DONE: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const& a) const {
  return a.CpuUtilization() < this->CpuUtilization();
}