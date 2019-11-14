#include "threading/Runnable.h"

#include <opencv2/highgui/highgui.hpp>

void Runnable::SetUseCVWait(bool useCvWait) {
  _useCvWait = useCvWait;
}

Runnable *Runnable::StartThread(double framerate) {
  _framerate = framerate;
  _thread = std::thread(&Runnable::ThreadFunc, this);
  return this;
}

void Runnable::JoinThread() {
  _thread.join();
}

void Runnable::DetachThread() {
  _thread.detach();
}

void Runnable::ThreadFunc() {
  Init();
  while (true) {
    timepoint_t start = clock_t::now();
    Periodic();
    timepoint_t end = clock_t::now();

    std::chrono::duration<double> duration = end - start;
    _frameperiod = duration.count();
    double remaining = 1.0 / _framerate - (duration.count());
    if (remaining > 0) {
      if (_useCvWait) {
        cv::waitKey((int)(remaining * 1000));
      } else {
        std::this_thread::sleep_for(std::chrono::duration<double>(remaining));
      }
    }
    _framerate_actual = 1.0 / std::chrono::duration<double>(start - _last_frame).count();
    _last_frame = start;
  }
}

double Runnable::GetFramePeriod() {
  return _frameperiod;
}

double Runnable::GetActualFramerate() {
  return _framerate_actual;
}