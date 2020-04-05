#ifndef PENDULUM_PARSER_H_
#define PENDULUM_PARSER_H_

#include <chrono>
#include <cstdio>
#include <string>

#include <cctz/time_zone.h>

#include "pendulum/datetime.h"
#include "pendulum/optional.h"

namespace pendulum {

inline Optional<DateTime> from_format(const std::string& input, const std::string& fmt,
                                      const std::string& tz = "UTC") {
    const auto& timezone = internal::timezone(tz);
    cctz::time_point<std::chrono::seconds> tp;

    auto ok = cctz::parse(fmt, input, timezone, &tp);

    if (!ok) {
        return nullopt;
    }

    const auto& cs = cctz::convert(tp, timezone);

    return DateTime(cs, timezone);
}

inline Optional<DateTime> parse(const std::string& input, const std::string& tz = "UTC") {
    auto dt = from_format(input, "%Y-%m-%d", tz);
    if (dt) {
        return dt;
    }

    dt = from_format(input, "%Y/%m/%d", tz);
    if (dt) {
        return dt;
    }

    // cctz cannot parse "%Y%m%d"
    int year, month, day;
    auto counts = std::sscanf(input.c_str(), "%04d%02d%02d", &year, &month, &day);
    if (counts == 3) {
        return DateTime(year, month, day, tz);
    }

    return nullopt;
}

}  // namespace pendulum

#endif  // PENDULUM_PARSER_H_
