#ifndef PENDULUM_TESTING_H_
#define PENDULUM_TESTING_H_

#include <functional>
#include <memory>

#include "pendulum/datetime.h"

namespace pendulum {

namespace internal {

inline std::unique_ptr<DateTime>& test_now() {
    static std::unique_ptr<DateTime> dt;
    return dt;
}

}  // namespace internal

inline bool has_test_now() { return internal::test_now() != nullptr; }
inline DateTime get_test_now() { return *internal::test_now(); }
inline void set_test_now(const DateTime& now) { internal::test_now().reset(new DateTime(now)); }
inline void set_test_now() { internal::test_now().reset(); }

inline void test(const DateTime& now, std::function<void()> func) {
    set_test_now(now);
    func();
    set_test_now();
}

}  // namespace pendulum

#endif  // PENDULUM_TESTING_H_
