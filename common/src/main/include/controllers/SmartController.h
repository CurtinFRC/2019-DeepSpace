#pragma once

// #include <functional>
// #include <utility>
// #include <vector>

#include "controllers/Controller.h"
#include "controllers/Inputs.h"
#include "Toggle.h"

namespace curtinfrc {
  namespace controllers {
    class SmartController : public Controller {
     public:
      SmartController(frc::GenericHID *cont) : Controller(cont) {};
    };
  }
} // ns curtinfrc
