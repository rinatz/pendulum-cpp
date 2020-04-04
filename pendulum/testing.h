#ifndef PENDULUM_TESTING_H_
#define PENDULUM_TESTING_H_

#include <functional>
#include <memory>

#include "pendulum/datetime.h"

namespace pendulum {

namespace internal {

inline std::unique_ptr<DateTime>& mutable_test_now() {
    static std::unique_ptr<DateTime> test_now;
    return test_now;
}

}  // namespace internal

inline void set_test_now(const DateTime& now) {
    internal::mutable_test_now().reset(new DateTime(now));
}

inline void set_test_now() { internal::mutable_test_now().reset(); }

inline void test(const DateTime& now, std::function<void()> func) {
    set_test_now(now);
    func();
    set_test_now();
}

}  // namespace pendulum

#endif  // PENDULUM_TESTING_H_
