#include "control/Filter.h"

using namespace wml::control;

LinearFilter::LinearFilter(wpi::ArrayRef<double> gainsFF, wpi::ArrayRef<double> gainsFB) :
  _bufferFF(gainsFF.size()), _bufferFB(gainsFB.size()), _gainsFF(gainsFF), _gainsFB(gainsFB) {}

double LinearFilter::Get(double input) {
  double ret = 0;

  _bufferFF.push_back(input);

  for (size_t i = 0; i < _gainsFF.size(); i++)
    ret += _bufferFF[i] * _gainsFF[i];
  for (size_t i = 0; i < _gainsFB.size(); i++)
    ret -= _bufferFB[i] * _gainsFB[i];

  _bufferFB.push_back(ret);

  return ret;
}

void LinearFilter::Reset() {
  _bufferFF.reset();
  _bufferFB.reset();
}

LinearFilter LinearFilter::LowPass(double time_const, double period) {
  double gain = std::exp(-period / time_const);
  return LinearFilter({1.0 - gain}, {-gain});
}

LinearFilter LinearFilter::HighPass(double time_const, double period) {
  double gain = std::exp(-period / time_const);
  return LinearFilter({gain, -gain}, {-gain});
}

LinearFilter LinearFilter::MovingAverage(int samples) {
  std::vector<double> gains(samples, 1.0 / samples);
  return LinearFilter(gains, {});
}