#include "simulation/ui/window.h"

#include <iostream>

using namespace simulation::ui;

window_manager *window_manager::INSTANCE() {
  static window_manager manager;
  return &manager;
}

void window_manager::update() {
  std::lock_guard<std::mutex> _lock{_mtx};
  std::for_each(_windows.begin(), _windows.end(), [](ui::window *window) {
    window->update();
  });
}

void window_manager::respawn() {
  std::lock_guard<std::mutex> _lock{_mtx};
  std::for_each(_windows.begin(), _windows.end(), [](ui::window *window) {
    if (!window->is_running())
      window->start();
  });
}

void window_manager::add(window *window) {
  std::lock_guard<std::mutex> _lock{_mtx};
  std::cout << "[SIM] [WM] Adding window: " << window->window_name() << std::endl;
  _windows.push_back(window);
}

void window_manager::remove(window *window) {
  std::lock_guard<std::mutex> _lock{_mtx};
  std::cout << "[SIM] [WM] Removing window: " << window->window_name() << std::endl;
  _windows.erase(std::remove(_windows.begin(), _windows.end(), window), _windows.end());
}