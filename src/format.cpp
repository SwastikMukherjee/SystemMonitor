#include "format.h"

#include <iomanip>
#include <sstream>
#include <string>

using std::string;

// DONE: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
#define SECONDS_IN_MINUTES 60
#define SECONDS_IN_HOURS 3600

string Format::ElapsedTime(long seconds) {
  long hours = seconds / SECONDS_IN_HOURS;
  seconds -= hours * SECONDS_IN_HOURS;
  long minutes = seconds / SECONDS_IN_MINUTES;
  seconds -= minutes * SECONDS_IN_MINUTES;
  std::ostringstream elapsedtime;
  // NOTE: setfill takes a char, it should be '0' instead of "0"
  elapsedtime << std::setfill('0') << std::setw(2) << hours << ":"
              << std::setfill('0') << std::setw(2) << minutes << ":"
              << std::setfill('0') << std::setw(2) << seconds;
  return elapsedtime.str();
}