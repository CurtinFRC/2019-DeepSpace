#include "strategy/Strategy.h"

#include <exception>
#include <iostream>

using namespace wml;

void StrategySystem::SetDefault(std::shared_ptr<Strategy> newStrategy) {
  if (newStrategy != nullptr) {
    if (!newStrategy->IsRequiring(this) || newStrategy->GetRequirements().size() > 1) {
      throw std::invalid_argument("A default strategy must be requiring the system as its one and only requirement.");
    }

    if (!newStrategy->_can_interrupt) {
      throw std::invalid_argument("A default strategy must be interruptable");
    }
  }
  
  _default = newStrategy;
}

void StrategySystem::StrategyUpdate(double dt) {
  StrategyStatusUpdate();
  // Run this strategy
  if (_active != nullptr && !_active->IsFinished()) {
    _active->Update(dt);
  }
  StrategyStatusUpdate();
}

void StrategySystem::StrategyStatusUpdate() {
  // Employ the default strategy if this one is complete
  if (_active == nullptr || _active->IsFinished()) {
    _active = _default;
    if (_active != nullptr) {
      std::cout << "Default Strategy Loaded: " << _active->GetStrategyName() << std::endl;
      _active->Start();
    }
  }
}

void StrategySystem::StrategyReplace(std::shared_ptr<Strategy> newStrat) {
  if (_active != nullptr) {
    _active->Interrupt();
  }
  _active = newStrat;
  if (newStrat != nullptr) {
    std::cout << "Strategy Loaded: " << newStrat->GetStrategyName() << std::endl;
    _active->Start();
  }
}