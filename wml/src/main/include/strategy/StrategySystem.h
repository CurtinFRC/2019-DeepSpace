#pragma once

#include <memory>

namespace curtinfrc {

class Strategy;
class StrategyController;

/**
 * A StrategySystem is any system, plant, or device that is capable of running
 * a Strategy.
 * 
 * A StrategySystem may only run one Strategy at once, but it may have a default
 * Strategy that is run whenever the system is not taken by another.
 * 
 * A StrategySystem is run at its own loop frequency. 
 */
class StrategySystem {
 public:
  StrategySystem() {};
  StrategySystem(std::shared_ptr<Strategy> defaultStrategy) { SetDefault(defaultStrategy); };

  /**
   * Set the default Strategy to run. This strategy will run whenever the system
   * is left at idle.
   */
  void SetDefault(std::shared_ptr<Strategy> newStrategy);

  /**
   * Get the currently active strategy.
   */
  std::shared_ptr<Strategy> GetActiveStrategy() {
    return _active;
  }

  /**
   * Get the default strategy.
   */
  std::shared_ptr<Strategy> GetDefaultStrategy() {
    return _default;
  }

 protected:
  void StrategyUpdate(double dt);
  void StrategyStatusUpdate();
  void StrategyReplace(std::shared_ptr<Strategy> newStrat);

 private:
  std::shared_ptr<Strategy> _active = nullptr;
  std::shared_ptr<Strategy> _default = nullptr;

  friend class StrategyController;
};

}