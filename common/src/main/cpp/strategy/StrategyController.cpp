#include "strategy/Strategy.h"

#include <mutex>
#include <exception>
#include <future>

#include <frc/Timer.h>
#include <wpi/spinlock.h>

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

bool StrategyController::Schedule(std::shared_ptr<Strategy> strategy, bool force) {
  std::unique_lock<std::mutex> lock(_impl->systemsMtx, std::try_to_lock);

  if (lock.owns_lock()) {
    return DoSchedule(strategy, force);
  } else {
    // We're being called when we're already locked - like inside of an active strategy.
    // Make this call asynchronous and wait for the result.
    return std::async(&StrategyController::DoSchedule, this, strategy, force).get();
  }
}

void StrategyController::Update(double dt) {
  if (dt < 0) {
    // Automatic dt finding with system clock
    double t = frc::GetTime();
    if (_last_update_time > 0.01) {
      dt = t - _last_update_time;
    } else {
      dt = 0;
    }
    _last_update_time = t;
  }

  {
    std::lock_guard<std::mutex> lk(_impl->systemsMtx);
    for (StrategySystem *sys : _impl->systems) {
      sys->StrategyUpdate(dt);
    }
  }
}

bool StrategyController::DoSchedule(std::shared_ptr<Strategy> strategy, bool force) {
  // Assert that systems exist, and that they may have an interrupted strategy
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
    sys->StrategyReplace(strategy);
    sys->StrategyStatusUpdate();
  }
  return true;
}