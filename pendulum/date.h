#ifndef PENDULUM_DATE_H_
#define PENDULUM_DATE_H_

#include <string>

#include <cctz/civil_time.h>
#include <cctz/time_zone.h>

#include "pendulum/constants.h"

namespace pendulum {

class Date {
   public:
    //
    // Instantiation
    //

    Date() : ymd_(1970, 1, 1) {}

    explicit Date(int y, int m = 1, int d = 1) : ymd_(y, m, d) {}

    explicit Date(const cctz::civil_day& ymd) : ymd_(ymd) {}

    //
    // Attributes
    //

    int year() const { return ymd_.year(); }
    int month() const { return ymd_.month(); }
    int day() const { return ymd_.day(); }

    int day_of_week() const { return static_cast<int>(cctz::get_weekday(ymd_)); }
    int day_of_year() const { return cctz::get_yearday(ymd_); }
    int week_of_month() const { return (day() + 6) / 7; }

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

    std::string to_date_string() const {
        return format("%Y-%m-%d");
    }

    std::string format(const std::string& fmt) const {
        const auto& tz = cctz::utc_time_zone();
        const auto& tp = cctz::convert(ymd_, tz);

        return cctz::format(fmt, tp, tz);
    }

    //
    // Addition
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
        Date date = add_days(1);

        while (date.day_of_week() != day_of_week) {
            date = date.add_days(1);
        }

        return date;
    }

    Date previous() const { return previous(day_of_week()); }

    Date previous(int day_of_week) const {
        Date date = add_days(-1);

        while (date.day_of_week() != day_of_week) {
            date = date.add_days(-1);
        }

        return date;
    }

    Date start_of(const std::string& when) const {
        if (when == "day") {
            return *this;
        }
        if (when == "week") {
            if (day_of_week() == kMonday) {
                return *this;
            }
            return previous(kMonday);
        }
        if (when == "month") {
            return on(year(), month(), 1);
        }
        if (when == "year") {
            return on(year(), 1, 1);
        }
        return *this;
    }

    //
    // Internals
    //

    void instance(const cctz::civil_day& ymd) { ymd_ = ymd; }

    const cctz::civil_day& instance() const { return ymd_; }

   private:
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
