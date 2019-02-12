#include <gtest/gtest.h>

#include "control/MotorFilters.h"

using namespace curtinfrc;

class MockEncoder : public sensors::Encoder {
 public:
  MockEncoder() : Encoder(2048) { }

  int GetEncoderTicks() override {
    return 0;
  }

  double GetEncoderTickVelocity() override {
    return tick_vel;
  }

  void SetVelocity(double radps) {
    tick_vel = radps / 6.28 * 2048;
  }
 private:
  double tick_vel = 0;
};  

TEST(AccelerationFilter, DoesLimit) {
  auto motor = physics::DcMotor::mCIM();
  MockEncoder encoder{};

  Gearbox gb{ nullptr, &encoder, 1, motor };

  // m = 1, r = 1, therefore torque = acceleration
  // Limits: -1 Nm (@ 3090 RPM), 1.5 Nm (@ 2025 RPM)
  control::AccelerationFFFilter filter{-1.0, 1.5, gb, 1, 1};

  double speed_min = -3090 / 60.0 * 2 * 3.1415;
  double speed_max = 2025 / 60.0 * 2 * 3.1415;

  // We are still, so all voltage is being used to accelerate.
  encoder.SetVelocity(0);
  EXPECT_LT(filter.Get(12.0), 12.0);
  
  // We are moving, so most of our voltage is being used to hold velocity.
  encoder.SetVelocity(speed_max * 1.1);
  EXPECT_DOUBLE_EQ(filter.Get(12.0), 12.0);

  encoder.SetVelocity(0);
  EXPECT_GT(filter.Get(-12.0), -12.0);

  encoder.SetVelocity(speed_min * 1.1);
  EXPECT_DOUBLE_EQ(filter.Get(-12.0), -12.0);
}