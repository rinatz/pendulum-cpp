#ifndef PENDULUM_HELPERS_H_
#define PENDULUM_HELPERS_H_

#include <chrono>
#include <ctime>
#include <string>

#include "pendulum/constants.h"
#include "pendulum/date.h"
#include "pendulum/datetime.h"
#include "pendulum/testing.h"

namespace pendulum {

inline Date date(int y = 1970, int m = 1, int d = 1) { return Date(y, m, d); }

inline DateTime datetime(int y = 1970, int m = 1, int d = 1, const std::string& tz = "UTC") {
    return DateTime(y, m, d, tz);
}

inline DateTime datetime(int y, int m, int d, int hh, int mm = 0, int ss = 0,
                         const std::string& tz = "UTC") {
    return DateTime(y, m, d, hh, mm, ss, tz);
}

inline DateTime local(int y = 1970, int m = 1, int d = 1) { return datetime(y, m, d, "local"); }

inline DateTime local(int y, int m, int d, int hh, int mm = 0, int ss = 0) {
    return datetime(y, m, d, hh, mm, ss, "local");
}

inline DateTime now(const std::string& tz = "local") {
    if (has_test_now()) {
        return get_test_now();
    }

    const auto& tz_ = internal::timezone(tz);
    const auto& cs = cctz::convert(std::chrono::system_clock::now(), tz_);

    return DateTime(cs, tz_);
}

inline DateTime today(const std::string& tz = "local") {
    return now(tz).start_of("day");
}

inline DateTime yesterday(const std::string& tz = "local") {
    return today(tz).add_days(-1);
}

inline DateTime tomorrow(const std::string& tz = "local") {
    return today(tz).add_days(1);
}

inline DateTime from_timestamp(time_t timestamp, const std::string& tz = "UTC") {
    const auto& timezone = internal::timezone(tz);
    const auto& tp = std::chrono::system_clock::from_time_t(timestamp);
    const auto& cs = cctz::convert(tp, timezone);

    return DateTime(cs, timezone);
}

}  // namespace pendulum

#endif  // PENDULUM_HELPERS_H_
