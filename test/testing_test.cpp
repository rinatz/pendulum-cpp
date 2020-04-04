#include <gmock/gmock.h>

#include "pendulum/pendulum.h"
#include "pendulum/testing.h"

namespace pendulum {

using testing::Not;

TEST(DateTimeTest, Testing1) {
    const auto& now = pendulum::datetime(2000, 12, 25, 16, 30, 50);

    pendulum::set_test_now(now);

    EXPECT_THAT(pendulum::now(), now);
    EXPECT_THAT(pendulum::today(), pendulum::datetime(2000, 12, 25));
    EXPECT_THAT(pendulum::yesterday(), pendulum::datetime(2000, 12, 24));
    EXPECT_THAT(pendulum::tomorrow(), pendulum::datetime(2000, 12, 26));

    pendulum::set_test_now();

    EXPECT_THAT(pendulum::now(), Not(now));
    EXPECT_THAT(pendulum::today(), Not(pendulum::datetime(2000, 12, 25)));
    EXPECT_THAT(pendulum::yesterday(), Not(pendulum::datetime(2000, 12, 24)));
    EXPECT_THAT(pendulum::tomorrow(), Not(pendulum::datetime(2000, 12, 26)));
}

TEST(DateTimeTest, Testing2) {
    const auto& now = pendulum::datetime(2000, 12, 25, 16, 30, 50);

    pendulum::test(now, [&]() {
        EXPECT_THAT(pendulum::now(), now);
        EXPECT_THAT(pendulum::today(), pendulum::datetime(2000, 12, 25));
        EXPECT_THAT(pendulum::yesterday(), pendulum::datetime(2000, 12, 24));
        EXPECT_THAT(pendulum::tomorrow(), pendulum::datetime(2000, 12, 26));
    });

    EXPECT_THAT(pendulum::now(), Not(now));
    EXPECT_THAT(pendulum::today(), Not(pendulum::datetime(2000, 12, 25)));
    EXPECT_THAT(pendulum::yesterday(), Not(pendulum::datetime(2000, 12, 24)));
    EXPECT_THAT(pendulum::tomorrow(), Not(pendulum::datetime(2000, 12, 26)));
}

}  // namespace pendulum
