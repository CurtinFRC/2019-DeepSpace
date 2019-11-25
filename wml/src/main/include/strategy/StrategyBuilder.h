#pragma once

#include "strategy/Strategy.h"
#include <memory>
#include <deque>
#include <wpi/SmallVector.h>

namespace wml {

class StrategyQueue;

/**
 * The StrategyBuilder is a builder for a StrategyQueue - allowing for a combination of sequential
 * and parallel actions. 
 * 
 * The builder operates in a queue-like manner. Each build starts with a call to Start() and is completed
 * with a call to Build(). In between, Add() may be used to add a parallel action, and Then() may be used
 * to move on to the next sequential action. 
 */
class StrategyBuilder {
 public:
  
  /**
   * Start building a sequence of Strategies.
   */
  StrategyBuilder *Start();

  /**
   * Add a parallel action to the current action
   * 
   * Parallel actions may not share the same system, they must be mutually exclusive.
   */
  StrategyBuilder *Add(std::shared_ptr<Strategy> strat);

  /**
   * Add a parallel action to the current action
   * 
   * Parallel actions may not share the same system, they must be mutually exclusive.
   */
  template<typename T, typename ...Args>
  StrategyBuilder *Add(const Args ... args) {
    return Add(std::make_shared<T>(args...));
  }

  /**
   * Move onto the next sequential action.
   */
  StrategyBuilder *Then();

  /**
   * Build the final StrategyQueue. A name may be given as an optional argument.
   * 
   * @param name The name for the Strategy. Optional.
   */
  std::shared_ptr<StrategyQueue> Build(std::string name = "");

 private:
  std::shared_ptr<StrategyQueue> _q;
};

/**
 * A StrategyQueue is a sequential queue of parallel strategies - allowing for a combination
 * of sequential and parallel actions.
 * 
 * Parallel actions may not use the same system / requirements, but sequential actions may.
 * 
 * The StrategyQueue should not be created directly, but should be made with @ref StrategyBuilder.
 */
class StrategyQueue : public Strategy {
 public:
  using node_parallel_container_t = typename wpi::SmallVector<std::shared_ptr<Strategy>, 4>;
  using node_queue_t = typename std::deque<node_parallel_container_t>;

  StrategyQueue();

  std::string GetStrategyName() override;

  void OnStart() override;
  void OnUpdate(double dt) override;
  void OnStop() override;
 
  node_queue_t::const_iterator begin();
  node_queue_t::const_iterator end();

 protected:
  void InheritRequirements(std::shared_ptr<Strategy> strat);

  std::string _custom_name = "";
  node_queue_t _queue;
  friend class StrategyBuilder;
};

}
