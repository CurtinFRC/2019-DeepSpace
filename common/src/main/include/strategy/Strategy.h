#pragma once

#include "strategy/StrategySystem.h"
#include "strategy/StrategyController.h"

#include <string>
#include <memory>

#include <wpi/SmallSet.h>

namespace curtinfrc {

enum class StrategyState {
  //! Initialized, but never run. The default state.
  INITIALIZED, 
  //! Currently running or active
  RUNNING, 
  //! Strategy was rejected from starting as the current strategy may not be interrupted
  CANCELLED, 
  //! Strategy was voluntarily marked as complete, using SetDone
  DONE, 
  //! Strategy has timed out
  TIMED_OUT, 
  //! Strategy was interrupted by another strategy
  INTERRUPTED
};

/**
 * A 'Strategy' is a single component in a sequential chain of events or actions.
 * 
 * This is similar to commands in WPILib's Command-Based Programming infrastructure,
 * but with a few key changes for our own use.
 * 
 * Strategies are run on a slow loop. It is recommended that the Strategy is only used
 * to orchestrate the system, the system itself should do its control logic on its own,
 * fast loop.
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
   * Called at the same rate as the robot base class. This method is intended to orchestrate
   * the system, while the system itself runs on a faster loop.
   * 
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

  /**
   * Make this strategy dependent on one, or many, systems, giving them
   * exclusive access over this system. A Strategy must be bound to at 
   * least one system in order to function.
   */
  void Requires(StrategySystem *s) {
    if (s != nullptr) {
      _requirements.insert(s);
    }
  }

  /**
   * Check if this strategy requires a given system.
   */
  bool IsRequiring(StrategySystem *s) {
    if (s == nullptr)
      return false;
    return _requirements.find(s) != _requirements.end();
  }

  /**
   * Get a reference to all system requirements.
   */
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
    StrategyState oldState = _strategy_state;
    _strategy_state = newState;
    
    if (oldState == StrategyState::RUNNING)
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