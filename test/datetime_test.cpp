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

#include <gmock/gmock.h>

#include <map>
#include <unordered_map>

#include "pendulum/datetime.h"
#include "pendulum/exceptions.h"

namespace pendulum {

TEST(DateTime, Instantiation1) {
    DateTime dt;

    EXPECT_THAT(dt.year(), 1970);
    EXPECT_THAT(dt.month(), 1);
    EXPECT_THAT(dt.day(), 1);
    EXPECT_THAT(dt.hour(), 0);
    EXPECT_THAT(dt.minute(), 0);
    EXPECT_THAT(dt.second(), 0);
    EXPECT_THAT(dt.timezone_name(), "UTC");
    EXPECT_THAT(dt, DateTime::epoch());
}

TEST(DateTime, InvalidTimezone) {
    EXPECT_THROW(DateTime(2020, 4, 3, "invalid"), PendulumException);
}

TEST(DateTime, Instantiation2) {
    DateTime dt(2020, 4, 3, "Asia/Tokyo");

    EXPECT_THAT(dt.year(), 2020);
    EXPECT_THAT(dt.month(), 4);
    EXPECT_THAT(dt.day(), 3);
    EXPECT_THAT(dt.hour(), 0);
    EXPECT_THAT(dt.minute(), 0);
    EXPECT_THAT(dt.second(), 0);
    EXPECT_THAT(dt.timezone_name(), "Asia/Tokyo");
}

TEST(DateTime, Instantiation3) {
    DateTime dt(2020, 4, 3, 15, 30, 10, "Asia/Tokyo");

    EXPECT_THAT(dt.year(), 2020);
    EXPECT_THAT(dt.month(), 4);
    EXPECT_THAT(dt.day(), 3);
    EXPECT_THAT(dt.hour(), 15);
    EXPECT_THAT(dt.minute(), 30);
    EXPECT_THAT(dt.second(), 10);
    EXPECT_THAT(dt.timezone_name(), "Asia/Tokyo");
}

TEST(DateTime, Instantiation4) {
    DateTime dt(Date(2020, 4, 3), "Asia/Tokyo");

    EXPECT_THAT(dt.year(), 2020);
    EXPECT_THAT(dt.month(), 4);
    EXPECT_THAT(dt.day(), 3);
    EXPECT_THAT(dt.hour(), 0);
    EXPECT_THAT(dt.minute(), 0);
    EXPECT_THAT(dt.second(), 0);
    EXPECT_THAT(dt.timezone_name(), "Asia/Tokyo");
}

TEST(DateTime, Attributes) {
    DateTime dt(2020, 4, 3, 15, 30, 10, "US/Eastern");

    EXPECT_THAT(dt.date(), Date(2020, 4, 3));
    EXPECT_THAT(dt.day_of_week(), 4);
    EXPECT_THAT(dt.day_of_year(), 94);
    EXPECT_THAT(dt.week_of_month(), 1);

    EXPECT_THAT(dt.is_leap_year(), true);
    EXPECT_THAT(dt.year(2021).is_leap_year(), false);
    EXPECT_THAT(dt.year(2024).is_leap_year(), true);
    EXPECT_TRUE(dt.is_dst());
    EXPECT_FALSE(dt.is_local());
    EXPECT_FALSE(dt.is_utc());

    EXPECT_THAT(dt.offset(), -14400);
    EXPECT_THAT(dt.offset_hours(), -4.0);

    EXPECT_THAT(dt.timestamp(), 1585942210);
    EXPECT_THAT(DateTime::epoch().timestamp(), 0);
}

TEST(DateTime, FluentHelpers) {
    DateTime dt(2020, 4, 3, "Asia/Tokyo");

    EXPECT_THAT(dt.year(2021), DateTime(2021, 4, 3, "Asia/Tokyo"));
    EXPECT_THAT(dt.month(5), DateTime(2020, 5, 3, "Asia/Tokyo"));
    EXPECT_THAT(dt.day(10), DateTime(2020, 4, 10, "Asia/Tokyo"));
    EXPECT_THAT(dt.hour(15), DateTime(2020, 4, 3, 15, 0, 0, "Asia/Tokyo"));
    EXPECT_THAT(dt.minute(30), DateTime(2020, 4, 3, 0, 30, 0, "Asia/Tokyo"));
    EXPECT_THAT(dt.second(10), DateTime(2020, 4, 3, 0, 0, 10, "Asia/Tokyo"));

    EXPECT_THAT(dt.on(2021, 12, 25), DateTime(2021, 12, 25, "Asia/Tokyo"));
    EXPECT_THAT(dt.at(15, 30, 10), DateTime(2020, 4, 3, 15, 30, 10, "Asia/Tokyo"));
    EXPECT_THAT(dt.timezone("US/Hawaii"), DateTime(2020, 4, 3, "US/Hawaii"));
    EXPECT_THAT(dt.on(2021, 12, 25).at(15, 30, 10).timezone("US/Hawaii"),
                DateTime(2021, 12, 25, 15, 30, 10, "US/Hawaii"));

    EXPECT_THAT(dt.offset(-14400), DateTime(2020, 4, 3, "US/Eastern"));
    EXPECT_THAT(dt.offset_hours(-4), DateTime(2020, 4, 3, "US/Eastern"));
    EXPECT_THAT(dt.offset_hours(8, 45), DateTime(2020, 4, 3, "Australia/Eucla"));
}

TEST(DateTime, StringFormatting) {
    DateTime dt(2020, 4, 3, 15, 10, 25, "Asia/Tokyo");

    EXPECT_THAT(dt.to_date_string(), "2020-04-03");
    EXPECT_THAT(dt.to_datetime_string(), "2020-04-03 15:10:25");
    EXPECT_THAT(dt.to_iso8601_string(), "2020-04-03T15:10:25+09:00");
    EXPECT_THAT(dt.format("I was born at %H:%M on %d/%m %Y."),
                "I was born at 15:10 on 03/04 2020.");
}

TEST(DateTime, Addition) {
    DateTime dt(2020, 4, 3, 15, 0, 0, "local");

    EXPECT_THAT(dt.add_years(3), DateTime(2023, 4, 3, 15, 0, 0, "local"));
    EXPECT_THAT(dt.add_months(2), DateTime(2020, 6, 3, 15, 0, 0, "local"));
    EXPECT_THAT(dt.add_days(30), DateTime(2020, 5, 3, 15, 0, 0, "local"));

    EXPECT_THAT(dt.add_hours(-24), DateTime(2020, 4, 2, 15, 0, 0, "local"));
    EXPECT_THAT(dt.add_minutes(30), DateTime(2020, 4, 3, 15, 30, 0, "local"));
    EXPECT_THAT(dt.add_seconds(-3600), DateTime(2020, 4, 3, 14, 0, 0, "local"));

    EXPECT_THAT(dt.add_date(1, 2, 3), DateTime(2021, 6, 6, 15, 0, 0, "local"));
    EXPECT_THAT(dt.add_time(-24, 10, 1), DateTime(2020, 4, 2, 15, 10, 1, "local"));
}

TEST(DateTime, Subtraction) {
    DateTime dt(2020, 10, 9, 15, 30, 0, "local");

    EXPECT_THAT(dt.subtract_years(3), DateTime(2017, 10, 9, 15, 30, 0, "local"));
    EXPECT_THAT(dt.subtract_months(2), DateTime(2020, 8, 9, 15, 30, 0, "local"));
    EXPECT_THAT(dt.subtract_days(30), DateTime(2020, 9, 9, 15, 30, 0, "local"));

    EXPECT_THAT(dt.subtract_hours(24), DateTime(2020, 10, 8, 15, 30, 0, "local"));
    EXPECT_THAT(dt.subtract_minutes(30), DateTime(2020, 10, 9, 15, 0, 0, "local"));
    EXPECT_THAT(dt.subtract_seconds(3600), DateTime(2020, 10, 9, 14, 30, 0, "local"));

    EXPECT_THAT(dt.subtract_date(1, 2, 3), DateTime(2019, 8, 6, 15, 30, 0, "local"));
    EXPECT_THAT(dt.subtract_time(24, 10, 1), DateTime(2020, 10, 8, 15, 19, 59, "local"));
}

TEST(DateTime, Modifers) {
    DateTime dt(2020, 4, 3, 15, 0, 0, "local");
    const auto keep_time = true;

    EXPECT_THAT(dt.next(), DateTime(2020, 4, 10, "local"));
    EXPECT_THAT(dt.next(keep_time), DateTime(2020, 4, 10, 15, 0, 0, "local"));
    EXPECT_THAT(dt.next(pendulum::kFriday), DateTime(2020, 4, 10, "local"));
    EXPECT_THAT(dt.next(pendulum::kMonday, keep_time), DateTime(2020, 4, 6, 15, 0, 0, "local"));

    EXPECT_THAT(dt.previous(), DateTime(2020, 3, 27, "local"));
    EXPECT_THAT(dt.previous(keep_time), DateTime(2020, 3, 27, 15, 0, 0, "local"));
    EXPECT_THAT(dt.previous(pendulum::kFriday), DateTime(2020, 3, 27, "local"));
    EXPECT_THAT(dt.previous(pendulum::kMonday, keep_time),
                DateTime(2020, 3, 30, 15, 0, 0, "local"));

    EXPECT_THAT(dt.start_of("year"), DateTime(2020, 1, 1, "local"));
    EXPECT_THAT(dt.start_of("month"), DateTime(2020, 4, 1, "local"));
    EXPECT_THAT(dt.start_of("day"), DateTime(2020, 4, 3, "local"));
    EXPECT_THAT(dt.minute(20).start_of("hour"), DateTime(2020, 4, 3, 15, 0, 0, "local"));
    EXPECT_THAT(dt.minute(20).second(30).start_of("minute"),
                DateTime(2020, 4, 3, 15, 20, 0, "local"));
    EXPECT_THAT(dt.start_of("week"), DateTime(2020, 3, 30, "local"));

    EXPECT_THAT(dt.end_of("year"), DateTime(2020, 12, 31, 23, 59, 59, "local"));
    EXPECT_THAT(dt.end_of("month"), DateTime(2020, 4, 30, 23, 59, 59, "local"));
    EXPECT_THAT(dt.month(2).end_of("month"), DateTime(2020, 2, 29, 23, 59, 59, "local"));
    EXPECT_THAT(dt.end_of("day"), DateTime(2020, 4, 3, 23, 59, 59, "local"));
    EXPECT_THAT(dt.end_of("week"), DateTime(2020, 4, 5, 23, 59, 59, "local"));
    EXPECT_THAT(dt.day(13).end_of("week"), DateTime(2020, 4, 19, 23, 59, 59, "local"));
    EXPECT_THAT(dt.end_of("hour"), DateTime(2020, 4, 3, 15, 59, 59, "local"));
    EXPECT_THAT(dt.end_of("minute"), DateTime(2020, 4, 3, 15, 0, 59, "local"));

    EXPECT_THAT(dt.in_timezone("US/Eastern"), DateTime(2020, 4, 3, 2, 0, 0, "US/Eastern"));
    EXPECT_THAT(dt.in_offset(-14400), DateTime(2020, 4, 3, 2, 0, 0, "US/Eastern"));
    EXPECT_THAT(dt.in_offset_hours(-4), DateTime(2020, 4, 3, 2, 0, 0, "US/Eastern"));
    EXPECT_THAT(dt.in_offset_hours(8, 45), DateTime(2020, 4, 3, 14, 45, 0, "Australia/Eucla"));
}

TEST(DateTime, Map) {
    std::map<DateTime, int> map{
            {DateTime(), 1},
    };

    EXPECT_THAT(map[DateTime()], 1);
}

TEST(DateTime, UnorderedMap) {
    std::unordered_map<DateTime, int> map{
            {DateTime(), 1},
    };

    EXPECT_THAT(map[DateTime()], 1);
}

}  // namespace pendulum
