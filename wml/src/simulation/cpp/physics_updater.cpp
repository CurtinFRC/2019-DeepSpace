#include "simulation/physics_updater.h"

#include <frc/Timer.h>
#include <algorithm>

using namespace simulation;

void physics_thread::add(physics_aware *phys) {
  std::lock_guard<std::mutex> g(_mtx);
  listeners.push_back(phys);
}

void physics_thread::remove(physics_aware *phys) {
  std::lock_guard<std::mutex> g(_mtx);
  listeners.erase(std::remove(listeners.begin(), listeners.end(), phys), listeners.end());
}

void physics_thread::threadfunc() {
  while (true) {
    _mtx.lock();
    std::for_each(listeners.begin(), listeners.end(), [&](physics_aware *phys) {
      phys->update_physics(0.002);
    });
    _mtx.unlock();
    frc::Wait(0.002);
  }
}