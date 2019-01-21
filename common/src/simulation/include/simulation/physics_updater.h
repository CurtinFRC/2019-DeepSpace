#pragma once

#include <thread>
#include <vector>
#include <mutex>

namespace simulation {

class physics_aware;

class physics_thread {
 public:
  static physics_thread *INSTANCE() {
    static physics_thread t;
    return &t;
  }

  std::vector<physics_aware *> listeners;

  void add(physics_aware *phys);
  void remove(physics_aware *phys);
  void threadfunc();

 private:
  std::mutex _mtx;
};

class physics_aware {
 public:
  physics_aware() {
    physics_thread::INSTANCE()->add(this);
  }

  ~physics_aware() {
    physics_thread::INSTANCE()->remove(this);
  }

  virtual void update_physics(double dt) = 0;
};

}  // namespace simulation