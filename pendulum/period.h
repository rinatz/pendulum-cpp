#ifndef PENDULUM_PERIOD_H_
#define PENDULUM_PERIOD_H_

#include <functional>
#include <string>

#include "pendulum/datetime.h"
#include "pendulum/optional.h"

namespace pendulum {

class DateTimeIterator {
   public:
    using Add = std::function<DateTime(const DateTime*)>;
    using Compare = std::function<bool(const DateTime&, const DateTime&)>;

    DateTimeIterator() = default;

    DateTimeIterator(const DateTime& start, const DateTime& stop, const Add& add,
                     const Compare& cmp)
            : dt_(start), stop_(stop), add_(add), cmp_(cmp) {}

    DateTimeIterator& operator++() {
        next();
        return *this;
    }

    DateTimeIterator operator++(int) {
        const auto& this_ = *this;
        next();
        return this_;
    }

    const DateTime& operator*() const { return *dt_; }
    const DateTime* operator->() const { return &dt_.value(); }

    bool operator==(const DateTimeIterator& that) const {
        return dt_ == that.dt_ && stop_ == that.stop_;
    }

    bool operator!=(const DateTimeIterator& that) const { return !operator==(that); }

   private:
    void next() {
        *dt_ = add_(&dt_.value());

        if (!cmp_(*dt_, *stop_)) {
            clear();
        }
    }

    void clear() {
        dt_ = nullopt;
        stop_ = nullopt;
        add_ = nullptr;
        cmp_ = nullptr;
    }

    Optional<DateTime> dt_;
    Optional<DateTime> stop_;
    Add add_;
    Compare cmp_;
};

class DateTimeRange {
   public:
    using Add = DateTimeIterator::Add;
    using Compare = DateTimeIterator::Compare;

    DateTimeRange() = default;

    DateTimeRange(const DateTime& start, const DateTime& stop, const Add& add, const Compare& cmp)
            : begin_(start, stop, add, cmp), end_() {}

    const DateTimeIterator& begin() const { return begin_; }
    const DateTimeIterator& end() const { return end_; }

   private:
    DateTimeIterator begin_;
    DateTimeIterator end_;
};

class Period {
   public:
    Period() = default;

    Period(const DateTime& start, const DateTime& stop, bool absolute = false)
            : start_(start), stop_(stop) {
        if (start > stop) {
            if (absolute) {
                start_ = stop;
                stop_ = start;
            } else {
                inverse_ = true;
            }
        }
    }

    const DateTime& start() const { return start_; }
    const DateTime& stop() const { return stop_; }

    int in_weeks() const { return in_days() / 7; }
    int remaining_days() const { return in_days() % 7; }
    int in_days() const { return in_hours() / 24; }

    int in_hours() const { return in_minutes() / 60; }
    int in_minutes() const { return in_seconds() / 60; }
    int in_seconds() const { return stop_.timestamp() - start_.timestamp(); }

    bool contains(const DateTime& dt) const { return start_ <= dt && dt <= stop_; }

    DateTimeRange range(const std::string& unit, int step = 1) const {
        using namespace std::placeholders;
        DateTimeIterator::Add add;

        if (unit == "years") {
            add = std::bind(&DateTime::add_years, _1, step * sign());
        } else if (unit == "months") {
            add = std::bind(&DateTime::add_months, _1, step * sign());
        } else if (unit == "days") {
            add = std::bind(&DateTime::add_days, _1, step * sign());
        } else if (unit == "hours") {
            add = std::bind(&DateTime::add_hours, _1, step * sign());
        } else if (unit == "minutes") {
            add = std::bind(&DateTime::add_minutes, _1, step * sign());
        } else if (unit == "seconds") {
            add = std::bind(&DateTime::add_seconds, _1, step * sign());
        }

        return DateTimeRange(start_, stop_, add, cmp());
    }

    DateTimeIterator begin() const {
        using namespace std::placeholders;
        const auto& add = std::bind(&DateTime::add_days, _1, sign());
        return DateTimeIterator(start_, stop_, add, cmp());
    }

    DateTimeIterator end() const { return DateTimeIterator(); }

   private:
    DateTimeIterator::Compare cmp() const {
        if (inverse_) {
            return [](const DateTime& dt, const DateTime& stop) { return dt >= stop; };
        }
        return [](const DateTime& dt, const DateTime& stop) { return dt <= stop; };
    }

    int sign() const {
        if (inverse_) {
            return -1;
        }
        return 1;
    }

    DateTime start_;
    DateTime stop_;
    bool inverse_ = false;
};

}  // namespace pendulum

#endif  // PENDULUM_PERIOD_H_
