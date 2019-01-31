#include "strategy/Strategy.h"

#include <mutex>
#include <exception>

#include <frc/Timer.h>

using namespace curtinfrc;

struct StrategyController::Impl {
  using SystemsColl = std::set<StrategySystem *>;

  std::mutex systemsMtx;
  SystemsColl systems;
};

StrategyController::StrategyController() : _impl(new Impl) { }
StrategyController::~StrategyController() {
  delete _impl;
}

void StrategyController::Register(StrategySystem *sys) {
  {
    std::lock_guard<std::mutex> lk(_impl->systemsMtx);
    _impl->systems.insert(sys);
  }
}

bool StrategyController::Run(std::shared_ptr<Strategy> strategy, bool force) {
  {
    // Assert that systems exist, and that they may have an interrupted strategy
    std::lock_guard<std::mutex> lk(_impl->systemsMtx);
    for (StrategySystem *sys : strategy->GetRequirements()) {
      if (std::find(_impl->systems.begin(), _impl->systems.end(), sys) == _impl->systems.end()) {
        throw std::invalid_argument("StrategyController does not have registered system required by " + strategy->GetStrategyName());
      } else {
        auto active = sys->GetActiveStrategy();
        if (active != nullptr) {
          bool can_interrupt = active->_can_interrupt || active == sys->GetDefaultStrategy();
          if (!force && !can_interrupt) {
            // Cannot interrupt, therefore cancelled.
            strategy->Stop(StrategyState::CANCELLED);
            return false;
          }
        }
      }
    }

    // Replace the running strategy on each system
    for (StrategySystem *sys : strategy->GetRequirements()) {
      // sys->_pending = strategy;
      std::lock_guard<wpi::spinlock> lk(sys->_active_mtx);
      if (sys->_active != nullptr) {
        sys->_active->Interrupt();
      }
      sys->_active = strategy;
      sys->_active->Start();
    }
  }
  return true;
}
