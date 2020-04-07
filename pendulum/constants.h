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

#ifndef PENDULUM_CONSTANTS_H_
#define PENDULUM_CONSTANTS_H_

#include <cctz/civil_time.h>

namespace pendulum {

static constexpr int kMonday = static_cast<int>(cctz::weekday::monday);
static constexpr int kTuesday = static_cast<int>(cctz::weekday::tuesday);
static constexpr int kWednesday = static_cast<int>(cctz::weekday::wednesday);
static constexpr int kThursday = static_cast<int>(cctz::weekday::thursday);
static constexpr int kFriday = static_cast<int>(cctz::weekday::friday);
static constexpr int kSaturday = static_cast<int>(cctz::weekday::saturday);
static constexpr int kSunday = static_cast<int>(cctz::weekday::sunday);

namespace internal {

static constexpr int kSecondsPerMinute = 60;
static constexpr int kMinutesPerHour = 60;

inline int& week_starts_at() {
    static int week_day = kMonday;
    return week_day;
}

}  // namespace internal

}  // namespace pendulum

#endif  // PENDULUM_CONSTANTS_H_
