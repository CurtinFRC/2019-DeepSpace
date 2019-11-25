#pragma once

namespace wml {
namespace physics {

  struct DcMotor {
    double v_nom;
    double free_speed;
    double free_current;
    double stall_current;
    double stall_torque;

    inline double R() const {
      return v_nom / stall_current;
    }

    inline double kw() const {
      return (v_nom - free_current * R()) / free_speed;
    }

    inline double kt() const {
      return stall_current / stall_torque;
    }

    double current(double voltage, double angular_vel) const {
      return (voltage - kw() * angular_vel) / R();
    }

    double torque(double current) {
      return current / kt();
    }

    DcMotor reduce(double reduction) const {
      return DcMotor{
        v_nom, 
        free_speed / reduction,
        free_current,
        stall_current,
        stall_torque * reduction
      };
    }

    static double rpm2rads(double rpm) {
      return rpm * 3.14159265 / 30;
    }

    static DcMotor m775pro() {
      return DcMotor{12.0, rpm2rads(18730), 0.7, 134, 0.71};
    }

    static DcMotor mCIM() {
      return DcMotor{12.0, rpm2rads(5330), 2.7, 131, 2.41};
    }
  };

  inline DcMotor operator*(const DcMotor &x, int y) {
    return DcMotor{
      x.v_nom,
      x.free_speed,
      x.free_current * y,
      x.stall_current * y,
      x.stall_torque * y
    };
  }

  inline DcMotor operator*(int x, const DcMotor &y) {
    return y * x;
  }

}  // namespace physics
}  // namespace wml
