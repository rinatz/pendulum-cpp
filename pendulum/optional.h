#ifndef PENDULUM_OPTIONAL_H_
#define PENDULUM_OPTIONAL_H_

#include "pendulum/internal/optional.hpp"

namespace pendulum {

template <typename T>
using Optional = tl::optional<T>;

static constexpr auto nullopt = tl::nullopt;

}  // namespace pendulum

#endif  // PENDULUM_OPTIONAL_H_
