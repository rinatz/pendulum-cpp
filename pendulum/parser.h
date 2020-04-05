#ifndef PENDULUM_PARSER_H_
#define PENDULUM_PARSER_H_

#include <chrono>
#include <cstdio>
#include <ctime>
#include <string>

#include <cctz/time_zone.h>

#include "pendulum/datetime.h"
#include "pendulum/helpers.h"
#include "pendulum/optional.h"

namespace pendulum {

inline Optional<DateTime> from_format(const std::string& input, const std::string& fmt,
                                      const std::string& tz = "UTC") {
    if (fmt == "%Y%m%d") {
        // cctz cannot parse '%Y%m%d'
        int year = 0, month = 0, day = 0;
        auto count = std::sscanf(input.c_str(), "%04d%02d%02d", &year, &month, &day);

        if (count != 3 || count == EOF) {
            return nullopt;
        }

        return DateTime(year, month, day, tz);
    }

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
    static const std::string formats[] = {
            "%Y-%m-%dT%H:%M:%S%Ez",
            "%Y-%m-%dT%H:%M:%S%E",
            "%Y-%m-%d %H:%M:%S",
            "%Y-%m-%d",
            "%Y-%m",
            "%Y%m%d",
            "%Y",
    };

    for (const auto& fmt : formats) {
        const auto& dt = from_format(input, fmt, tz);
        if (dt) {
            return dt;
        }
    }

    return nullopt;
}

}  // namespace pendulum

#endif  // PENDULUM_PARSER_H_
