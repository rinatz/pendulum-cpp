#include <gmock/gmock.h>

#include "pendulum/date.h"

namespace pendulum {

TEST(DateTest, Instantiation1) {
    Date date;

    EXPECT_THAT(date.year(), 1970);
    EXPECT_THAT(date.month(), 1);
    EXPECT_THAT(date.day(), 1);
}

TEST(DateTest, Instantiation2) {
    Date date(2020, 4, 17);

    EXPECT_THAT(date.year(), 2020);
    EXPECT_THAT(date.month(), 4);
    EXPECT_THAT(date.day(), 17);
}

TEST(DateTest, Attributes) {
    Date date(2020, 4, 17);

    EXPECT_THAT(date.day_of_week(), pendulum::kFriday);
    EXPECT_THAT(date.day_of_year(), 108);
    EXPECT_THAT(date.week_of_month(), 3);
}

TEST(DateTest, FluentHelpers) {
    Date date(2020, 4, 17);

    EXPECT_THAT(date.year(2021), Date(2021, 4, 17));
    EXPECT_THAT(date.month(5), Date(2020, 5, 17));
    EXPECT_THAT(date.day(20), Date(2020, 4, 20));
    EXPECT_THAT(date.on(2021, 5, 10), Date(2021, 5, 10));
}

TEST(DateTest, StringFormatting) {
    Date date(2020, 4, 17);

    EXPECT_THAT(date.to_date_string(), "2020-04-17");
    EXPECT_THAT(date.format("I was born on %d/%m %Y"), "I was born on 17/04 2020");
}

TEST(DateTest, Addition) {
    Date date(2020, 12, 31);

    EXPECT_THAT(date.add_years(1), Date(2021, 12, 31));
    EXPECT_THAT(date.add_months(1), Date(2021, 1, 31));
    EXPECT_THAT(date.add_days(1), Date(2021, 1, 1));
}

TEST(DateTest, Modifiers) {
    Date date(2020, 4, 17);

    EXPECT_THAT(date.next(), Date(2020, 4, 24));
    EXPECT_THAT(date.next(pendulum::kMonday), Date(2020, 4, 20));

    EXPECT_THAT(date.previous(), Date(2020, 4, 10));
    EXPECT_THAT(date.previous(pendulum::kWednesday), Date(2020, 4, 15));

    EXPECT_THAT(date.start_of("day"), Date(2020, 4, 17));
    EXPECT_THAT(date.start_of("week"), Date(2020, 4, 13));
    EXPECT_THAT(date.start_of("month"), Date(2020, 4, 1));
    EXPECT_THAT(date.start_of("year"), Date(2020, 1, 1));
}

}  // namespace pendulum
