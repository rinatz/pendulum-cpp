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

#ifndef PENDULUM_DATETIME_H_
#define PENDULUM_DATETIME_H_

#include <chrono>
#include <ctime>
#include <functional>
#include <string>

#include <cctz/civil_time.h>
#include <cctz/time_zone.h>

#include "pendulum/constants.h"
#include "pendulum/date.h"
#include "pendulum/timezone.h"

namespace pendulum {

class DateTime {
   public:
    //
    // Instantiation
    //

    static const DateTime& epoch() {
        static DateTime dt(1970, 1, 1, "UTC");
        return dt;
    }

    DateTime() { *this = epoch(); }

    DateTime(const Date& date, const std::string& tz = "UTC")
            : DateTime(date.year(), date.month(), date.day(), tz) {}

    DateTime(int y, int m, int d, const std::string& tz = "UTC") : DateTime(y, m, d, 0, 0, 0, tz) {}

    DateTime(int y, int m, int d, int hh, int mm = 0, int ss = 0, const std::string& tz = "UTC")
            : DateTime(cctz::civil_second(y, m, d, hh, mm, ss), internal::timezone(tz)) {}

    DateTime(const cctz::civil_second& cs, const cctz::time_zone& tz) : cs_(cs), tz_(tz) {}

    DateTime(const DateTime&) = default;
    DateTime(DateTime&&) = default;
    virtual ~DateTime() = default;

    DateTime& operator=(const DateTime&) = default;
    DateTime& operator=(DateTime&&) = default;

    //
    // Attributes
    //

    int year() const { return cs_.year(); }
    int month() const { return cs_.month(); }
    int day() const { return cs_.day(); }
    int hour() const { return cs_.hour(); }
    int minute() const { return cs_.minute(); }
    int second() const { return cs_.second(); }

    Date date() const { return Date(year(), month(), day()); }
    int day_of_week() const { return date().day_of_week(); }
    int day_of_year() const { return date().day_of_year(); }
    int week_of_month() const { return date().week_of_month(); }

    bool is_leap_year() const { return date().is_leap_year(); }
    bool is_dst() const { return tz_.lookup(cctz::convert(cs_, tz_)).is_dst; }
    bool is_local() const { return tz_ == cctz::local_time_zone(); }
    bool is_utc() const { return tz_ == cctz::utc_time_zone(); }

    std::string timezone_name() const { return tz_.name(); }

    int offset() const { return tz_.lookup(cctz::convert(cs_, tz_)).offset; }

    double offset_hours() const {
        using namespace internal;
        return offset() / kSecondsPerMinute / kMinutesPerHour;
    }

    time_t timestamp() const {
        const auto& tp = cctz::convert(cs_, tz_);
        return std::chrono::system_clock::to_time_t(tp);
    }

    //
    // Fluent helpers
    //

    DateTime year(int y) const { return on(y, month(), day()); }
    DateTime month(int m) const { return on(year(), m, day()); }
    DateTime day(int d) const { return on(year(), month(), d); }
    DateTime hour(int hh) const { return at(hh, minute(), second()); }
    DateTime minute(int mm) const { return at(hour(), mm, second()); }
    DateTime second(int ss) const { return at(hour(), minute(), ss); }

    DateTime on(int y, int m, int d) const {
        const auto& cs = cctz::civil_second(y, m, d, hour(), minute(), second());
        return DateTime(cs, tz_);
    }

    DateTime at(int hh, int mm, int ss) const {
        const auto& cs = cctz::civil_second(year(), month(), day(), hh, mm, ss);
        return DateTime(cs, tz_);
    }

    DateTime timezone(const std::string& name) const {
        const auto& tz = internal::timezone(name);
        return DateTime(cs_, tz);
    }

    DateTime offset(int seconds) const {
        return DateTime(cs_, internal::timezone(std::chrono::seconds(seconds)));
    }

    DateTime offset_hours(int hours, int minutes = 0) const {
        const auto s = std::chrono::hours(hours) + std::chrono::minutes(minutes);
        return DateTime(cs_, internal::timezone(s));
    }

    //
    // String formatting
    //

    std::string to_date_string() const { return date().to_date_string(); }
    std::string to_time_string() const { return format("%H:%M:%S"); }
    std::string to_datetime_string() const { return format("%Y-%m-%d %H:%M:%S"); }
    std::string to_iso8601_string() const { return format("%Y-%m-%dT%H:%M:%S%Ez"); }

    std::string format(const std::string& fmt) const {
        const auto tp = cctz::convert(cs_, tz_);
        return cctz::format(fmt, tp, tz_);
    }

    //
    // Additions
    //

    DateTime add_years(int y) const { return add_date(y, 0, 0); }
    DateTime add_months(int m) const { return add_date(0, m, 0); }
    DateTime add_days(int d) const { return add_date(0, 0, d); }
    DateTime add_hours(int hh) const { return add_time(hh, 0, 0); }
    DateTime add_minutes(int mm) const { return add_time(0, mm, 0); }
    DateTime add_seconds(int ss) const { return add_time(0, 0, ss); }

    DateTime add_date(int y, int m, int d) const {
        const auto& cs =
                cctz::civil_second(year() + y, month() + m, day() + d, hour(), minute(), second());

        return DateTime(cs, tz_);
    }

    DateTime add_time(int hh, int mm, int ss) const {
        const auto& cs = cctz::civil_second(year(), month(), day(), hour() + hh, minute() + mm,
                                            second() + ss);

        return DateTime(cs, tz_);
    }

    //
    // Modifiers
    //

    DateTime next(bool keep_time = false) const { return next(day_of_week(), keep_time); }

    DateTime next(int day_of_week, bool keep_time = false) const {
        const auto& cs = DateTime(date().next(day_of_week)).instance();
        auto dt = DateTime(cs, tz_);

        if (keep_time) {
            dt = dt.at(hour(), minute(), second());
        }

        return dt;
    }

    DateTime previous(bool keep_time = false) const { return previous(day_of_week(), keep_time); }

    DateTime previous(int day_of_week, bool keep_time = false) const {
        const auto& cs = DateTime(date().previous(day_of_week)).instance();
        auto dt = DateTime(cs, tz_);

        if (keep_time) {
            dt = dt.at(hour(), minute(), second());
        }

        return dt;
    }

    DateTime start_of(const std::string& unit) const {
        if (unit == "year") {
            return start_of_year();
        }
        if (unit == "month") {
            return start_of_month();
        }
        if (unit == "day") {
            return start_of_day();
        }
        if (unit == "hour") {
            return start_of_hour();
        }
        if (unit == "minute") {
            return start_of_minute();
        }
        if (unit == "week") {
            return start_of_week();
        }
        throw PendulumException("Invalid unit for start_of(): " + unit);
    }

    DateTime end_of(const std::string& unit) const {
        if (unit == "year") {
            return end_of_year();
        }
        if (unit == "month") {
            return end_of_month();
        }
        if (unit == "day") {
            return end_of_day();
        }
        if (unit == "hour") {
            return end_of_hour();
        }
        if (unit == "minute") {
            return end_of_minute();
        }
        if (unit == "week") {
            return end_of_week();
        }
        throw PendulumException("Invalid unit for end_of(): " + unit);
    }

    DateTime in_timezone(const std::string& name) const {
        const auto& tp = cctz::convert(cs_, tz_);
        const auto& tz = internal::timezone(name);
        const auto& cs = cctz::convert(tp, tz);

        return DateTime(cs, tz);
    }

    DateTime in_offset(int seconds) const {
        const auto& tp = cctz::convert(cs_, tz_);
        const auto& tz = internal::timezone(std::chrono::seconds(seconds));
        const auto& cs = cctz::convert(tp, tz);

        return DateTime(cs, tz);
    }

    DateTime in_offset_hours(int hours, int minutes = 0) const {
        const auto& tp = cctz::convert(cs_, tz_);
        const auto s = std::chrono::hours(hours) + std::chrono::minutes(minutes);
        const auto& tz = internal::timezone(s);
        const auto& cs = cctz::convert(tp, tz);

        return DateTime(cs, tz);
    }

    //
    // Internals
    //

    const cctz::time_zone& timezone() const { return tz_; }
    const cctz::civil_second& instance() const { return cs_; }

    void instance(const cctz::civil_second& cs, const cctz::time_zone& tz) {
        cs_ = cs;
        tz_ = tz;
    }

   private:
    DateTime start_of_year() const { return on(year(), 1, 1).at(0, 0, 0); }
    DateTime start_of_month() const { return on(year(), month(), 1).at(0, 0, 0); }
    DateTime start_of_day() const { return at(0, 0, 0); }
    DateTime start_of_hour() const { return at(hour(), 0, 0); }
    DateTime start_of_minute() const { return at(hour(), minute(), 0); }

    DateTime start_of_week() const {
        const auto week_day = internal::week_starts_at();

        if (day_of_week() == week_day) {
            return at(0, 0, 0);
        }

        return previous(week_day);
    }

    DateTime end_of_year() const { return on(year(), 12, 31).at(23, 59, 59); }
    DateTime end_of_month() const { return on(year(), month() + 1, 1).add_days(-1).at(23, 59, 59); }
    DateTime end_of_day() const { return at(23, 59, 59); }
    DateTime end_of_hour() const { return at(hour(), 59, 59); }
    DateTime end_of_minute() const { return at(hour(), minute(), 59); }

    DateTime end_of_week() const {
        const auto week_day = internal::week_starts_at();
        return next(week_day).add_days(-1).at(23, 59, 59);
    }

    cctz::civil_second cs_;
    cctz::time_zone tz_;
};

inline std::ostream& operator<<(std::ostream& out, const DateTime& dt) {
    out << dt.to_iso8601_string();
    return out;
}

inline bool operator==(const DateTime& a, const DateTime& b) {
    return a.timestamp() == b.timestamp();
}

inline bool operator!=(const DateTime& a, const DateTime& b) {
    return a.timestamp() != b.timestamp();
}

inline bool operator<(const DateTime& a, const DateTime& b) {
    return a.timestamp() < b.timestamp();
}

inline bool operator<=(const DateTime& a, const DateTime& b) {
    return a.timestamp() <= b.timestamp();
}

inline bool operator>(const DateTime& a, const DateTime& b) {
    return a.timestamp() > b.timestamp();
}

inline bool operator>=(const DateTime& a, const DateTime& b) {
    return a.timestamp() >= b.timestamp();
}

}  // namespace pendulum

namespace std {

template <>
struct hash<pendulum::DateTime> {
    using result_type = size_t;
    using argument_type = pendulum::DateTime;

    hash() = default;
    hash(const hash&) = default;
    hash(hash&&) = default;

    virtual ~hash() = default;

    hash& operator=(const hash&) = default;
    hash& operator=(hash&&) = default;

    size_t operator()(const pendulum::DateTime& dt) const {
        return hash<std::string>()(dt.to_iso8601_string());
    }
};

}  // namespace std

#endif  // PENDULUM_DATETIME_H_
