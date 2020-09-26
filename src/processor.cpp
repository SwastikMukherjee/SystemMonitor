#include "processor.h"

#include "linux_parser.h"

// DONE: Return the aggregate CPU utilization
float Processor::Utilization() {
  float jiffies = LinuxParser::Jiffies();
  float active_jiffies = LinuxParser::ActiveJiffies();
  return active_jiffies / jiffies;
}