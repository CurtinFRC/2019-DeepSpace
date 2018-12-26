#pragma once

#include "ui/window.h"

#include "mockdata/DriverStationData.h"

#include <csignal>
#include <iostream>

namespace simulation {

class control_window : public ui::window {
 public:
  control_window() : ui::window("Simulation Control", 250, 400) {
    exit.on_activate([&](bool, ui::button &) { do_exit(); });
    register_button(exit);

    ui::colour mode_deactive{0.2, 0.2, 0.2};

    mode_buttons.emplace_back(ui::box{0, 0.4, 1, 0.15}, "DISABLED", mode_deactive, ui::colour{0.5, 0.25, 0.25}, ui::colour{1, 0.5, 0.5});
    mode_buttons.back().on_activate([&](bool active, ui::button &me) {
      toggle_modes(active, me);
      HALSIM_SetDriverStationEnabled(!active);
    });

    mode_buttons.emplace_back(ui::box{0, 0.55, 1, 0.15}, "AUTO", mode_deactive, ui::colour{0.25, 0.25, 0.5}, ui::colour{0.5, 0.5, 1});
    mode_buttons.back().on_activate([&](bool active, ui::button &me) {
      toggle_modes(active, me);
      HALSIM_SetDriverStationAutonomous(active);
    });

    mode_buttons.emplace_back(ui::box{0, 0.7, 1, 0.15}, "TELEOP", mode_deactive, ui::colour{0.25, 0.5, 0.25}, ui::colour{0.5, 1, 0.5});
    mode_buttons.back().on_activate([&](bool active, ui::button &me) {
      toggle_modes(active, me);
    });

    mode_buttons.emplace_back(ui::box{0, 0.85, 1, 0.15}, "TEST", mode_deactive, ui::colour{0.5, 0.25, 0.5}, ui::colour{1, 0.5, 1});
    mode_buttons.back().on_activate([&](bool active, ui::button &me) {
      toggle_modes(active, me);
      HALSIM_SetDriverStationTest(active);
    });

    for (auto it = mode_buttons.begin(); it != mode_buttons.end(); it++) {
      register_button(*it);
    }
  };

  void render(cv::Mat &img) override {
    ui::point{0.5, 0.05}.text(img, "Sim Controls", 1.0, ui::colour::white(), 2);
    ui::point{0.5, 0.35}.text(img, "Mode", 1.0, ui::colour::white(), 2);
  }

  void on_start() override {
    mode_buttons[0].activate();
  }

  void on_stop() override {
    mode_buttons[0].activate();
  }

  void on_closed() override { do_exit(); }

  void do_exit() {
    std::cout << "[SIMCTRL] SIGTERM requested" << std::endl;
    raise(SIGTERM);
  }

  void toggle_modes(bool active, ui::button &me) {
    if (active) {
      for (auto it = mode_buttons.begin(); it != mode_buttons.end(); it++) {
        if (it->id != me.id) it->deactivate();
      }
    }
  }

 private:
  ui::button exit{ui::box{0, 0.1, 1, 0.1}, "Exit", ui::colour{1, 0.75, 0.75}, ui::colour{1, 0.5, 0.5}};

  std::vector<ui::button> mode_buttons;
};

}  // namespace simulation