#pragma once

#include "simulation/ui/window.h"

#include <vector>
#include <memory>

namespace simulation {

class harness {
 public:
  void run(std::function<int()>);
};

}  // namespace simulation