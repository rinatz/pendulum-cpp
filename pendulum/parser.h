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

#ifndef PENDULUM_PARSER_H_
#define PENDULUM_PARSER_H_

#include <algorithm>
#include <cctype>
#include <chrono>
#include <cstdio>
#include <ctime>
#include <string>

#include <cctz/time_zone.h>

#include "pendulum/datetime.h"
#include "pendulum/exceptions.h"
#include "pendulum/helpers.h"

namespace pendulum {

namespace internal {

inline bool is_digit(const std::string& input) {
    return std::all_of(input.begin(), input.end(), [](unsigned char c) { return std::isdigit(c); });
}

inline DateTime from_yyyymmdd(const std::string& input, const std::string& tz = "UTC") {
    if (input.size() != 8) {
        throw UnsupportedFormat("input: " + input + " - format: %Y%m%d");
    }

    if (!internal::is_digit(input)) {
        throw UnsupportedFormat("input: " + input + " - format: %Y%m%d");
    }

    int year = 0, month = 0, day = 0;
    auto count = std::sscanf(input.c_str(), "%04d%02d%02d", &year, &month, &day);

    if (count != 3 || count == EOF) {
        throw UnsupportedFormat("input: " + input + " - format: %Y%m%d");
    }

    return DateTime(year, month, day, tz);
}

}  // namespace internal

inline DateTime from_format(const std::string& input, const std::string& fmt,
                            const std::string& tz = "UTC") {
    if (fmt == "%Y%m%d") {
        // cctz cannot parse '%Y%m%d'
        return internal::from_yyyymmdd(input, tz);
    }

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
            "%Y-%m-%dT%H:%M:%S%Ez",
            "%Y-%m-%dT%H:%M:%S%E",
            "%Y-%m-%d %H:%M:%S",
            "%Y-%m-%d",
            "%Y-%m",
            "%Y%m%d",
            "%Y",
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
