#pragma once

#include "strategy/StrategySystem.h"
#include "strategy/StrategyController.h"

#include <string>
#include <memory>

#include <wpi/SmallSet.h>

namespace curtinfrc {

enum class StrategyState {
  INITIALIZED, RUNNING, CANCELLED, DONE, TIMED_OUT, INTERRUPTED
};

/**
 * A 'Strategy' is a single component in a sequential chain of events or actions.
 * 
 * This is similar to commands in WPILib's Command-Based Programming infrastructure,
 * but with a few key changes for our own use.
 */
class Strategy {
 public:
  /**
   * Get the name of this Strategy
   */
  virtual std::string GetStrategyName() = 0;

  /**
   * Called upon the Strategy being started
   */
  virtual void OnStart() {}

  /**
   * Called at the same rate as the robot base class
   * @param dt The delta time (in seconds) from the last iteration
   */
  virtual void OnUpdate(double dt) = 0;

  /**
   * Called upon the Strategy being put into a state of DONE, or CANCELLED
   */
  virtual void OnStop() {}

  /**
   * Set whether this Strategy can be interrupted by other Strategies
   */
  void SetCanBeInterrupted(bool canBeInterrupted) {
    _can_interrupt = canBeInterrupted;
  }

  /**
   * Attempt to interrupt this Strategy
   * 
   * @return true if it can be interrupted, false otherwise
   */
  bool Interrupt() {
    if (_can_interrupt) {
      Stop(StrategyState::INTERRUPTED);
      return true;
    }
    return false;
  }

  /**
   * Set this Strategy as Done
   */
  void SetDone() {
    Stop(StrategyState::DONE);
  }

  /**
   * Set the Timeout for this Strategy, in seconds.
   * 
   * @param seconds The timeout, in seconds.
   */
  void SetTimeout(double seconds) { _timeout = seconds * 1000; }

  /**
   * Get the Timeout of the Strategy, in seconds, or 0 if it does not
   * have an associated timeout.
   */
  double GetTimeout() {
    return _timeout;
  }

  /**
   * Get the time, in seconds, since the Strategy was started.
   */
  double TimeSinceStarted() {
    return _time;
  }

  /**
   * Get the current state of the Strategy
   */
  StrategyState GetStrategyState() {
    return _strategy_state;
  }

  /**
   * Return true if the state is in any finished state.
   */
  bool IsFinished() {
    return _strategy_state != StrategyState::INITIALIZED && _strategy_state != StrategyState::RUNNING;
  }

  void Requires(StrategySystem *s) {
    if (s != nullptr) {
      _requirements.insert(s);
    }
  }

  bool IsRequiring(StrategySystem *s) {
    if (s == nullptr)
      return false;
    return _requirements.find(s) != _requirements.end();
  }

  int NumRequirements() {
    return _requirements.size();
  }

  wpi::SmallPtrSetImpl<StrategySystem *> &GetRequirements() {
    return _requirements;
  }

 protected:
  void Start() {
    if (_strategy_state != StrategyState::RUNNING) {
      _time = 0;
      _strategy_state = StrategyState::RUNNING;
      OnStart();
    }
  }

  void Update(double dt) {
    if (_strategy_state != StrategyState::RUNNING) {
      Start();
    }

    if (_strategy_state == StrategyState::RUNNING) {
      OnUpdate(dt);
      _time += dt;
    }

    if (_timeout > 0 && _time > _timeout) {
      Stop(StrategyState::TIMED_OUT);
    }
  }

  void Stop(StrategyState newState) {
    _strategy_state = newState;
    OnStop();
  }

  friend class StrategySystem;
  friend class StrategyController;

 private:
  double _time = 0;
  double _timeout = 0;
  bool _can_interrupt = false;
  StrategyState _strategy_state = StrategyState::INITIALIZED;

  wpi::SmallSet<StrategySystem *, 8> _requirements;
};

}  // namespace curtinfrc