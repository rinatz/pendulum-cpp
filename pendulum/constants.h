#ifndef PENDULUM_CONSTANTS_H_
#define PENDULUM_CONSTANTS_H_

#include <cctz/civil_time.h>

namespace pendulum {

namespace internal {

constexpr int kSecondsPerMinute = 60;
constexpr int kMinutesPerHour = 60;

}  // namespace internal

constexpr int kMonday = static_cast<int>(cctz::weekday::monday);
constexpr int kTuesday = static_cast<int>(cctz::weekday::tuesday);
constexpr int kWednesday = static_cast<int>(cctz::weekday::wednesday);
constexpr int kThursday = static_cast<int>(cctz::weekday::thursday);
constexpr int kFriday = static_cast<int>(cctz::weekday::friday);
constexpr int kSaturday = static_cast<int>(cctz::weekday::saturday);
constexpr int kSunday = static_cast<int>(cctz::weekday::sunday);

}  // namespace pendulum

#endif  // PENDULUM_CONSTANTS_H_
