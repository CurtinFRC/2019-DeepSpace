#pragma once

#include <frc/Notifier.h>
#include <memory>

namespace wml {

class StrategySystem;
class Strategy;

/**
 * The StrategyController is the orchestrator for everything to do with Strategies.
 * 
 * The StrategyController is used to start Strategies, as well as orchestrating the
 * StrategySystems and dependencies.
 * 
 * This class should be used to start Strategies, and all StrategySystems should be 
 * registered with this class. There should only be one StrategyController per Robot.
 * 
 * The StrategyController Update method should be called from RobotPeriodic.
 */
class StrategyController {
 public:
  StrategyController();
  ~StrategyController();

  /**
   * Register a StrategySystem with the controller. This must be called for the
   * StrategySystem to be used with any Strategies ran by this class.
   */
  void Register(StrategySystem *device);

  /**
   * Schedule a Strategy to be ran. 
   * 
   * For each system the Strategy requires, it will be checked whether the Strategy
   * is able to override the one currently running. If it can, the Strategy will be
   * made active and the old one interrupted. If not, this function will return false
   * and the Strategy will be marked as CANCELLED. The system can be forced into being
   * interrupted with the force param.
   * 
   * @param strategy  The new strategy to schedule
   * @param force     Should this strategy be forced? If so, all interrupt checking is
   *                  bypassed. Use this with caution.
   * @return          True if the new strategy was successfully applied to all systems, 
   *                  false otherwise.
   */
  bool Schedule(std::shared_ptr<Strategy> strategy, bool force = false);

  /**
   * Call a single update the Strategy system. Call this from RobotPeriodic.
   * 
   * This updates the Strategy status for all systems, including scheduling. This is run
   * on a slow loop.
   */
  void Update(double dt = -1);


  /**
   * Interrupt all Strategy Systems that are registered, stopping the current strategy
   * and reverting to the default.
   * 
   * @param force Should the interrupt be forced? Usually yes.
   */
  void InterruptAll(bool force);

 private:
  bool DoSchedule(std::shared_ptr<Strategy> strategy, bool force = false);
  double _last_update_time = 0;

  struct Impl;
  Impl *_impl;
};

}