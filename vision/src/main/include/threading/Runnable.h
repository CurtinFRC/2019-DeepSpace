#pragma once

#include <thread>
#include <chrono>

class Runnable {
 public:
  using clock_t = std::chrono::steady_clock; 
  using timepoint_t = clock_t::time_point;

  virtual void Init() = 0;
  virtual void Periodic() = 0;

  /**
   * Use cv::waitKey, if this has a GUI
   */
  void SetUseCVWait(bool useCvWait);

  Runnable *StartThread(double framerate);
  void JoinThread();
  void DetachThread();

  double GetFramePeriod();
  double GetActualFramerate();

 private:
  void ThreadFunc();

  std::thread _thread;
  bool _useCvWait = false;

  double _framerate;
  double _frameperiod;
  double _framerate_actual;
  timepoint_t _last_frame;
};