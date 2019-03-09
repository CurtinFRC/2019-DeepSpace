#include "gtest/gtest.h"

#include "controllers/CurtinControllers.h"

#define EPS 0.00001

using AxisMap = curtinfrc::controllers::Joystick::AxisMap;

namespace testing {
  class Joystick : public frc::GenericHID {
   public:
    using frc::GenericHID::GenericHID;

    virtual double GetX(JoystickHand hand = kRightHand) const override { return _axis[1]; };
    virtual double GetY(JoystickHand hand = kRightHand) const override { return _axis[0]; };

    bool GetRawButton(int button) const {
      return _buttons[button - 1];
    };

    void SetButton(int button, bool val) {
      _buttons[button - 1] = val;
    };


    double GetRawAxis(int axis) const {
      return _axis[axis];
    };

    void SetAxis(int axis, double val) {
      _axis[axis] = val;
    };

   private:
    bool _buttons[12] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    double _axis[4] = { 0, 0, 0, 0 };
  };
} // ns testing


// CONSTRUCTION TEST

TEST (testing_Joystick, Constructor) {
  testing::Joystick raw(0);
  curtinfrc::controllers::SmartController joy(&raw, { 4, 12, 1 });
  // joy.PairAxis({ -1, AxisMap::kYAxis }, { -1, AxisMap::kXAxis }, true);

  ASSERT_EQ(joy.GetPort(), 0);
  ASSERT_EQ(joy.GetAxis(AxisMap::kXAxis), 0);
//   ASSERT_EQ(joy.GetAxis(AxisMap::kYAxis), 0);
//   ASSERT_EQ(joy.GetAxis(AxisMap::kZAxis), 0);
//   ASSERT_EQ(joy.GetAxis(AxisMap::kTwistAxis), 0);
//   ASSERT_EQ(joy.GetAxis(AxisMap::kThrottleAxis), 0);
}


// // SETTER TESTS

// TEST (testing_Joystick, SetButton) {
//   testing::Joystick raw(0);
//   curtinfrc::controllers::SmartController joy(&raw, { 4, 12, 1 });
//   joy.PairAxis({ -1, AxisMap::kYAxis }, { -1, AxisMap::kXAxis }, true);

//   for (int i = 1; i <= 12; i++) {
//     EXPECT_FALSE(joy.GetButton(i));

//     for (int j = 0; j < 2; j++) {
//       raw.SetButton(i, 0);
//       ASSERT_FALSE(joy.GetButton(i));
//       ASSERT_FALSE(joy.GetButton(i));

//       raw.SetButton(i, 1);
//       ASSERT_TRUE(joy.GetButton(i));
//       ASSERT_TRUE(joy.GetButton(i));
//     }
//   }
// }

// TEST (testing_Joystick, SetAxis) {
//   testing::Joystick raw(0);
//   curtinfrc::controllers::SmartController joy(&raw, { 4, 12, 1 });
//   joy.PairAxis({ -1, AxisMap::kYAxis }, { -1, AxisMap::kXAxis }, true);

//   raw.SetAxis(AxisMap::kXAxis, 0.5);
//   ASSERT_EQ(joy.GetAxis(AxisMap::kXAxis), 0.5);
//   ASSERT_EQ(joy.GetAxis(AxisMap::kYAxis), 0);
//   ASSERT_EQ(joy.GetAxis(AxisMap::kZAxis), 0);
//   ASSERT_EQ(joy.GetAxis(AxisMap::kTwistAxis), 0);
//   ASSERT_EQ(joy.GetAxis(AxisMap::kThrottleAxis), 0);

//   raw.SetAxis(AxisMap::kYAxis, 0.34);
//   ASSERT_EQ(joy.GetAxis(AxisMap::kXAxis), 0.5);
//   ASSERT_EQ(joy.GetAxis(AxisMap::kYAxis), 0.34);
//   ASSERT_EQ(joy.GetAxis(AxisMap::kZAxis), 0);
//   ASSERT_EQ(joy.GetAxis(AxisMap::kTwistAxis), 0);
//   ASSERT_EQ(joy.GetAxis(AxisMap::kThrottleAxis), 0);

//   raw.SetAxis(AxisMap::kZAxis, -0.8);
//   ASSERT_EQ(joy.GetAxis(AxisMap::kXAxis), 0.5);
//   ASSERT_EQ(joy.GetAxis(AxisMap::kYAxis), 0.34);
//   ASSERT_EQ(joy.GetAxis(AxisMap::kZAxis), -0.8);
//   ASSERT_EQ(joy.GetAxis(AxisMap::kTwistAxis), -0.8);
//   ASSERT_EQ(joy.GetAxis(AxisMap::kThrottleAxis), 0);

//   raw.SetAxis(AxisMap::kTwistAxis, 1);
//   ASSERT_EQ(joy.GetAxis(AxisMap::kXAxis), 0.5);
//   ASSERT_EQ(joy.GetAxis(AxisMap::kYAxis), 0.34);
//   ASSERT_EQ(joy.GetAxis(AxisMap::kZAxis), 1);
//   ASSERT_EQ(joy.GetAxis(AxisMap::kTwistAxis), 1);
//   ASSERT_EQ(joy.GetAxis(AxisMap::kThrottleAxis), 0);

//   raw.SetAxis(AxisMap::kThrottleAxis, 0.1);
//   ASSERT_EQ(joy.GetAxis(AxisMap::kXAxis), 0.5);
//   ASSERT_EQ(joy.GetAxis(AxisMap::kYAxis), 0.34);
//   ASSERT_EQ(joy.GetAxis(AxisMap::kZAxis), 1);
//   ASSERT_EQ(joy.GetAxis(AxisMap::kTwistAxis), 1);
//   ASSERT_EQ(joy.GetAxis(AxisMap::kThrottleAxis), 0.1);
// }


// // CIRCULARISATION TESTS

// TEST (testing_Joystick, SingleMaxAxis) { // Testing when a single axis is at max magnitude
//   testing::Joystick raw(0);
//   curtinfrc::controllers::SmartController joy(&raw, { 4, 12, 1 });
//   joy.PairAxis({ -1, AxisMap::kYAxis }, { -1, AxisMap::kXAxis }, true);

//   // Front
//   raw.SetAxis(AxisMap::kYAxis, -1);
//   raw.SetAxis(AxisMap::kXAxis,  0);
//   ASSERT_NEAR(joy.GetAxis(AxisMap::kYAxis), -1, EPS);
//   ASSERT_NEAR(joy.GetAxis(AxisMap::kXAxis),  0, EPS);

//   // Right
//   raw.SetAxis(AxisMap::kYAxis,  0);
//   raw.SetAxis(AxisMap::kXAxis,  1);
//   ASSERT_NEAR(joy.GetAxis(AxisMap::kYAxis),  0, EPS);
//   ASSERT_NEAR(joy.GetAxis(AxisMap::kXAxis),  1, EPS);

//   // Back
//   raw.SetAxis(AxisMap::kYAxis,  1);
//   raw.SetAxis(AxisMap::kXAxis,  0);
//   ASSERT_NEAR(joy.GetAxis(AxisMap::kYAxis),  1, EPS);
//   ASSERT_NEAR(joy.GetAxis(AxisMap::kXAxis),  0, EPS);

//   // Left
//   raw.SetAxis(AxisMap::kYAxis,  0);
//   raw.SetAxis(AxisMap::kXAxis, -1);
//   ASSERT_NEAR(joy.GetAxis(AxisMap::kYAxis),  0, EPS);
//   ASSERT_NEAR(joy.GetAxis(AxisMap::kXAxis), -1, EPS);
// }

// TEST (testing_Joystick, SingleHalfMaxAxis) { // Testing when a single axis is at half max magnitude
//   testing::Joystick raw(0);
//   curtinfrc::controllers::SmartController joy(&raw, { 4, 12, 1 });
//   joy.PairAxis({ -1, AxisMap::kYAxis }, { -1, AxisMap::kXAxis }, true);

//   // Front
//   raw.SetAxis(AxisMap::kYAxis, -0.5);
//   raw.SetAxis(AxisMap::kXAxis,  0);
//   ASSERT_NEAR(joy.GetAxis(AxisMap::kYAxis), -0.5, EPS);
//   ASSERT_NEAR(joy.GetAxis(AxisMap::kXAxis),  0, EPS);

//   // Right
//   raw.SetAxis(AxisMap::kYAxis,  0);
//   raw.SetAxis(AxisMap::kXAxis,  0.5);
//   ASSERT_NEAR(joy.GetAxis(AxisMap::kYAxis),  0, EPS);
//   ASSERT_NEAR(joy.GetAxis(AxisMap::kXAxis),  0.5, EPS);

//   // Back
//   raw.SetAxis(AxisMap::kYAxis,  0.5);
//   raw.SetAxis(AxisMap::kXAxis,  0);
//   ASSERT_NEAR(joy.GetAxis(AxisMap::kYAxis),  0.5, EPS);
//   ASSERT_NEAR(joy.GetAxis(AxisMap::kXAxis),  0, EPS);

//   // Left
//   raw.SetAxis(AxisMap::kYAxis,  0);
//   raw.SetAxis(AxisMap::kXAxis, -0.5);
//   ASSERT_NEAR(joy.GetAxis(AxisMap::kYAxis),  0, EPS);
//   ASSERT_NEAR(joy.GetAxis(AxisMap::kXAxis), -0.5, EPS);
// }

// TEST (testing_Joystick, DualMaxAxis) { // Testing when both axi are at max magnitude
//   testing::Joystick raw(0);
//   curtinfrc::controllers::SmartController joy(&raw, { 4, 12, 1 });
//   joy.PairAxis({ -1, AxisMap::kYAxis }, { -1, AxisMap::kXAxis }, true);

//   // Front/Right
//   raw.SetAxis(AxisMap::kYAxis, -1);
//   raw.SetAxis(AxisMap::kXAxis,  1);
//   ASSERT_NEAR(joy.GetAxis(AxisMap::kYAxis), -sqrt(0.5), EPS);
//   ASSERT_NEAR(joy.GetAxis(AxisMap::kXAxis),  sqrt(0.5), EPS);

//   // Back/Right
//   raw.SetAxis(AxisMap::kYAxis,  1);
//   raw.SetAxis(AxisMap::kXAxis,  1);
//   ASSERT_NEAR(joy.GetAxis(AxisMap::kYAxis),  sqrt(0.5), EPS);
//   ASSERT_NEAR(joy.GetAxis(AxisMap::kXAxis),  sqrt(0.5), EPS);

//   // Back/Left
//   raw.SetAxis(AxisMap::kYAxis,  1);
//   raw.SetAxis(AxisMap::kXAxis, -1);
//   ASSERT_NEAR(joy.GetAxis(AxisMap::kYAxis),  sqrt(0.5), EPS);
//   ASSERT_NEAR(joy.GetAxis(AxisMap::kXAxis), -sqrt(0.5), EPS);

//   // Front/Left
//   raw.SetAxis(AxisMap::kYAxis, -1);
//   raw.SetAxis(AxisMap::kXAxis, -1);
//   ASSERT_NEAR(joy.GetAxis(AxisMap::kYAxis), -sqrt(0.5), EPS);
//   ASSERT_NEAR(joy.GetAxis(AxisMap::kXAxis), -sqrt(0.5), EPS);
// }

// TEST (testing_Joystick, DualHalfMaxAxis) { // Testing when both axi are at half max magnitude
//   testing::Joystick raw(0);
//   curtinfrc::controllers::SmartController joy(&raw, { 4, 12, 1 });
//   joy.PairAxis({ -1, AxisMap::kYAxis }, { -1, AxisMap::kXAxis }, true);

//   // Front/Right
//   raw.SetAxis(AxisMap::kYAxis, -0.5);
//   raw.SetAxis(AxisMap::kXAxis,  0.5);
//   ASSERT_NEAR(joy.GetAxis(AxisMap::kYAxis), -sqrt(0.125), EPS);
//   ASSERT_NEAR(joy.GetAxis(AxisMap::kXAxis),  sqrt(0.125), EPS);

//   // Back/Right
//   raw.SetAxis(AxisMap::kYAxis,  0.5);
//   raw.SetAxis(AxisMap::kXAxis,  0.5);
//   ASSERT_NEAR(joy.GetAxis(AxisMap::kYAxis),  sqrt(0.125), EPS);
//   ASSERT_NEAR(joy.GetAxis(AxisMap::kXAxis),  sqrt(0.125), EPS);

//   // Back/Left
//   raw.SetAxis(AxisMap::kYAxis,  0.5);
//   raw.SetAxis(AxisMap::kXAxis, -0.5);
//   ASSERT_NEAR(joy.GetAxis(AxisMap::kYAxis),  sqrt(0.125), EPS);
//   ASSERT_NEAR(joy.GetAxis(AxisMap::kXAxis), -sqrt(0.125), EPS);

//   // Front/Left
//   raw.SetAxis(AxisMap::kYAxis, -0.5);
//   raw.SetAxis(AxisMap::kXAxis, -0.5);
//   ASSERT_NEAR(joy.GetAxis(AxisMap::kYAxis), -sqrt(0.125), EPS);
//   ASSERT_NEAR(joy.GetAxis(AxisMap::kXAxis), -sqrt(0.125), EPS);
// }

// TEST (testing_Joystick, SplitMaxAxis) { // Testing when one axis is at max mag, and the other at half max mag
//   testing::Joystick raw(0);
//   curtinfrc::controllers::SmartController joy(&raw, { 4, 12, 1 });
//   joy.PairAxis({ -1, AxisMap::kYAxis }, { -1, AxisMap::kXAxis }, true);

//   // Bearing 26.6
//   raw.SetAxis(AxisMap::kYAxis, -1);
//   raw.SetAxis(AxisMap::kXAxis,  0.5);
//   ASSERT_NEAR(joy.GetAxis(AxisMap::kYAxis), -sqrt(0.8), EPS);
//   ASSERT_NEAR(joy.GetAxis(AxisMap::kXAxis),  sqrt(0.2), EPS);

//   // Bearing 63.4
//   raw.SetAxis(AxisMap::kYAxis, -0.5);
//   raw.SetAxis(AxisMap::kXAxis,  1);
//   ASSERT_NEAR(joy.GetAxis(AxisMap::kYAxis), -sqrt(0.2), EPS);
//   ASSERT_NEAR(joy.GetAxis(AxisMap::kXAxis),  sqrt(0.8), EPS);


//   // Bearing 116.6
//   raw.SetAxis(AxisMap::kYAxis, 0.5);
//   raw.SetAxis(AxisMap::kXAxis, 1);
//   ASSERT_NEAR(joy.GetAxis(AxisMap::kYAxis),  sqrt(0.2), EPS);
//   ASSERT_NEAR(joy.GetAxis(AxisMap::kXAxis),  sqrt(0.8), EPS);

//   // Bearing 153.4
//   raw.SetAxis(AxisMap::kYAxis, 1);
//   raw.SetAxis(AxisMap::kXAxis, 0.5);
//   ASSERT_NEAR(joy.GetAxis(AxisMap::kYAxis),  sqrt(0.8), EPS);
//   ASSERT_NEAR(joy.GetAxis(AxisMap::kXAxis),  sqrt(0.2), EPS);


//   // Bearing 203.6
//   raw.SetAxis(AxisMap::kYAxis,  1);
//   raw.SetAxis(AxisMap::kXAxis, -0.5);
//   ASSERT_NEAR(joy.GetAxis(AxisMap::kYAxis),  sqrt(0.8), EPS);
//   ASSERT_NEAR(joy.GetAxis(AxisMap::kXAxis), -sqrt(0.2), EPS);

//   // Bearing 243.4
//   raw.SetAxis(AxisMap::kYAxis,  0.5);
//   raw.SetAxis(AxisMap::kXAxis, -1);
//   ASSERT_NEAR(joy.GetAxis(AxisMap::kYAxis),  sqrt(0.2), EPS);
//   ASSERT_NEAR(joy.GetAxis(AxisMap::kXAxis), -sqrt(0.8), EPS);


//   // Bearing 293.6
//   raw.SetAxis(AxisMap::kYAxis, -0.5);
//   raw.SetAxis(AxisMap::kXAxis, -1);
//   ASSERT_NEAR(joy.GetAxis(AxisMap::kYAxis), -sqrt(0.2), EPS);
//   ASSERT_NEAR(joy.GetAxis(AxisMap::kXAxis), -sqrt(0.8), EPS);

//   // Bearing 333.4
//   raw.SetAxis(AxisMap::kYAxis, -1);
//   raw.SetAxis(AxisMap::kXAxis, -0.5);
//   ASSERT_NEAR(joy.GetAxis(AxisMap::kYAxis), -sqrt(0.8), EPS);
//   ASSERT_NEAR(joy.GetAxis(AxisMap::kXAxis), -sqrt(0.2), EPS);
// }



// // JOYSTICK TOGGLE TESTS

// TEST (testing_Joystick, GetButtonRise) {
//   testing::Joystick raw(0);
//   curtinfrc::controllers::SmartController joy(&raw, { 4, 12, 1 });
//   joy.PairAxis({ -1, AxisMap::kYAxis }, { -1, AxisMap::kXAxis }, true);

//   EXPECT_FALSE(joy.Get({ -1, 1 }, joy.ONRISE));

//   for (int i = 0; i < 2; i++) {
//     raw.SetButton(1, true);
//     ASSERT_TRUE(joy.Get({ -1, 1 }, joy.ONRISE));
//     ASSERT_FALSE(joy.Get({ -1, 1 }, joy.ONRISE));
//     ASSERT_FALSE(joy.Get({ -1, 1 }, joy.ONRISE));

//     raw.SetButton(1, false);
//     ASSERT_FALSE(joy.Get({ -1, 1 }, joy.ONRISE));
//     ASSERT_FALSE(joy.Get({ -1, 1 }, joy.ONRISE));
//   }
// }

// TEST (testing_Joystick, GetButtonFall) {
//   testing::Joystick raw(0);
//   curtinfrc::controllers::SmartController joy(&raw, { 4, 12, 1 });
//   joy.PairAxis({ -1, AxisMap::kYAxis }, { -1, AxisMap::kXAxis }, true);

//   EXPECT_FALSE(joy.Get({ -1, 1 }, joy.ONFALL));

//   for (int i = 0; i < 2; i++) {
//     raw.SetButton(1, true);
//     ASSERT_FALSE(joy.Get({ -1, 1 }, joy.ONFALL));
//     ASSERT_FALSE(joy.Get({ -1, 1 }, joy.ONFALL));

//     raw.SetButton(1, false);
//     ASSERT_TRUE(joy.Get({ -1, 1 }, joy.ONFALL));
//     ASSERT_FALSE(joy.Get({ -1, 1 }, joy.ONFALL));
//     ASSERT_FALSE(joy.Get({ -1, 1 }, joy.ONFALL));
//   }
// }
