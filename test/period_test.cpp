#include <gmock/gmock.h>

#include <iterator>
#include <unordered_map>
#include <vector>

#include "pendulum/datetime.h"
#include "pendulum/period.h"

namespace pendulum {

using testing::ElementsAre;

TEST(Period, Attributes) {
    DateTime start(2020, 1, 1);
    DateTime stop(2030, 1, 1);
    Period period(start, stop);

    EXPECT_THAT(period.in_weeks(), 521);
    EXPECT_THAT(period.in_days(), 3653);
    EXPECT_THAT(period.in_hours(), 87672);
    EXPECT_THAT(period.in_minutes(), 5260320);
    EXPECT_THAT(period.in_seconds(), 315619200);
}

TEST(Period, RangeYears) {
    DateTime start(2020, 1, 1);
    DateTime stop(2024, 12, 31);
    Period period(start, stop);

    std::vector<DateTime> years;

    for (const auto& dt : period.range("years")) {
        years.push_back(dt);
    }

    // clang-format off
    EXPECT_THAT(years, ElementsAre(
        DateTime(2020, 1, 1),
        DateTime(2021, 1, 1),
        DateTime(2022, 1, 1),
        DateTime(2023, 1, 1),
        DateTime(2024, 1, 1)
    ));
    // clang-format on
}

TEST(Period, RangeMonths) {
    DateTime start(2020, 1, 5);
    DateTime stop(2020, 5, 1);
    Period period(start, stop);

    std::vector<DateTime> months;

    for (const auto& dt : period.range("months")) {
        months.push_back(dt);
    }

    // clang-format off
    EXPECT_THAT(months, ElementsAre(
        DateTime(2020, 1, 5),
        DateTime(2020, 2, 5),
        DateTime(2020, 3, 5),
        DateTime(2020, 4, 5)
    ));
    // clang-format on
}

TEST(Period, RangeDays1) {
    DateTime start(2020, 1, 1, 3);
    DateTime stop(2020, 1, 1, 5);
    Period period(start, stop);

    std::vector<DateTime> days;

    for (const auto& dt : period) {
        days.push_back(dt);
    }

    // clang-format off
    EXPECT_THAT(days, ElementsAre(
        DateTime(2020, 1, 1, 3)
    ));
    // clang-format on
}

TEST(Period, RangeDays2) {
    DateTime start(2020, 1, 1);
    DateTime stop(2020, 1, 5);
    Period period(start, stop);

    std::vector<DateTime> days;

    for (const auto& dt : period.range("days")) {
        days.push_back(dt);
    }

    // clang-format off
    EXPECT_THAT(days, ElementsAre(
        DateTime(2020, 1, 1),
        DateTime(2020, 1, 2),
        DateTime(2020, 1, 3),
        DateTime(2020, 1, 4),
        DateTime(2020, 1, 5)
    ));
    // clang-format on
}

TEST(Period, RangeHours) {
    DateTime start(2020, 1, 1, 0, 5);
    DateTime stop(2020, 1, 1, 5);
    Period period(start, stop);

    std::vector<DateTime> hours;

    for (const auto& dt : period.range("hours")) {
        hours.push_back(dt);
    }

    // clang-format off
    EXPECT_THAT(hours, ElementsAre(
        DateTime(2020, 1, 1, 0, 5),
        DateTime(2020, 1, 1, 1, 5),
        DateTime(2020, 1, 1, 2, 5),
        DateTime(2020, 1, 1, 3, 5),
        DateTime(2020, 1, 1, 4, 5)
    ));
    // clang-format on
}

TEST(Period, RangeMinutes) {
    DateTime start(2020, 1, 1, 0, 1, 5);
    DateTime stop(2020, 1, 1, 0, 5);
    Period period(start, stop);

    std::vector<DateTime> minutes;

    for (const auto& dt : period.range("minutes")) {
        minutes.push_back(dt);
    }

    // clang-format off
    EXPECT_THAT(minutes, ElementsAre(
        DateTime(2020, 1, 1, 0, 1, 5),
        DateTime(2020, 1, 1, 0, 2, 5),
        DateTime(2020, 1, 1, 0, 3, 5),
        DateTime(2020, 1, 1, 0, 4, 5)
    ));
    // clang-format on
}

TEST(Period, RangeSeconds) {
    DateTime start(2020, 1, 1, 0, 0, 1);
    DateTime stop(2020, 1, 1, 0, 0, 5);
    Period period(start, stop);

    std::vector<DateTime> seconds;

    for (const auto& dt : period.range("seconds")) {
        seconds.push_back(dt);
    }

    // clang-format off
    EXPECT_THAT(seconds, ElementsAre(
        DateTime(2020, 1, 1, 0, 0, 1),
        DateTime(2020, 1, 1, 0, 0, 2),
        DateTime(2020, 1, 1, 0, 0, 3),
        DateTime(2020, 1, 1, 0, 0, 4),
        DateTime(2020, 1, 1, 0, 0, 5)
    ));
    // clang-format on
}

TEST(Period, Absolute) {
    DateTime start(2020, 12, 31);
    DateTime stop(2020, 1, 1);
    const auto absolute = true;
    Period period(start, stop, absolute);

    EXPECT_THAT(period.start(), stop);
    EXPECT_THAT(period.stop(), start);
}

TEST(Period, Inverse) {
    DateTime start(2020, 12, 31);
    DateTime stop(2020, 12, 26);
    Period period(start, stop);

    EXPECT_THAT(period.start(), start);
    EXPECT_THAT(period.stop(), stop);

    std::vector<DateTime> days;

    for (const auto& dt : period.range("days", 2)) {
        days.push_back(dt);
    }

    // clang-format off
    EXPECT_THAT(days, ElementsAre(
        DateTime(2020, 12, 31),
        DateTime(2020, 12, 29),
        DateTime(2020, 12, 27)
    ));
    // clang-format on
}

TEST(Period, UnorderedMap) {
    std::unordered_map<Period, int> map{
            {Period(), 1},
    };

    EXPECT_THAT(map[Period()], 1);
}

}  // namespace pendulum
