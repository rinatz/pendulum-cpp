// MIT License
//
// Copyright (c) 2020 IDA Kenichiro
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

#ifndef PENDULUM_PARSER_H_
#define PENDULUM_PARSER_H_

#include <chrono>
#include <string>

#include <cctz/time_zone.h>

#include "pendulum/datetime.h"
#include "pendulum/exceptions.h"
#include "pendulum/helpers.h"

namespace pendulum {

inline DateTime from_format(const std::string& input, const std::string& fmt,
                            const std::string& tz = "UTC") {
    const auto& tz_ = internal::timezone(tz);
    cctz::time_point<std::chrono::seconds> tp;

    auto ok = cctz::parse(fmt, input, tz_, &tp);

    if (!ok) {
        throw UnsupportedFormat("input: " + input + " - format: " + fmt);
    }

    const auto& cs = cctz::convert(tp, tz_);

    return DateTime(cs, tz_);
}

inline DateTime parse(const std::string& input, const std::string& tz = "UTC") {
    static const std::string formats[] = {
            // ISO 8601 extended format
            "%E4Y-%m-%dT%H:%M:%S%Ez",
            "%E4Y-%m-%dT%H:%M:%SZ",
            // ISO 8601 datetime format
            "%E4Y%m%dT%H%M%S%z",
            "%E4Y-%m-%d %H:%M:%S",
            // ISO 8601 date format
            "%E4Y-%m-%d",
            "%E4Y-%m",
            "%E4Y%m%d",
            "%E4Y",
    };

    if (input == "now") {
        return now(tz);
    }

    for (const auto& fmt : formats) {
        try {
            return from_format(input, fmt, tz);
        } catch (const UnsupportedFormat&) {
            continue;
        }
    }

    throw UnsupportedFormat("input: " + input);
}

}  // namespace pendulum

#endif  // PENDULUM_PARSER_H_
