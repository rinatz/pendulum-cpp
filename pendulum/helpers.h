// MIT License

// Copyright (c) 2020 Ida Kenichiro

// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#ifndef PENDULUM_HELPERS_H_
#define PENDULUM_HELPERS_H_

#include <chrono>
#include <ctime>
#include <string>

#include "pendulum/constants.h"
#include "pendulum/date.h"
#include "pendulum/datetime.h"
#include "pendulum/period.h"
#include "pendulum/testing.h"

namespace pendulum {

inline Date date(int y, int m, int d) { return Date(y, m, d); }

inline DateTime datetime(int y, int m, int d, const std::string& tz = "UTC") {
    return DateTime(y, m, d, tz);
}

inline DateTime datetime(int y, int m, int d, int hh, int mm = 0, int ss = 0,
                         const std::string& tz = "UTC") {
    return DateTime(y, m, d, hh, mm, ss, tz);
}

inline DateTime local(int y, int m, int d) { return datetime(y, m, d, "local"); }

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

inline DateTime today(const std::string& tz = "local") { return now(tz).start_of("day"); }
inline DateTime yesterday(const std::string& tz = "local") { return today(tz).add_days(-1); }
inline DateTime tomorrow(const std::string& tz = "local") { return today(tz).add_days(1); }

inline DateTime from_timestamp(time_t timestamp, const std::string& tz = "UTC") {
    const auto& timezone = internal::timezone(tz);
    const auto& tp = std::chrono::system_clock::from_time_t(timestamp);
    const auto& cs = cctz::convert(tp, timezone);

    return DateTime(cs, timezone);
}

inline Period period(const DateTime& start, const DateTime& stop, bool absolute = false) {
    return Period(start, stop, absolute);
}

inline bool week_starts_at(int week_day) {
    if (kMonday <= week_day && week_day <= kSunday) {
        internal::week_starts_at() = week_day;
        return true;
    }

    return false;
}

}  // namespace pendulum

#endif  // PENDULUM_HELPERS_H_
