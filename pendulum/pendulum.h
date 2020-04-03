#ifndef PENDULUM_H_
#define PENDULUM_H_

#include <chrono>
#include <string>

#include "pendulum/constants.h"
#include "pendulum/date.h"
#include "pendulum/datetime.h"

namespace pendulum {

inline Date date(int y = 1970, int m = 1, int d = 1) { return Date(y, m, d); }

inline DateTime datetime(int y = 1970, int m = 1, int d = 1, const std::string& tz = "UTC") {
    return DateTime(y, m, d, tz);
}

inline DateTime datetime(int y, int m, int d, int hh, int mm = 0, int ss = 0,
                         const std::string& tz = "UTC") {
    return DateTime(y, m, d, hh, mm, ss, tz);
}

inline DateTime now(const std::string& tz = "local") {
    const auto& tz_ = internal::timezone(tz);
    const auto& cs = cctz::convert(std::chrono::system_clock::now(), tz_);

    DateTime dt;
    dt.instance(cs, tz_);

    return dt;
}

inline DateTime local(int y = 1970, int m = 1, int d = 1) { return datetime(y, m, d, "local"); }

inline DateTime local(int y, int m, int d, int hh, int mm = 0, int ss = 0) {
    return datetime(y, m, d, hh, mm, ss, "local");
}

inline DateTime today(const std::string& tz = "local") {
    const auto& dt = now(tz);
    return DateTime(dt.year(), dt.month(), dt.day(), tz);
}

inline DateTime yesterday(const std::string& tz = "local") {
    const auto& dt = now(tz);
    return DateTime(dt.year(), dt.month(), dt.day() - 1, tz);
}

inline DateTime tomorrow(const std::string& tz = "local") {
    const auto& dt = now(tz);
    return DateTime(dt.year(), dt.month(), dt.day() + 1, tz);
}

inline DateTime from_format(const std::string& input, const std::string& fmt,
                            const std::string& tz = "UTC") {
    const auto& timezone = internal::timezone(tz);

    cctz::time_point<std::chrono::seconds> tp;
    cctz::parse(fmt, input, timezone, &tp);

    const auto& cs = cctz::convert(tp, timezone);

    DateTime dt;
    dt.instance(cs, timezone);

    return dt;
}

}  // namespace pendulum

#endif  // PENDULUM_H_
