#include "gtest/gtest.h"

#include "CurtinControllers.h"

#define EPS 0.00001

namespace testing {
  class Joystick : public curtinfrc::Joystick {
   public:
    using curtinfrc::Joystick::Joystick;

    bool GetButton(int button) override {
      return _buttons[button - 1];
    };

    void SetButton(int button, bool val) {
      _buttons[button - 1] = val;
    };


    double GetAxis(int axis) override {
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
  testing::Joystick joy(0);

  ASSERT_EQ(joy.GetPort(), 0);
  ASSERT_EQ(joy.GetAxis(joy.kXAxis), 0);
  ASSERT_EQ(joy.GetAxis(joy.kYAxis), 0);
  ASSERT_EQ(joy.GetAxis(joy.kZAxis), 0);
  ASSERT_EQ(joy.GetAxis(joy.kTwistAxis), 0);
  ASSERT_EQ(joy.GetAxis(joy.kThrottleAxis), 0);
}


// SETTER TESTS

TEST (testing_Joystick, SetButton) {
  testing::Joystick joy(0);

  for (int i = 1; i <= 12; i++) {
    EXPECT_FALSE(joy.GetButton(i));

    for (int j = 0; j < 2; j++) {
      joy.SetButton(i, 0);
      ASSERT_FALSE(joy.GetButton(i));
      ASSERT_FALSE(joy.GetButton(i));

      joy.SetButton(i, 1);
      ASSERT_TRUE(joy.GetButton(i));
      ASSERT_TRUE(joy.GetButton(i));
    }
  }
}

TEST (testing_Joystick, SetAxis) {
  testing::Joystick joy(0);

  joy.SetAxis(joy.kXAxis, 0.5);
  ASSERT_EQ(joy.GetAxis(joy.kXAxis), 0.5);
  ASSERT_EQ(joy.GetAxis(joy.kYAxis), 0);
  ASSERT_EQ(joy.GetAxis(joy.kZAxis), 0);
  ASSERT_EQ(joy.GetAxis(joy.kTwistAxis), 0);
  ASSERT_EQ(joy.GetAxis(joy.kThrottleAxis), 0);

  joy.SetAxis(joy.kYAxis, 0.34);
  ASSERT_EQ(joy.GetAxis(joy.kXAxis), 0.5);
  ASSERT_EQ(joy.GetAxis(joy.kYAxis), 0.34);
  ASSERT_EQ(joy.GetAxis(joy.kZAxis), 0);
  ASSERT_EQ(joy.GetAxis(joy.kTwistAxis), 0);
  ASSERT_EQ(joy.GetAxis(joy.kThrottleAxis), 0);

  joy.SetAxis(joy.kZAxis, -0.8);
  ASSERT_EQ(joy.GetAxis(joy.kXAxis), 0.5);
  ASSERT_EQ(joy.GetAxis(joy.kYAxis), 0.34);
  ASSERT_EQ(joy.GetAxis(joy.kZAxis), -0.8);
  ASSERT_EQ(joy.GetAxis(joy.kTwistAxis), -0.8);
  ASSERT_EQ(joy.GetAxis(joy.kThrottleAxis), 0);

  joy.SetAxis(joy.kTwistAxis, 1);
  ASSERT_EQ(joy.GetAxis(joy.kXAxis), 0.5);
  ASSERT_EQ(joy.GetAxis(joy.kYAxis), 0.34);
  ASSERT_EQ(joy.GetAxis(joy.kZAxis), 1);
  ASSERT_EQ(joy.GetAxis(joy.kTwistAxis), 1);
  ASSERT_EQ(joy.GetAxis(joy.kThrottleAxis), 0);

  joy.SetAxis(joy.kThrottleAxis, 0.1);
  ASSERT_EQ(joy.GetAxis(joy.kXAxis), 0.5);
  ASSERT_EQ(joy.GetAxis(joy.kYAxis), 0.34);
  ASSERT_EQ(joy.GetAxis(joy.kZAxis), 1);
  ASSERT_EQ(joy.GetAxis(joy.kTwistAxis), 1);
  ASSERT_EQ(joy.GetAxis(joy.kThrottleAxis), 0.1);
}


// CIRCULARISATION TESTS

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



// JOYSTICK TOGGLE TESTS

TEST (testing_Joystick, GetButtonRise) {
  testing::Joystick joy(0);

  EXPECT_FALSE(joy.GetButtonRise(1));

  for (int i = 0; i < 2; i++) {
    joy.SetButton(1, true);
    ASSERT_TRUE(joy.GetButtonRise(1));
    ASSERT_FALSE(joy.GetButtonRise(1));
    ASSERT_FALSE(joy.GetButtonRise(1));

    joy.SetButton(1, false);
    ASSERT_FALSE(joy.GetButtonRise(1));
    ASSERT_FALSE(joy.GetButtonRise(1));
  }
}

TEST (testing_Joystick, GetButtonFall) {
  testing::Joystick joy(0);

  EXPECT_FALSE(joy.GetButtonFall(1));

  for (int i = 0; i < 2; i++) {
    joy.SetButton(1, true);
    ASSERT_FALSE(joy.GetButtonFall(1));
    ASSERT_FALSE(joy.GetButtonFall(1));

    joy.SetButton(1, false);
    ASSERT_TRUE(joy.GetButtonFall(1));
    ASSERT_FALSE(joy.GetButtonFall(1));
    ASSERT_FALSE(joy.GetButtonFall(1));
  }
}
