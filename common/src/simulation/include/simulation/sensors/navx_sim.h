#include "sensors/NavX.h"

#include <array>

namespace curtinfrc {
namespace sensors {
  struct NavX::Impl {
    std::array<double, 3> angles{ {0, 0, 0} };
    std::array<double, 3> angular_rates{ {0, 0, 0} };
  };
}  // namespace sensors
}  // namespace curtinfrc