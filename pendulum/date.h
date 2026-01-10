// MIT License
//
// Copyright (c) 2026 IDA Kenichiro
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#ifndef PENDULUM_DATE_H_
#define PENDULUM_DATE_H_

#include <functional>
#include <string>

#include <cctz/civil_time.h>
#include <cctz/time_zone.h>

#include "pendulum/constants.h"
#include "pendulum/exceptions.h"

namespace pendulum {

class Date {
   public:
    //
    // Instantiation
    //

    Date() : ymd_(1970, 1, 1) {}
    Date(int y, int m, int d) : ymd_(y, m, d) {}
    explicit Date(const cctz::civil_day& ymd) : ymd_(ymd) {}
    Date(const Date&) = default;
    Date(Date&&) = default;

    virtual ~Date() = default;

    Date& operator=(const Date&) = default;
    Date& operator=(Date&&) = default;

    //
    // Attributes
    //

    int year() const { return ymd_.year(); }
    int month() const { return ymd_.month(); }
    int day() const { return ymd_.day(); }

    Weekday day_of_week() const {
        auto wday = static_cast<int>(cctz::get_weekday(ymd_));
        return static_cast<Weekday>((wday + 1) % 7);
    }

    int day_of_year() const { return cctz::get_yearday(ymd_); }
    int week_of_month() const { return week_of_year() - day(1).week_of_year() + 1; }
    int week_of_year() const { return std::stoi(format("%V")); }

    bool is_leap_year() const {
        return (year() % 4 == 0 && year() % 100 != 0) || year() % 400 == 0;
    }

    int days_in_month() const {
        if (month() == 2) {
            return is_leap_year() ? 29 : 28;
        }
        if (month() == 4 || month() == 6 || month() == 9 || month() == 11) {
            return 30;
        }
        return 31;
    }

    //
    // Fluent helpers
    //

    Date year(int y) const { return on(y, month(), day()); }
    Date month(int m) const { return on(year(), m, day()); }
    Date day(int d) const { return on(year(), month(), d); }
    Date on(int y, int m, int d) const { return Date(y, m, d); }

    //
    // String formatting
    //

    std::string to_date_string() const { return format("%Y-%m-%d"); }

    std::string format(const std::string& fmt) const {
        const auto& tz = cctz::utc_time_zone();
        const auto& tp = cctz::convert(ymd_, tz);

        return cctz::format(fmt, tp, tz);
    }

    //
    // Additions and Subtractions
    //

    Date add_years(int y) const { return add(y, 0, 0); }
    Date add_months(int m) const { return add(0, m, 0); }
    Date add_days(int d) const { return add(0, 0, d); }

    Date add(int y, int m, int d) const {
        int new_year = year() + y;
        int new_month = month() + m;

        // Adjust month and year
        if (new_month > 12) {
            new_year += (new_month - 1) / 12;
            new_month = (new_month - 1) % 12 + 1;
        } else if (new_month < 1) {
            new_year += (new_month - 12) / 12;
            if ((new_month - 12) % 12 != 0) {
                new_month = (new_month - 12) % 12 + 12;
            } else {
                new_month = 12;
            }
        }

        // Clamp day for month addition/subtraction
        const int days_in_new_month = Date(new_year, new_month, 1).days_in_month();
        const int clamped_day = std::min(day(), days_in_new_month);

        // Add the day delta to the clamped day, let cctz handle the normalization
        return Date(cctz::civil_day(new_year, new_month, clamped_day + d));
    }

    Date subtract_years(int y) const { return add_years(-y); }
    Date subtract_months(int m) const { return add_months(-m); }
    Date subtract_days(int d) const { return add_days(-d); }
    Date subtract(int y, int m, int d) const { return add(-y, -m, -d); }

    //
    // Modifiers
    //

    Date next() const { return next(day_of_week()); }

    Date next(Weekday weekday) const {
        auto wday = (static_cast<int>(weekday) + 6) % 7;
        return Date(cctz::next_weekday(ymd_, static_cast<cctz::weekday>(wday)));
    }

    Date previous() const { return previous(day_of_week()); }

    Date previous(Weekday weekday) const {
        auto wday = (static_cast<int>(weekday) + 6) % 7;
        return Date(cctz::prev_weekday(ymd_, static_cast<cctz::weekday>(wday)));
    }

    Date start_of(const std::string& unit) const {
        if (unit == "year") {
            return start_of_year();
        }
        if (unit == "month") {
            return start_of_month();
        }
        if (unit == "day") {
            return start_of_day();
        }
        if (unit == "week") {
            return start_of_week();
        }
        throw InvalidUnit("unit for start_of(): " + unit);
    }

    Date end_of(const std::string& unit) const {
        if (unit == "year") {
            return end_of_year();
        }
        if (unit == "month") {
            return end_of_month();
        }
        if (unit == "day") {
            return end_of_day();
        }
        if (unit == "week") {
            return end_of_week();
        }
        throw InvalidUnit("unit for end_of(): " + unit);
    }

    //
    // Internals
    //

    void instance(const cctz::civil_day& ymd) { ymd_ = ymd; }

    const cctz::civil_day& instance() const { return ymd_; }

   private:
    Date start_of_year() const { return on(year(), 1, 1); }
    Date start_of_month() const { return on(year(), month(), 1); }
    Date start_of_day() const { return *this; }

    Date start_of_week() const {
        const auto wday = internal::week_starts_at();

        if (day_of_week() == wday) {
            return *this;
        }

        return previous(wday);
    }

    Date end_of_year() const { return on(year(), 12, 31); }
    Date end_of_month() const { return on(year(), month() + 1, 1).add_days(-1); }
    Date end_of_day() const { return *this; }

    Date end_of_week() const {
        const auto weekday = internal::week_starts_at();
        return next(weekday).add_days(-1);
    }

    cctz::civil_day ymd_;
};

inline std::ostream& operator<<(std::ostream& out, const Date& date) {
    out << date.to_date_string();
    return out;
}

inline bool operator==(const Date& a, const Date& b) { return a.instance() == b.instance(); }
inline bool operator!=(const Date& a, const Date& b) { return a.instance() != b.instance(); }
inline bool operator<(const Date& a, const Date& b) { return a.instance() < b.instance(); }
inline bool operator<=(const Date& a, const Date& b) { return a.instance() <= b.instance(); }
inline bool operator>(const Date& a, const Date& b) { return a.instance() > b.instance(); }
inline bool operator>=(const Date& a, const Date& b) { return a.instance() >= b.instance(); }

}  // namespace pendulum

namespace std {

template <>
struct hash<pendulum::Date> {
    using result_type = size_t;
    using argument_type = pendulum::Date;

    hash() = default;
    hash(const hash&) = default;
    hash(hash&&) = default;

    virtual ~hash() = default;

    hash& operator=(const hash&) = default;
    hash& operator=(hash&&) = default;

    size_t operator()(const pendulum::Date& date) const {
        return hash<std::string>()(date.to_date_string());
    }
};

}  // namespace std

#endif  // PENDULUM_DATE_H_
