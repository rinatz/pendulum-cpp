#ifndef PENDULUM_DURATION_H_
#define PENDULUM_DURATION_H_

#include <chrono>

#include <cctz/civil_time.h>

namespace pendulum {

class Duration {
   public:
    Duration() = default;

    Duration(int years, int months = 0, int days = 0, int hours = 0, int minutes = 0, int seconds = 0) {
        seconds_ = cctz::year_t()
    }

   private:
    std::chrono::seconds seconds_;
};

}  // namespace pendulum

#endif  // PENDULUM_DURATION_H_
