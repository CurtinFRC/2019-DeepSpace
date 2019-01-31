#pragma once

#include <frc/Notifier.h>
#include <memory>

namespace curtinfrc {

class StrategySystem;
class Strategy;

class StrategyController {
 public:
  StrategyController();
  ~StrategyController();

  void Register(StrategySystem *device);
  bool Run(std::shared_ptr<Strategy> strategy, bool force = false);

  template<typename T, typename ...Args>
  bool EmplaceRun(const Args & ... args) {
    Run(std::make_shared<T>(args...));
  }

 private:
  double _last_notify_time = 0;

  struct Impl;
  Impl *_impl;
};

}