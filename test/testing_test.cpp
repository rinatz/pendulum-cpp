// MIT License

// Copyright (c) 2020 Ida Kenichiro

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

#include "pendulum/pendulum.h"
#include "pendulum/testing.h"

namespace pendulum {

using testing::Not;

TEST(Testing, Testing1) {
    const auto& test_now = datetime(2000, 12, 25, 16, 30, 50);

    set_test_now(test_now);

    EXPECT_THAT(now(), test_now);
    EXPECT_THAT(today(), datetime(2000, 12, 25));
    EXPECT_THAT(yesterday(), datetime(2000, 12, 24));
    EXPECT_THAT(tomorrow(), datetime(2000, 12, 26));

    set_test_now();

    EXPECT_THAT(now(), Not(test_now));
    EXPECT_THAT(today(), Not(datetime(2000, 12, 25)));
    EXPECT_THAT(yesterday(), Not(datetime(2000, 12, 24)));
    EXPECT_THAT(tomorrow(), Not(datetime(2000, 12, 26)));
}

TEST(Testing, Testing2) {
    const auto& test_now = datetime(2000, 12, 25, 16, 30, 50);

    test(test_now, [&]() {
        EXPECT_THAT(now(), test_now);
        EXPECT_THAT(today(), datetime(2000, 12, 25));
        EXPECT_THAT(yesterday(), datetime(2000, 12, 24));
        EXPECT_THAT(tomorrow(), datetime(2000, 12, 26));
    });

    EXPECT_THAT(now(), Not(test_now));
    EXPECT_THAT(today(), Not(datetime(2000, 12, 25)));
    EXPECT_THAT(yesterday(), Not(datetime(2000, 12, 24)));
    EXPECT_THAT(tomorrow(), Not(datetime(2000, 12, 26)));
}

TEST(Testing, Exception) {
    const auto& test_now = datetime(2000, 12, 25, 16, 30, 50);

    try {
        test(test_now, [&]() {
            EXPECT_THAT(now(), test_now);
            throw PendulumException("testing");
        });
    } catch (const PendulumException&) {
        // Check test now will be cleared even if throwed exceptions
    }

    EXPECT_THAT(now(), Not(test_now));
}

}  // namespace pendulum
