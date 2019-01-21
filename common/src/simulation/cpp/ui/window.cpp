#include "simulation/ui/window.h"

#include <iostream>

using namespace simulation::ui;

static void mouse_callback(int event, int x, int y, int flags, void *data) {
  window *ptr = static_cast<window *>(data);
  point   pt  = point{ptr->size(), cv::Point{x, y}};

  if (pt.x <= 1 && pt.x >= 0 && pt.y <= 1 && pt.y >= 0) ptr->update_mouse(event, pt);
}

window::window(std::string name, int width, int height) : _window_name(name), _image(height, width, CV_8UC3) {
  window_manager::INSTANCE()->add(this);
}

window::~window() {
  window_manager::INSTANCE()->remove(this);
  if (_running) {
    stop();
    update();
  }
}

void window::start() {
  if (!_running) {
    std::cout << "[SIM] Starting " << _window_name << std::endl;
    _running       = true;
    _pending_state = true;
  }
}

void window::stop() {
  if (_running) {
    std::cout << "[SIM] Stopping " << _window_name << std::endl;
    _running       = false;
    _pending_state = true;
  }
}

cv::Size window::size() {
  return _window_size;
}

bool window::is_running() const {
  return _running;
}

std::string window::window_name() const {
  return _window_name;
}

double window::dt() const {
  return _time_elapsed;
}

void window::update() {
  bool window_exists = cvGetWindowHandle(_window_name.c_str()) != nullptr;

  if (_pending_state) {
    if (_running) {
      // Start window
      cv::namedWindow(_window_name.c_str(), cv::WINDOW_NORMAL | cv::WINDOW_KEEPRATIO);
      _window_size = _image.size();
      cv::setMouseCallback(_window_name, mouse_callback, (void *)this);
      on_start();
    } else {
      // Stop window
      if (window_exists) {
        on_stop();
        cv::destroyWindow(_window_name.c_str());
      }
    }
    _pending_state = false;
    return;  // Move to the next iteration.
  }

  if (_running) {
    if (!window_exists) {
      // Window doesn't already exist, user manually closed it!
      stop();
      on_closed();
    } else {
      cv::namedWindow(_window_name.c_str());
      auto now = clock::now();
      _time_elapsed =
          _time_elapsed == 0
              ? 1.0 / 45.0
              : std::chrono::duration_cast<std::chrono::duration<double>>(now - _time_last).count();

      _image = cv::Mat::zeros(_window_size, _image.type());

      render(_image);

      for (auto it : _buttons) {
        it.get().render(_image);
      }

      cv::imshow(_window_name.c_str(), _image);

      _time_last = now;
    }
  }
}

void window::update_mouse(int event, point pt) {
  on_mouse(event, pt);
  for (auto it : _buttons) {
    it.get().handle_mouse(event, pt);
  }
}

void window::register_button(button &btn) {
  _buttons.emplace_back(btn);
}