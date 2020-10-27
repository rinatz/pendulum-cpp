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

#ifndef PENDULUM_TESTING_H_
#define PENDULUM_TESTING_H_

#include <exception>
#include <functional>

#include "pendulum/datetime.h"
#include "pendulum/internal.h"

namespace pendulum {

namespace internal {

inline optional<DateTime>& test_now() {
    static optional<DateTime> now;
    return now;
}

}  // namespace internal

inline bool has_test_now() { return internal::test_now().has_value(); }
inline DateTime get_test_now() { return internal::test_now().value(); }
inline void set_test_now(const DateTime& now) { internal::test_now() = now; }
inline void set_test_now() { internal::test_now() = internal::nullopt; }

inline void test(const DateTime& now, std::function<void()> func) {
    auto act = internal::finally([] { set_test_now(); });

    set_test_now(now);
    func();
}

}  // namespace pendulum

#endif  // PENDULUM_TESTING_H_
