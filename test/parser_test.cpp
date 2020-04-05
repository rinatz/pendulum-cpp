#include <gmock/gmock.h>

#include "pendulum/parser.h"

namespace pendulum {

TEST(DateTimeTest, Parsing1) {
    const auto& dt = parse("2020-04-03", "Asia/Tokyo");

    ASSERT_THAT(dt.has_value(), true);
    EXPECT_THAT(*dt, DateTime(2020, 4, 3, "Asia/Tokyo"));
}

TEST(DateTimeTest, Parsing2) {
    const auto& dt = parse("2020/04/03", "Asia/Tokyo");

    ASSERT_THAT(dt.has_value(), true);
    EXPECT_THAT(dt.value(), DateTime(2020, 4, 3, "Asia/Tokyo"));
}

TEST(DateTimeTest, Parsing3) {
    const auto& dt = parse("20200403", "Asia/Tokyo");

    ASSERT_THAT(dt.has_value(), true);
    EXPECT_THAT(dt.value_or(DateTime::epoch()), DateTime(2020, 4, 3, "Asia/Tokyo"));
}

TEST(DateTimeTest, Parsing4) {
    const auto& dt = parse("10", "Asia/Tokyo");

    ASSERT_THAT(dt.has_value(), false);
    EXPECT_THAT(dt.value_or(DateTime::epoch()), DateTime::epoch());
}

}  // namespace pendulum
