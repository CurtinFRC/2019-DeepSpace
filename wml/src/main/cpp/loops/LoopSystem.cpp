#include "loops/LoopSystem.h"

#include <frc/Timer.h>

using namespace wml::loops;

void LoopSystem::UpdateOnce() {
  double dt = frc::Timer::GetFPGATimestamp() - _lastNotifierCall;
  Update(dt);
  _lastNotifierCall = frc::Timer::GetFPGATimestamp();
}

void LoopSystem::StartLoop(unsigned int hz) {
  _lastNotifierCall = frc::Timer::GetFPGATimestamp();
  _notifier.StartPeriodic(1.0 / hz);
  _notifierRunning = true;
}

bool LoopSystem::IsLoopRunning() {
  return _notifierRunning;
}