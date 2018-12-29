#pragma once

#include "elements.h"

#include <chrono>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <string>

namespace simulation {
namespace ui {

  class window {
   public:
    using clock = std::chrono::high_resolution_clock;

    window(std::string name, int width, int height) : _window_name(name), _image(height, width, CV_8UC3) { }

    ~window();

    void start();

    void stop();

    virtual void render(cv::Mat &img){};
    virtual void on_mouse(int event, point pt){};
    virtual void on_start(){};
    virtual void on_stop(){};
    virtual void on_closed(){};

    cv::Size size();

    bool is_running() const;

    std::string window_name() const;

    double dt() const;

    void update();

    void update_mouse(int event, point pt);

    void register_button(button &btn);
   private:
    std::string _window_name;
    cv::Mat     _image;
    cv::Size    _window_size;
    bool        _running       = false;
    bool        _pending_state = false;  // All actions must happen in main thread.

    double                         _time_elapsed = 0;
    std::chrono::time_point<clock> _time_last;

    std::vector<std::reference_wrapper<button>> _buttons;
  };

}  // namespace ui
}  // namespace simulation