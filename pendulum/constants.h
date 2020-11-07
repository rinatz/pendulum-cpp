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

#ifndef PENDULUM_CONSTANTS_H_
#define PENDULUM_CONSTANTS_H_

#include <cctz/civil_time.h>

namespace pendulum {

enum class Weekday : int {
    kMonday = static_cast<int>(cctz::weekday::monday),
    kTuesday = static_cast<int>(cctz::weekday::tuesday),
    kWednesday = static_cast<int>(cctz::weekday::wednesday),
    kThursday = static_cast<int>(cctz::weekday::thursday),
    kFriday = static_cast<int>(cctz::weekday::friday),
    kSaturday = static_cast<int>(cctz::weekday::saturday),
    kSunday = static_cast<int>(cctz::weekday::sunday),
};

static constexpr Weekday kMonday = Weekday::kMonday;
static constexpr Weekday kTuesday = Weekday::kTuesday;
static constexpr Weekday kWednesday = Weekday::kWednesday;
static constexpr Weekday kThursday = Weekday::kThursday;
static constexpr Weekday kFriday = Weekday::kFriday;
static constexpr Weekday kSaturday = Weekday::kSaturday;
static constexpr Weekday kSunday = Weekday::kSunday;

namespace internal {

static constexpr int kSecondsPerMinute = 60;
static constexpr int kMinutesPerHour = 60;

inline Weekday& week_starts_at() {
    static Weekday weekday = kMonday;
    return weekday;
}

}  // namespace internal

}  // namespace pendulum

#endif  // PENDULUM_CONSTANTS_H_
