#include <gmock/gmock.h>

#include "pendulum/parser.h"

namespace pendulum {

TEST(DateTimeTest, Parsing1) {
    const auto& dt = parse("2020-04-03T15:20:30+09:00");

    ASSERT_THAT(dt.has_value(), true);
    EXPECT_THAT(dt->to_iso8601_string(), "2020-04-03T06:20:30+00:00");
    EXPECT_THAT(*dt, DateTime(2020, 4, 3, 15, 20, 30, "Asia/Tokyo"));
}

TEST(DateTimeTest, Parsing2) {
    const auto& dt = parse("2020-04-03T15:20:30+0900");

    ASSERT_THAT(dt.has_value(), true);
    EXPECT_THAT(dt->to_iso8601_string(), "2020-04-03T06:20:30+00:00");
    EXPECT_THAT(*dt, DateTime(2020, 4, 3, 15, 20, 30, "Asia/Tokyo"));
}

TEST(DateTimeTest, Parsing3) {
    const auto& dt = parse("2020-04-03 15:20:30", "Asia/Tokyo");

    ASSERT_THAT(dt.has_value(), true);
    EXPECT_THAT(*dt, DateTime(2020, 4, 3, 15, 20, 30, "Asia/Tokyo"));
}

TEST(DateTimeTest, Parsing4) {
    const auto& dt = parse("2020-04-03", "Asia/Tokyo");

    ASSERT_THAT(dt.has_value(), true);
    EXPECT_THAT(*dt, DateTime(2020, 4, 3, "Asia/Tokyo"));
}

TEST(DateTimeTest, Parsing5) {
    const auto& dt = parse("20200403", "Asia/Tokyo");

    ASSERT_THAT(dt.has_value(), true);
    EXPECT_THAT(*dt, DateTime(2020, 4, 3, "Asia/Tokyo"));
}

TEST(DateTimeTest, Parsing6) {
    const auto& dt = parse("2020-04", "Asia/Tokyo");

    ASSERT_THAT(dt.has_value(), true);
    EXPECT_THAT(*dt, DateTime(2020, 4, 1, "Asia/Tokyo"));
}

TEST(DateTimeTest, Parsing7) {
    const auto& dt = parse("2020", "Asia/Tokyo");

    ASSERT_THAT(dt.has_value(), true);
    EXPECT_THAT(*dt, DateTime(2020, 1, 1, "Asia/Tokyo"));
}

TEST(DateTimeTest, Parsing8) {
    const auto& dt = parse("2020/12/25", "Asia/Tokyo");

    ASSERT_THAT(dt.has_value(), false);
    EXPECT_THAT(dt.value_or(DateTime::epoch()), DateTime::epoch());
}

}  // namespace pendulum
