#ifndef PENDULUM_TESTING_H_
#define PENDULUM_TESTING_H_

#include <functional>

#include "pendulum/datetime.h"
#include "pendulum/optional.h"

namespace pendulum {

namespace internal {

inline Optional<DateTime>& test_now() {
    static Optional<DateTime> now;
    return now;
}

}  // namespace internal

inline bool has_test_now() { return internal::test_now().has_value(); }
inline DateTime get_test_now() { return internal::test_now().value(); }
inline void set_test_now(const DateTime& now) { internal::test_now() = now; }
inline void set_test_now() { internal::test_now() = nullopt; }

inline void test(const DateTime& now, std::function<void()> func) {
    set_test_now(now);
    func();
    set_test_now();
}

}  // namespace pendulum

#endif  // PENDULUM_TESTING_H_
