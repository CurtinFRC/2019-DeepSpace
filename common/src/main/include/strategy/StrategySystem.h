#pragma once

#include <memory>
#include <wpi/spinlock.h>

namespace curtinfrc {

class Strategy;
class StrategyController;

class StrategySystem {
 public:
  void SetDefault(std::shared_ptr<Strategy> newStrategy);

  template<typename T, typename ...Args>
  void EmplaceDefault(const Args & ... args) {
    SetDefault(std::make_shared<T>(args...));
  }

  std::shared_ptr<Strategy> GetActiveStrategy() {
    return _active;
  }

  std::shared_ptr<Strategy> GetDefaultStrategy() {
    return _default;
  }

  void StrategyUpdate(double dt);

 private:
  wpi::spinlock _active_mtx;
  std::shared_ptr<Strategy> _active = nullptr;
  std::shared_ptr<Strategy> _default = nullptr;

  friend class StrategyController;
};

}