#include <ctime>
struct WeekRange {
  time_t start;
  time_t end;
};

struct DayRange {
  time_t start;
  time_t end;
};

WeekRange getCurrentWeek();
DayRange getCurrentDay();
