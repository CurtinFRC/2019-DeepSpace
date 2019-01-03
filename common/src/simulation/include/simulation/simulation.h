#pragma once

#include "simulation/ui/window.h"

#include <vector>
#include <memory>

namespace simulation {

class harness {
 public:
  harness();
  void run(std::function<int()>);

 private:
  std::vector<std::unique_ptr<ui::window>> _windows;
};

}  // namespace simulation