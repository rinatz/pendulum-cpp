// MIT License

// Copyright (c) 2020 IDA Kenichiro

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

#ifndef PENDULUM_PERIOD_H_
#define PENDULUM_PERIOD_H_

#include <functional>
#include <iterator>
#include <string>

#include "pendulum/datetime.h"
#include "pendulum/exceptions.h"
#include "pendulum/internal.h"

namespace pendulum {

class DateTimeIterator {
   public:
    using difference_type = int;
    using value_type = DateTime;
    using pointer = const DateTime*;
    using reference = const DateTime&;
    using iterator_category = std::input_iterator_tag;

    using Add = std::function<DateTime(const DateTime*)>;
    using Compare = std::function<bool(const DateTime&, const DateTime&)>;

    DateTimeIterator() = default;

    DateTimeIterator(const DateTime& start, const DateTime& stop, const Add& add,
                     const Compare& cmp)
            : dt_(start), stop_(stop), add_(add), cmp_(cmp) {}

    DateTimeIterator(const DateTimeIterator&) = default;
    DateTimeIterator(DateTimeIterator&&) = default;

    virtual ~DateTimeIterator() = default;

    DateTimeIterator& operator=(const DateTimeIterator&) = default;
    DateTimeIterator& operator=(DateTimeIterator&&) = default;

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
    const DateTime* operator->() const { return &*dt_; }

    bool operator==(const DateTimeIterator& that) const {
        return dt_ == that.dt_ && stop_ == that.stop_;
    }

    bool operator!=(const DateTimeIterator& that) const { return !operator==(that); }

   private:
    void next() {
        *dt_ = add_(&*dt_);

        if (!cmp_(*dt_, *stop_)) {
            clear();
        }
    }

    void clear() {
        dt_ = internal::nullopt;
        stop_ = internal::nullopt;
        add_ = nullptr;
        cmp_ = nullptr;
    }

    internal::optional<DateTime> dt_;
    internal::optional<DateTime> stop_;
    Add add_;
    Compare cmp_;
};

class DateTimeRange {
   public:
    DateTimeRange() = default;

    DateTimeRange(const DateTimeIterator& begin, const DateTimeIterator& end)
            : begin_(begin), end_(end) {}

    DateTimeRange(const DateTimeRange&) = default;
    DateTimeRange(DateTimeRange&&) = default;

    virtual ~DateTimeRange() = default;

    DateTimeRange& operator=(const DateTimeRange&) = default;
    DateTimeRange& operator=(DateTimeRange&&) = default;

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
            : start_(start), stop_(stop), absolute_(absolute), invert_(false) {
        if (start > stop) {
            if (absolute) {
                start_ = stop;
                stop_ = start;
            } else {
                invert_ = true;
            }
        }
    }

    Period(const Period&) = default;
    Period(Period&&) = default;

    virtual ~Period() = default;

    Period& operator=(const Period&) = default;
    Period& operator=(Period&&) = default;

    const DateTime& start() const { return start_; }
    const DateTime& stop() const { return stop_; }
    bool absolute() const { return absolute_; }

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
        } else {
            throw InvalidUnit("unit for range(): " + unit);
        }

        return DateTimeRange(DateTimeIterator(start_, stop_, add, cmp()), DateTimeIterator());
    }

    DateTimeIterator begin() const {
        using namespace std::placeholders;
        const auto& add = std::bind(&DateTime::add_days, _1, sign());
        return DateTimeIterator(start_, stop_, add, cmp());
    }

    DateTimeIterator end() const { return DateTimeIterator(); }

   private:
    DateTimeIterator::Compare cmp() const {
        return invert_ ? [](const DateTime& dt, const DateTime& stop) { return dt >= stop; }
                       : [](const DateTime& dt, const DateTime& stop) { return dt <= stop; };
    }

    int sign() const { return invert_ ? -1 : 1; }

    DateTime start_;
    DateTime stop_;
    bool absolute_ = false;
    bool invert_ = false;
};

inline bool operator==(const Period& a, const Period& b) {
    return a.start() == b.start() && a.stop() == b.stop() && a.absolute() == b.absolute();
}

inline bool operator!=(const Period& a, const Period& b) { return !operator==(a, b); }

}  // namespace pendulum

namespace std {

template <>
struct hash<pendulum::Period> {
    using result_type = size_t;
    using argument_type = pendulum::Period;

    hash() = default;
    hash(const hash&) = default;
    hash(hash&&) = default;

    virtual ~hash() = default;

    hash& operator=(const hash&) = default;
    hash& operator=(hash&&) = default;

    size_t operator()(const pendulum::Period& period) const {
        const auto& start = period.start().to_iso8601_string();
        const auto& stop = period.stop().to_iso8601_string();

        const size_t hash_values[] = {
                hash<std::string>()(start + "/" + stop),
                hash<bool>()(period.absolute()),
        };

        size_t seed = 0;

        for (auto hash_value : hash_values) {
            seed ^= hash_value + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        }

        return seed;
    }
};

}  // namespace std

#endif  // PENDULUM_PERIOD_H_
