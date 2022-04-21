// MIT License
//
// Copyright (c) 2022 IDA Kenichiro
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#ifndef PENDULUM_TIMEZONE_H_
#define PENDULUM_TIMEZONE_H_

#include <chrono>
#include <string>

#include <cctz/time_zone.h>

#include "pendulum/exceptions.h"

namespace pendulum {
namespace internal {

inline cctz::time_zone timezone(const std::string& name) {
    if (name == "UTC") {
        return cctz::utc_time_zone();
    }

    if (name == "local") {
        return cctz::local_time_zone();
    }

    cctz::time_zone tz;
    auto ok = cctz::load_time_zone(name, &tz);

    if (!ok) {
        throw InvalidTimezone("tz: " + name);
    }

    return tz;
}

inline cctz::time_zone timezone(std::chrono::seconds offset) {
    return cctz::fixed_time_zone(offset);
}

}  // namespace internal
}  // namespace pendulum

#endif  // PENDULUM_TIMEZONE_H_
