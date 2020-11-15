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

#include "pendulum/date.h"

namespace pendulum {

TEST(Date, Instantiation1) {
    Date date;

    EXPECT_THAT(date.year(), 1970);
    EXPECT_THAT(date.month(), 1);
    EXPECT_THAT(date.day(), 1);
}

TEST(Date, Instantiation2) {
    Date date(2020, 4, 17);

    EXPECT_THAT(date.year(), 2020);
    EXPECT_THAT(date.month(), 4);
    EXPECT_THAT(date.day(), 17);
}

TEST(Date, Attributes) {
    Date date(2020, 4, 17);

    EXPECT_THAT(date.day_of_week(), kFriday);
    EXPECT_THAT(date.day_of_week(), 4);
    EXPECT_THAT(date.day_of_year(), 108);
    EXPECT_THAT(date.week_of_month(), 3);

    EXPECT_THAT(date.is_leap_year(), true);
    EXPECT_THAT(date.year(2021).is_leap_year(), false);
    EXPECT_THAT(date.year(2024).is_leap_year(), true);

    EXPECT_THAT(Date(2020, 1, 1).week_of_month(), 1);
    EXPECT_THAT(Date(2020, 1, 7).week_of_month(), 2);
    EXPECT_THAT(Date(2020, 1, 14).week_of_month(), 3);
}

TEST(Date, FluentHelpers) {
    Date date(2020, 4, 17);

    EXPECT_THAT(date.year(2021), Date(2021, 4, 17));
    EXPECT_THAT(date.month(5), Date(2020, 5, 17));
    EXPECT_THAT(date.day(20), Date(2020, 4, 20));
    EXPECT_THAT(date.on(2021, 5, 10), Date(2021, 5, 10));
}

TEST(Date, StringFormatting) {
    Date date(2020, 4, 17);

    EXPECT_THAT(date.to_date_string(), "2020-04-17");
    EXPECT_THAT(date.format("I was born on %d/%m %Y"), "I was born on 17/04 2020");
}

TEST(Date, Addition) {
    Date date(2020, 12, 31);

    EXPECT_THAT(date.add_years(1), Date(2021, 12, 31));
    EXPECT_THAT(date.add_months(1), Date(2021, 1, 31));
    EXPECT_THAT(date.add_days(1), Date(2021, 1, 1));
}

TEST(Date, Subtraction) {
    Date date(2020, 1, 1);

    EXPECT_THAT(date.subtract_years(1), Date(2019, 1, 1));
    EXPECT_THAT(date.subtract_months(1), Date(2019, 12, 1));
    EXPECT_THAT(date.subtract_days(1), Date(2019, 12, 31));
}

TEST(Date, Modifiers) {
    Date date(2020, 4, 17);

    EXPECT_THAT(date.next(), Date(2020, 4, 24));
    EXPECT_THAT(date.next(kMonday), Date(2020, 4, 20));

    EXPECT_THAT(date.previous(), Date(2020, 4, 10));
    EXPECT_THAT(date.previous(kWednesday), Date(2020, 4, 15));

    EXPECT_THAT(date.start_of("year"), Date(2020, 1, 1));
    EXPECT_THAT(date.start_of("month"), Date(2020, 4, 1));
    EXPECT_THAT(date.start_of("day"), Date(2020, 4, 17));
    EXPECT_THAT(date.start_of("week"), Date(2020, 4, 13));

    EXPECT_THAT(date.end_of("year"), Date(2020, 12, 31));
    EXPECT_THAT(date.end_of("month"), Date(2020, 4, 30));
    EXPECT_THAT(date.month(2).end_of("month"), Date(2020, 2, 29));
    EXPECT_THAT(date.end_of("day"), Date(2020, 4, 17));
    EXPECT_THAT(date.end_of("week"), Date(2020, 4, 19));
    EXPECT_THAT(date.day(13).end_of("week"), Date(2020, 4, 19));
}

TEST(Date, Map) {
    std::map<Date, int> map{
            {Date(), 1},
    };

    EXPECT_THAT(map[Date()], 1);
}

TEST(Date, UnorderedMap) {
    std::unordered_map<Date, int> map{
            {Date(), 1},
    };

    EXPECT_THAT(map[Date()], 1);
}

}  // namespace pendulum
