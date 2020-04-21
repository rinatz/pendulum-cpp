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

#include "pendulum/exceptions.h"
#include "pendulum/parser.h"

namespace pendulum {

TEST(Parser, ISO8601String1) {
    const auto& dt = parse("2020-04-03T15:20:30+09:00");

    EXPECT_THAT(dt.to_iso8601_string(), "2020-04-03T06:20:30+00:00");
    EXPECT_THAT(dt, DateTime(2020, 4, 3, 15, 20, 30, "Asia/Tokyo"));
}

TEST(Parser, ISO8601String2) {
    const auto& dt = parse("2020-04-03T15:20:30+0900");

    EXPECT_THAT(dt.to_iso8601_string(), "2020-04-03T06:20:30+00:00");
    EXPECT_THAT(dt, DateTime(2020, 4, 3, 15, 20, 30, "Asia/Tokyo"));
}

TEST(Parser, DateTimeString) {
    const auto& dt = parse("2020-04-03 15:20:30", "Asia/Tokyo");

    EXPECT_THAT(dt, DateTime(2020, 4, 3, 15, 20, 30, "Asia/Tokyo"));
}

TEST(Parser, DateString1) {
    const auto& dt = parse("2020-04-03", "Asia/Tokyo");

    EXPECT_THAT(dt, DateTime(2020, 4, 3, "Asia/Tokyo"));
}

TEST(Parser, DateString2) {
    const auto& dt = parse("20200403", "Asia/Tokyo");

    EXPECT_THAT(dt, DateTime(2020, 4, 3, "Asia/Tokyo"));
}

TEST(Parser, YearMonthString) {
    const auto& dt = parse("2020-04", "Asia/Tokyo");

    EXPECT_THAT(dt, DateTime(2020, 4, 1, "Asia/Tokyo"));
}

TEST(Parser, YearString) {
    const auto& dt = parse("2020", "Asia/Tokyo");

    EXPECT_THAT(dt, DateTime(2020, 1, 1, "Asia/Tokyo"));
}

TEST(Parser, InvalidString) {
    DateTime dt;

    EXPECT_THROW(dt = parse("2020/12/25", "Asia/Tokyo"), PendulumException);
    EXPECT_THAT(dt, DateTime::epoch());
}

TEST(Parser, FormatUnmatched) {
    DateTime dt;

    EXPECT_THROW(dt = from_format("2020-04-20", "%Y%m%d", "Asia/Tokyo"), PendulumException);
    EXPECT_THAT(dt, DateTime::epoch());
}

}  // namespace pendulum
