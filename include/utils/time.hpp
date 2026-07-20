#include <ctime>
struct TimeRange {
  time_t start;
  time_t end;
};

TimeRange getCurrentWeek();
TimeRange getCurrentDay();
