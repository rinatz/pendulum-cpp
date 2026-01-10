// MIT License

// Copyright (c) 2026 IDA Kenichiro

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

TEST(Parser, Now) {
    const auto& now = datetime(2006, 1, 2, 15, 4, 5).offset_hours(-7);

    pendulum::test(now, [&]() { EXPECT_THAT(parse("now"), now); });
}

TEST(Parser, Iso8601ExtendedFormat1) {
    const auto& dt = parse("2006-01-02T15:04:05-07:00", "MST");

    EXPECT_THAT(dt.to_iso8601_string(), "2006-01-02T15:04:05-07:00");
    EXPECT_THAT(dt, DateTime(2006, 1, 2, 15, 4, 5, "MST"));
}

TEST(Parser, Iso8601ExtendedFormat2) {
    const auto& dt = parse("2006-01-02T15:04:05-0700", "MST");

    EXPECT_THAT(dt.to_iso8601_string(), "2006-01-02T15:04:05-07:00");
    EXPECT_THAT(dt, DateTime(2006, 1, 2, 15, 4, 5, "MST"));
}

TEST(Parser, Iso8601ExtendedFormat3) {
    const auto& dt = parse("2006-01-02T15:04:05Z");

    EXPECT_THAT(dt.to_iso8601_string(), "2006-01-02T15:04:05+00:00");
    EXPECT_THAT(dt, DateTime(2006, 1, 2, 15, 4, 5));
}

TEST(Parser, Iso8601DateTimeFormat1) {
    const auto& dt = parse("20060102T150405-0700", "MST");

    EXPECT_THAT(dt.to_iso8601_string(), "2006-01-02T15:04:05-07:00");
    EXPECT_THAT(dt, DateTime(2006, 1, 2, 15, 4, 5, "MST"));
}

TEST(Parser, Iso8601DateTimeFormat2) {
    const auto& dt = parse("2006-01-02 15:04:05");

    EXPECT_THAT(dt.to_iso8601_string(), "2006-01-02T15:04:05+00:00");
    EXPECT_THAT(dt, DateTime(2006, 1, 2, 15, 4, 5));
}

TEST(Parser, Iso8601DateFormat1) {
    const auto& dt = parse("2006-01-02");

    EXPECT_THAT(dt, DateTime(2006, 1, 2));
}

TEST(Parser, Iso8601DateFormat2) {
    const auto& dt = parse("2006-01");

    EXPECT_THAT(dt, DateTime(2006, 1, 1));
}

TEST(Parser, Iso8601DateFormat3) {
    const auto& dt = parse("20060102");

    EXPECT_THAT(dt, DateTime(2006, 1, 2));
}

TEST(Parser, Iso8601DateFormat4) {
    const auto& dt = parse("2006");

    EXPECT_THAT(dt, DateTime(2006, 1, 1));
}

TEST(Parser, Iso8601DateFormatWithFormat) {
    const auto& dt = from_format("100001231", "%Y%m%d");

    EXPECT_THAT(dt, DateTime(10000, 12, 31));
}

TEST(Parser, UnsupportedFormat1) {
    DateTime dt;

    EXPECT_THROW(dt = parse("10000"), PendulumException);
    EXPECT_THAT(dt, DateTime::epoch());
}

TEST(Parser, UnsupportedFormat2) {
    DateTime dt;

    EXPECT_THROW(dt = parse("2006/01/02", "Asia/Tokyo"), PendulumException);
    EXPECT_THAT(dt, DateTime::epoch());
}

TEST(Parser, UnsupportedFormat3) {
    DateTime dt;

    EXPECT_THROW(dt = from_format("2006-01-02", "%Y%m%d", "Asia/Tokyo"), PendulumException);
    EXPECT_THAT(dt, DateTime::epoch());
}

TEST(Parser, UnsupportedFormat4) {
    DateTime dt;

    EXPECT_THROW(dt = from_format("yyyymmdd", "%Y%m%d", "Asia/Tokyo"), PendulumException);
    EXPECT_THAT(dt, DateTime::epoch());
}

TEST(Parser, InvalidDateTime) {
    EXPECT_THROW(parse("2023-02-30"), PendulumException);
    EXPECT_THROW(parse("2023-13-01"), PendulumException);
    EXPECT_THROW(parse("2023-01-01T25:00:00"), PendulumException);
    EXPECT_THROW(parse("2023-01-01T00:61:00"), PendulumException);
    EXPECT_THROW(parse("2023-01-01T00:00:61"), PendulumException);
}

TEST(Parser, InvalidDateTimeWithFormat) {
    EXPECT_THROW(from_format("2023-02-30", "%Y-%m-%d"), PendulumException);
    EXPECT_THROW(from_format("2023-13-01", "%Y-%m-%d"), PendulumException);
    EXPECT_THROW(from_format("2023-01-01 25:00:00", "%Y-%m-%d %H:%M:%S"), PendulumException);
    EXPECT_THROW(from_format("2023-01-01 00:61:00", "%Y-%m-%d %H:%M:%S"), PendulumException);
    EXPECT_THROW(from_format("2023-01-01 00:00:61", "%Y-%m-%d %H:%M:%S"), PendulumException);
    EXPECT_THROW(from_format("not-a-date", "%Y-%m-%d"), PendulumException);
}

}  // namespace pendulum
