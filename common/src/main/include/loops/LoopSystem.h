#pragma once

#include <frc/Notifier.h>

namespace curtinfrc {
namespace loops {

  /**
   * A Loop System is any system that runs in a loop. This class allows automatic running
   * in a high-precision notifier thread, or called manually.
   */
  class LoopSystem {
   public:
    /**
     * Called when the Loop System is notified.
     *
     * @param dt The time difference between this call and the last call
     */
    virtual void Update(double dt) = 0;

    /**
     * Call a single update, automatically calculating the difference in time.
     */
    void UpdateOnce();

    /**
     * Start the loop running automatically, with a specific frequency.
     *
     * @param hz The rate of the loop, in Hz. 50-100 are sane defaults.
     */
    void StartLoop(unsigned int hz);

    /**
     * Is this system currently running in a loop?
     */
    bool IsLoopRunning();

   private:
    frc::Notifier _notifier{&LoopSystem::UpdateOnce, this};
    double        _lastNotifierCall;
    bool          _notifierRunning = false;
  };

}  // namespace loop
}  // namespace curtinfrc