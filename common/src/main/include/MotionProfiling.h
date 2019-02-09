#pragma once

#include <frc/SpeedController.h>
#include <pathfinder-frc.h>

#include "sensors/Encoder.h"

namespace curtinfrc {

  // class Pathfinder {
  // public:
  //   /**
  //   * Load a pathfinder trajectory from a .csv file.
  //   */
  //   static int pathfinder_load_file(const char *filename, Segment *segments) {
  //     FILE *fp;
  //     fp = fopen(filename, "rb");
  //     int len = pathfinder_deserialize_csv(fp, segments);
  //     fclose(fp);
  //     return len;
  //   }
  // };

  struct MotionProfileConfig {
    double wheel_diameter, kp, ki, kd, kv, ka;  // note: ki unused in pathfinder, ka unused on Talon internal profile
  };

  /**
   * Motion Profiling Mode.
   * Contains the inner logic of different Motion Profiling Modes available to use on 
   * a given subsystem.
   */
  class MotionProfilingMode {
  public:
    MotionProfilingMode(MotionProfileConfig cfg, const char *file)
      : _cfg(cfg) {
      _seg_length = PathfinderFRC::get_trajectory(file, _segments);
    }

    /**
     * Initialize the Motion Profile. Only called once per follow.
     */
    virtual void init() = 0;

    /**
     * Get the required Control Loop period based on the given path.
     * This is the desired speed to call the calculate() method at.
     */
    virtual double ctrl_period() {
      return _segments[0].dt;
    }

    /**
     * Calculate a single step of the Motion Profile. 
     * \return \c The output to send to the motor controller
     *            or other actuator.
     */
    virtual double calculate() = 0;

    virtual bool gyro_capable() { return false; };
    virtual double gyro_desired() { return 0; };
    bool done;

  protected:
    MotionProfileConfig _cfg;
    Segment _segments[8192];
    int _seg_length;
  };

  class PathfinderMPMode : public MotionProfilingMode {
  public:
    PathfinderMPMode(sensors::Encoder *enc, MotionProfileConfig cfg, const char *file)
      : _enc(enc), MotionProfilingMode(cfg, file) { }

    void init() override;
    double calculate() override;

    bool gyro_capable() override { return true; }
    double gyro_desired() override { 
      return fmod((_follow.heading / PI * 180.0), 360);
    }

  private:
    sensors::Encoder *_enc;

    EncoderFollower _follow;
    EncoderConfig _ecfg;
  };

  // class TalonMPMode : public MotionProfilingMode {
  // public:
  //   TalonMPMode(CurtinTalonSRX *talon, MotionProfileConfig cfg, const char *file)
  //     : _talon(talon), MotionProfilingMode(cfg, file) { }

  //   void init() override;
  //   double ctrl_period() override {
  //     return _segments[0].dt / 2;
  //   }
  //   double calculate() override;

  // private:
  //   CurtinTalonSRX *_talon;
  // };
} // ns curtinfrc
