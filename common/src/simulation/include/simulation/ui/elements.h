#pragma once

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>

namespace simulation {
namespace ui {
  struct colour {
    double r, g, b;

    cv::Scalar tocv() { return cv::Scalar{b * 255, g * 255, r * 255}; }

    static colour black() { return colour{0, 0, 0}; }

    static colour white() { return colour{1, 1, 1}; }
  };

  struct point {
    double x, y;

    point(double x, double y) : x(x), y(y) {}

    point(cv::Size size, cv::Point loc) {
      x = (double)loc.x / (double)size.width;
      y = (double)loc.y / (double)size.height;
    }

    cv::Point in(cv::Mat &m) { return cv::Point{(int)(x * m.size().width), (int)(y * m.size().height)}; }

    void text(cv::Mat &m, const char *str, double font_scale, colour col, int thick = 1,
              int font = cv::FONT_HERSHEY_SIMPLEX) {
      int      bline;
      cv::Size size = cv::getTextSize(str, font, font_scale, thick, &bline);

      cv::Point pt{in(m) + cv::Point{-size.width / 2, size.height / 2}};
      cv::putText(m, str, pt, font, font_scale, col.tocv(), thick);
    }
  };

  struct box {
    double x, y, w, h;

    point centre() { return point{x + w / 2, y + h / 2}; }

    bool contains(point p) { return p.x >= x && p.x <= x + w && p.y >= y && p.y <= y + h; }

    cv::Rect in(cv::Mat &m) { return cv::Rect{point{x, y}.in(m), point{x + w, y + h}.in(m)}; }

    void fill(cv::Mat img, colour col) { cv::rectangle(img, in(img), col.tocv(), -1); }
  };

  struct button {
    box         b;
    std::string text;
    colour      colour_normal;
    colour      colour_hover;
    colour      colour_active;
    int id;

    button(box bounds, std::string txt, colour c_normal, colour c_hover_activate)
        : button(bounds, txt, c_normal, c_hover_activate, c_hover_activate) {}

    button(box bounds, std::string txt, colour c_normal, colour c_hover, colour c_active)
        : b(bounds), text(txt), colour_normal(c_normal), colour_hover(c_hover), colour_active(c_active) {
          static int _id = 0;
          _id++;
          id = _id;
        }

    void on_click(std::function<void(int, button &)> cb) { click_callback = cb; }

    void on_activate(std::function<void(bool, button &)> cb) { activate_callback = cb; }

    void handle_mouse(int event, point pt) {
      _hover = b.contains(pt);

      if (_hover && event == cv::EVENT_LBUTTONUP) {
        if (event == cv::EVENT_LBUTTONUP || event == cv::EVENT_RBUTTONUP) {
          activate();
          if (click_callback != nullptr) click_callback(event, *this);
        }
      }
    }

    void activate() {
      _active = true;
      if (activate_callback != nullptr) {
        activate_callback(true, *this);
      }
    }

    void deactivate() {
      _active = false;
      if (activate_callback != nullptr) {
        activate_callback(false, *this);
      }
    }

    bool active() { return _active; }

    void render(cv::Mat &img) {
      b.fill(img, _active ? colour_active : _hover ? colour_hover : colour_normal);
      b.centre().text(img, text.c_str(), 0.75, colour::black(), 2);
    }

   private:
    bool _hover  = false;
    bool _active = false;

    std::function<void(int, button &)>  click_callback;
    std::function<void(bool, button &)> activate_callback;
  };
}  // namespace ui
}  // namespace simulation