#include <utils/time.hpp>

WeekRange getCurrentWeek() {
  time_t now = time(nullptr);

  std::tm local = *std::localtime(&now);

  int daysSinceMonday = (local.tm_wday + 6) % 7;

  local.tm_mday -= daysSinceMonday;
  local.tm_hour = 0;
  local.tm_min = 0;
  local.tm_sec = 0;

  WeekRange range;
  range.start = std::mktime(&local);

  local.tm_mday += 6;
  local.tm_hour = 23;
  local.tm_min = 59;
  local.tm_sec = 59;

  range.end = std::mktime(&local);

  return range;
}

DayRange getCurrentDay() {
  time_t now = time(nullptr);

  std::tm local = *std::localtime(&now);

  // Beginning of today
  local.tm_hour = 0;
  local.tm_min = 0;
  local.tm_sec = 0;

  DayRange range;
  range.start = std::mktime(&local);

  // End of today
  local.tm_hour = 23;
  local.tm_min = 59;
  local.tm_sec = 59;

  range.end = std::mktime(&local);

  return range;
}
