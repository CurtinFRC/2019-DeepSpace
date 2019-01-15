#include "gtest/gtest.h"

#include "CurtinControllers.h"

#define EPS 0.00001

namespace testing {
  class Joystick : public curtinfrc::Joystick {
   public:
    using curtinfrc::Joystick::Joystick;

    double GetAxis(AxisType axis) override {
      switch (axis) {
       case curtinfrc::Joystick::kXAxis:
       case kYAxis:
       case kZAxis:
        return _axis[(int)axis];

       case kTwistAxis:
        return _axis[kDefaultTwistChannel];
      
       case kThrottleAxis:
        return _axis[kDefaultThrottleChannel];
      }
    };

    void SetAxis(AxisType axis, double val) {
      switch (axis) {
       case kXAxis:
       case kYAxis:
       case kZAxis:
        _axis[(int)axis] = val;
        break;

       case kTwistAxis:
        _axis[kDefaultTwistChannel] = val;
        break;
      
       case kThrottleAxis:
        _axis[kDefaultThrottleChannel] = val;
        break;
      }
    };

   private:
    double _axis[4] = { 0, 0, 0, 0 };
  };
} // ns testing


TEST (testing_Joystick, Constructor) {
  testing::Joystick joystickDef(0);

  ASSERT_EQ(joystickDef.GetPort(), 0);
  ASSERT_EQ(joystickDef.GetAxis(joystickDef.kXAxis), 0);
  ASSERT_EQ(joystickDef.GetAxis(joystickDef.kYAxis), 0);
  ASSERT_EQ(joystickDef.GetAxis(joystickDef.kZAxis), 0);
  ASSERT_EQ(joystickDef.GetAxis(joystickDef.kTwistAxis), 0);
  ASSERT_EQ(joystickDef.GetAxis(joystickDef.kThrottleAxis), 0);
}

TEST (testing_Joystick, SetAxis) {
  testing::Joystick joystickDef(0);

  joystickDef.SetAxis(joystickDef.kXAxis, 0.5);
  ASSERT_EQ(joystickDef.GetAxis(joystickDef.kXAxis), 0.5);
  ASSERT_EQ(joystickDef.GetAxis(joystickDef.kYAxis), 0);
  ASSERT_EQ(joystickDef.GetAxis(joystickDef.kZAxis), 0);
  ASSERT_EQ(joystickDef.GetAxis(joystickDef.kTwistAxis), 0);
  ASSERT_EQ(joystickDef.GetAxis(joystickDef.kThrottleAxis), 0);

  joystickDef.SetAxis(joystickDef.kYAxis, 0.34);
  ASSERT_EQ(joystickDef.GetAxis(joystickDef.kXAxis), 0.5);
  ASSERT_EQ(joystickDef.GetAxis(joystickDef.kYAxis), 0.34);
  ASSERT_EQ(joystickDef.GetAxis(joystickDef.kZAxis), 0);
  ASSERT_EQ(joystickDef.GetAxis(joystickDef.kTwistAxis), 0);
  ASSERT_EQ(joystickDef.GetAxis(joystickDef.kThrottleAxis), 0);

  joystickDef.SetAxis(joystickDef.kZAxis, -0.8);
  ASSERT_EQ(joystickDef.GetAxis(joystickDef.kXAxis), 0.5);
  ASSERT_EQ(joystickDef.GetAxis(joystickDef.kYAxis), 0.34);
  ASSERT_EQ(joystickDef.GetAxis(joystickDef.kZAxis), -0.8);
  ASSERT_EQ(joystickDef.GetAxis(joystickDef.kTwistAxis), -0.8);
  ASSERT_EQ(joystickDef.GetAxis(joystickDef.kThrottleAxis), 0);

  joystickDef.SetAxis(joystickDef.kTwistAxis, 1);
  ASSERT_EQ(joystickDef.GetAxis(joystickDef.kXAxis), 0.5);
  ASSERT_EQ(joystickDef.GetAxis(joystickDef.kYAxis), 0.34);
  ASSERT_EQ(joystickDef.GetAxis(joystickDef.kZAxis), 1);
  ASSERT_EQ(joystickDef.GetAxis(joystickDef.kTwistAxis), 1);
  ASSERT_EQ(joystickDef.GetAxis(joystickDef.kThrottleAxis), 0);

  joystickDef.SetAxis(joystickDef.kThrottleAxis, 0.1);
  ASSERT_EQ(joystickDef.GetAxis(joystickDef.kXAxis), 0.5);
  ASSERT_EQ(joystickDef.GetAxis(joystickDef.kYAxis), 0.34);
  ASSERT_EQ(joystickDef.GetAxis(joystickDef.kZAxis), 1);
  ASSERT_EQ(joystickDef.GetAxis(joystickDef.kTwistAxis), 1);
  ASSERT_EQ(joystickDef.GetAxis(joystickDef.kThrottleAxis), 0.1);
}

TEST (testing_Joystick, SingleMaxAxis) { // Testing when a single axis is at max magnitude
  testing::Joystick joy(0);

  // Front
  joy.SetAxis(joy.kYAxis, -1);
  joy.SetAxis(joy.kXAxis,  0);
  ASSERT_NEAR(joy.GetCircularisedAxis(joy.kYAxis), -1, EPS);
  ASSERT_NEAR(joy.GetCircularisedAxis(joy.kXAxis),  0, EPS);

  // Right
  joy.SetAxis(joy.kYAxis,  0);
  joy.SetAxis(joy.kXAxis,  1);
  ASSERT_NEAR(joy.GetCircularisedAxis(joy.kYAxis),  0, EPS);
  ASSERT_NEAR(joy.GetCircularisedAxis(joy.kXAxis),  1, EPS);

  // Back
  joy.SetAxis(joy.kYAxis,  1);
  joy.SetAxis(joy.kXAxis,  0);
  ASSERT_NEAR(joy.GetCircularisedAxis(joy.kYAxis),  1, EPS);
  ASSERT_NEAR(joy.GetCircularisedAxis(joy.kXAxis),  0, EPS);

  // Left
  joy.SetAxis(joy.kYAxis,  0);
  joy.SetAxis(joy.kXAxis, -1);
  ASSERT_NEAR(joy.GetCircularisedAxis(joy.kYAxis),  0, EPS);
  ASSERT_NEAR(joy.GetCircularisedAxis(joy.kXAxis), -1, EPS);
}

TEST (testing_Joystick, SingleHalfMaxAxis) { // Testing when a single axis is at half max magnitude
  testing::Joystick joy(0);

  // Front
  joy.SetAxis(joy.kYAxis, -0.5);
  joy.SetAxis(joy.kXAxis,  0);
  ASSERT_NEAR(joy.GetCircularisedAxis(joy.kYAxis), -0.5, EPS);
  ASSERT_NEAR(joy.GetCircularisedAxis(joy.kXAxis),  0, EPS);

  // Right
  joy.SetAxis(joy.kYAxis,  0);
  joy.SetAxis(joy.kXAxis,  0.5);
  ASSERT_NEAR(joy.GetCircularisedAxis(joy.kYAxis),  0, EPS);
  ASSERT_NEAR(joy.GetCircularisedAxis(joy.kXAxis),  0.5, EPS);

  // Back
  joy.SetAxis(joy.kYAxis,  0.5);
  joy.SetAxis(joy.kXAxis,  0);
  ASSERT_NEAR(joy.GetCircularisedAxis(joy.kYAxis),  0.5, EPS);
  ASSERT_NEAR(joy.GetCircularisedAxis(joy.kXAxis),  0, EPS);

  // Left
  joy.SetAxis(joy.kYAxis,  0);
  joy.SetAxis(joy.kXAxis, -0.5);
  ASSERT_NEAR(joy.GetCircularisedAxis(joy.kYAxis),  0, EPS);
  ASSERT_NEAR(joy.GetCircularisedAxis(joy.kXAxis), -0.5, EPS);
}

TEST (testing_Joystick, DualMaxAxis) { // Testing when both axi are at max magnitude
  testing::Joystick joy(0);

  // Front/Right
  joy.SetAxis(joy.kYAxis, -1);
  joy.SetAxis(joy.kXAxis,  1);
  ASSERT_NEAR(joy.GetCircularisedAxis(joy.kYAxis), -sqrt(0.5), EPS);
  ASSERT_NEAR(joy.GetCircularisedAxis(joy.kXAxis),  sqrt(0.5), EPS);

  // Back/Right
  joy.SetAxis(joy.kYAxis,  1);
  joy.SetAxis(joy.kXAxis,  1);
  ASSERT_NEAR(joy.GetCircularisedAxis(joy.kYAxis),  sqrt(0.5), EPS);
  ASSERT_NEAR(joy.GetCircularisedAxis(joy.kXAxis),  sqrt(0.5), EPS);

  // Back/Left
  joy.SetAxis(joy.kYAxis,  1);
  joy.SetAxis(joy.kXAxis, -1);
  ASSERT_NEAR(joy.GetCircularisedAxis(joy.kYAxis),  sqrt(0.5), EPS);
  ASSERT_NEAR(joy.GetCircularisedAxis(joy.kXAxis), -sqrt(0.5), EPS);

  // Front/Left
  joy.SetAxis(joy.kYAxis, -1);
  joy.SetAxis(joy.kXAxis, -1);
  ASSERT_NEAR(joy.GetCircularisedAxis(joy.kYAxis), -sqrt(0.5), EPS);
  ASSERT_NEAR(joy.GetCircularisedAxis(joy.kXAxis), -sqrt(0.5), EPS);
}

TEST (testing_Joystick, DualHalfMaxAxis) { // Testing when both axi are at half max magnitude
  testing::Joystick joy(0);

  // Front/Right
  joy.SetAxis(joy.kYAxis, -0.5);
  joy.SetAxis(joy.kXAxis,  0.5);
  ASSERT_NEAR(joy.GetCircularisedAxis(joy.kYAxis), -sqrt(0.125), EPS);
  ASSERT_NEAR(joy.GetCircularisedAxis(joy.kXAxis),  sqrt(0.125), EPS);

  // Back/Right
  joy.SetAxis(joy.kYAxis,  0.5);
  joy.SetAxis(joy.kXAxis,  0.5);
  ASSERT_NEAR(joy.GetCircularisedAxis(joy.kYAxis),  sqrt(0.125), EPS);
  ASSERT_NEAR(joy.GetCircularisedAxis(joy.kXAxis),  sqrt(0.125), EPS);

  // Back/Left
  joy.SetAxis(joy.kYAxis,  0.5);
  joy.SetAxis(joy.kXAxis, -0.5);
  ASSERT_NEAR(joy.GetCircularisedAxis(joy.kYAxis),  sqrt(0.125), EPS);
  ASSERT_NEAR(joy.GetCircularisedAxis(joy.kXAxis), -sqrt(0.125), EPS);

  // Front/Left
  joy.SetAxis(joy.kYAxis, -0.5);
  joy.SetAxis(joy.kXAxis, -0.5);
  ASSERT_NEAR(joy.GetCircularisedAxis(joy.kYAxis), -sqrt(0.125), EPS);
  ASSERT_NEAR(joy.GetCircularisedAxis(joy.kXAxis), -sqrt(0.125), EPS);
}

TEST (testing_Joystick, SplitMaxAxis) { // Testing when one axis is at max mag, and the other at half max mag
  testing::Joystick joy(0);

  // Bearing 26.6
  joy.SetAxis(joy.kYAxis, -1);
  joy.SetAxis(joy.kXAxis,  0.5);
  ASSERT_NEAR(joy.GetCircularisedAxis(joy.kYAxis), -sqrt(0.8), EPS);
  ASSERT_NEAR(joy.GetCircularisedAxis(joy.kXAxis),  sqrt(0.2), EPS);

  // Bearing 63.4
  joy.SetAxis(joy.kYAxis, -0.5);
  joy.SetAxis(joy.kXAxis,  1);
  ASSERT_NEAR(joy.GetCircularisedAxis(joy.kYAxis), -sqrt(0.2), EPS);
  ASSERT_NEAR(joy.GetCircularisedAxis(joy.kXAxis),  sqrt(0.8), EPS);


  // Bearing 116.6
  joy.SetAxis(joy.kYAxis, 0.5);
  joy.SetAxis(joy.kXAxis, 1);
  ASSERT_NEAR(joy.GetCircularisedAxis(joy.kYAxis),  sqrt(0.2), EPS);
  ASSERT_NEAR(joy.GetCircularisedAxis(joy.kXAxis),  sqrt(0.8), EPS);

  // Bearing 153.4
  joy.SetAxis(joy.kYAxis, 1);
  joy.SetAxis(joy.kXAxis, 0.5);
  ASSERT_NEAR(joy.GetCircularisedAxis(joy.kYAxis),  sqrt(0.8), EPS);
  ASSERT_NEAR(joy.GetCircularisedAxis(joy.kXAxis),  sqrt(0.2), EPS);


  // Bearing 203.6
  joy.SetAxis(joy.kYAxis,  1);
  joy.SetAxis(joy.kXAxis, -0.5);
  ASSERT_NEAR(joy.GetCircularisedAxis(joy.kYAxis),  sqrt(0.8), EPS);
  ASSERT_NEAR(joy.GetCircularisedAxis(joy.kXAxis), -sqrt(0.2), EPS);

  // Bearing 243.4
  joy.SetAxis(joy.kYAxis,  0.5);
  joy.SetAxis(joy.kXAxis, -1);
  ASSERT_NEAR(joy.GetCircularisedAxis(joy.kYAxis),  sqrt(0.2), EPS);
  ASSERT_NEAR(joy.GetCircularisedAxis(joy.kXAxis), -sqrt(0.8), EPS);


  // Bearing 293.6
  joy.SetAxis(joy.kYAxis, -0.5);
  joy.SetAxis(joy.kXAxis, -1);
  ASSERT_NEAR(joy.GetCircularisedAxis(joy.kYAxis), -sqrt(0.2), EPS);
  ASSERT_NEAR(joy.GetCircularisedAxis(joy.kXAxis), -sqrt(0.8), EPS);

  // Bearing 333.4
  joy.SetAxis(joy.kYAxis, -1);
  joy.SetAxis(joy.kXAxis, -0.5);
  ASSERT_NEAR(joy.GetCircularisedAxis(joy.kYAxis), -sqrt(0.8), EPS);
  ASSERT_NEAR(joy.GetCircularisedAxis(joy.kXAxis), -sqrt(0.2), EPS);
}
