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

#ifndef PENDULUM_DATE_H_
#define PENDULUM_DATE_H_

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

    explicit Date(int y, int m, int d) : ymd_(y, m, d) {}

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

    int day_of_week() const { return static_cast<int>(cctz::get_weekday(ymd_)); }
    int day_of_year() const { return cctz::get_yearday(ymd_); }
    int week_of_month() const { return (day() + 6) / 7; }

    bool is_leap_year() const { return on(year(), 2, 29).month() == 2; }

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
    // Additions
    //

    Date add_years(int y) const { return add(y, 0, 0); }
    Date add_months(int m) const { return add(0, m, 0); }
    Date add_days(int d) const { return add(0, 0, d); }
    Date add(int y, int m, int d) const { return Date(year() + y, month() + m, day() + d); }

    //
    // Modifiers
    //

    Date next() const { return next(day_of_week()); }

    Date next(int day_of_week) const {
        return Date(cctz::next_weekday(ymd_, static_cast<cctz::weekday>(day_of_week)));
    }

    Date previous() const { return previous(day_of_week()); }

    Date previous(int day_of_week) const {
        return Date(cctz::prev_weekday(ymd_, static_cast<cctz::weekday>(day_of_week)));
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
        throw PendulumException("Invalid unit for start_of(): " + unit);
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
        throw PendulumException("Invalid unit for end_of(): " + unit);
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
        const auto week_day = internal::week_starts_at();

        if (day_of_week() == week_day) {
            return *this;
        }

        return previous(week_day);
    }

    Date end_of_year() const { return on(year(), 12, 31); }
    Date end_of_month() const { return on(year(), month() + 1, 1).add_days(-1); }
    Date end_of_day() const { return *this; }

    Date end_of_week() const {
        const auto week_day = internal::week_starts_at();
        return next(week_day).add_days(-1);
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

#endif  // PENDULUM_DATE_H_
