#ifndef PENDULUM_CONSTANTS_H_
#define PENDULUM_CONSTANTS_H_

#include <cctz/civil_time.h>

namespace pendulum {

namespace internal {

static constexpr int kSecondsPerMinute = 60;
static constexpr int kMinutesPerHour = 60;

}  // namespace internal

static constexpr int kMonday = static_cast<int>(cctz::weekday::monday);
static constexpr int kTuesday = static_cast<int>(cctz::weekday::tuesday);
static constexpr int kWednesday = static_cast<int>(cctz::weekday::wednesday);
static constexpr int kThursday = static_cast<int>(cctz::weekday::thursday);
static constexpr int kFriday = static_cast<int>(cctz::weekday::friday);
static constexpr int kSaturday = static_cast<int>(cctz::weekday::saturday);
static constexpr int kSunday = static_cast<int>(cctz::weekday::sunday);

}  // namespace pendulum

#endif  // PENDULUM_CONSTANTS_H_
