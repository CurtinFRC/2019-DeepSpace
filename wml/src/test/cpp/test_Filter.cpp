#include "gtest/gtest.h"

#include "control/Filter.h"

#include <cmath>
#include <fstream>

using namespace curtinfrc::control;

using signal_t = std::function<double(double)>;

static signal_t signal_low = [](double t) {
  return std::sin(2 * 3.1415 * 3 * t);      // 3 Hz
};

static signal_t signal_high = [](double t) {
  return std::sin(2 * 3.1415 * 100 * t);    // 100 Hz
};

static double tc_low = 1.0 / (2 * 3.1415 * 5);  // 5 Hz cutoff
static double tc_high = 1.0 / (2 * 3.1415 * 95);  // 95 Hz cutoff

static signal_t combined_signal = [](double t) {
  return signal_low(t) + signal_high(t);
};

static double dt = 0.001;

// TODO: FFT tests

TEST(Filter, LowPass) {
  LinearFilter f = LinearFilter::LowPass(tc_low, dt);
  
  std::ofstream outstream{"lowpass"};

  for (double t = 0; t < 1.0; t += dt) {
    double val = f.Get(combined_signal(t));
    outstream << val << std::endl;
  }

  outstream.close();
}

TEST(Filter, HighPass) {
  LinearFilter f = LinearFilter::HighPass(tc_high, dt);
  
  std::ofstream outstream{"highpass"};

  for (double t = 0; t < 1.0; t += dt) {
    double val = f.Get(combined_signal(t));
    outstream << val << std::endl;
  }

  outstream.close();
}

TEST(Filter, MovingAvg) {
  LinearFilter f = LinearFilter::MovingAverage(100);
  
  std::ofstream outstream{"movingavg"};

  for (double t = 0; t < 1.0; t += dt) {
    double val = f.Get(combined_signal(t));
    outstream << val << std::endl;
  }

  outstream.close();
}