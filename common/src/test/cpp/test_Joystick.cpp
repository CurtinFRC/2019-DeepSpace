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
  testing::Joystick joystickDef(0);

  ASSERT_EQ(joystickDef.CurrentPort(), 0);
  ASSERT_EQ(joystickDef.GetAxis(joystickDef.kDefaultXChannel), 0);
  ASSERT_EQ(joystickDef.GetAxis(joystickDef.kDefaultYChannel), 0);
  ASSERT_EQ(joystickDef.GetAxis(joystickDef.kDefaultZChannel), 0);
  ASSERT_EQ(joystickDef.GetAxis(joystickDef.kDefaultTwistChannel), 0);
  ASSERT_EQ(joystickDef.GetAxis(joystickDef.kDefaultThrottleChannel), 0);
}


// SETTER TESTS

TEST (testing_Joystick, SetButton) {
  testing::Joystick joy(0);

  for (int i = 0; i < 12; i++) {
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

  joy.SetAxis(joy.kDefaultXChannel, 0.5);
  ASSERT_EQ(joy.GetAxis(joy.kDefaultXChannel), 0.5);
  ASSERT_EQ(joy.GetAxis(joy.kDefaultYChannel), 0);
  ASSERT_EQ(joy.GetAxis(joy.kDefaultZChannel), 0);
  ASSERT_EQ(joy.GetAxis(joy.kDefaultTwistChannel), 0);
  ASSERT_EQ(joy.GetAxis(joy.kDefaultThrottleChannel), 0);

  joy.SetAxis(joy.kDefaultYChannel, 0.34);
  ASSERT_EQ(joy.GetAxis(joy.kDefaultXChannel), 0.5);
  ASSERT_EQ(joy.GetAxis(joy.kDefaultYChannel), 0.34);
  ASSERT_EQ(joy.GetAxis(joy.kDefaultZChannel), 0);
  ASSERT_EQ(joy.GetAxis(joy.kDefaultTwistChannel), 0);
  ASSERT_EQ(joy.GetAxis(joy.kDefaultThrottleChannel), 0);

  joy.SetAxis(joy.kDefaultZChannel, -0.8);
  ASSERT_EQ(joy.GetAxis(joy.kDefaultXChannel), 0.5);
  ASSERT_EQ(joy.GetAxis(joy.kDefaultYChannel), 0.34);
  ASSERT_EQ(joy.GetAxis(joy.kDefaultZChannel), -0.8);
  ASSERT_EQ(joy.GetAxis(joy.kDefaultTwistChannel), -0.8);
  ASSERT_EQ(joy.GetAxis(joy.kDefaultThrottleChannel), 0);

  joy.SetAxis(joy.kDefaultTwistChannel, 1);
  ASSERT_EQ(joy.GetAxis(joy.kDefaultXChannel), 0.5);
  ASSERT_EQ(joy.GetAxis(joy.kDefaultYChannel), 0.34);
  ASSERT_EQ(joy.GetAxis(joy.kDefaultZChannel), 1);
  ASSERT_EQ(joy.GetAxis(joy.kDefaultTwistChannel), 1);
  ASSERT_EQ(joy.GetAxis(joy.kDefaultThrottleChannel), 0);

  joy.SetAxis(joy.kDefaultThrottleChannel, 0.1);
  ASSERT_EQ(joy.GetAxis(joy.kDefaultXChannel), 0.5);
  ASSERT_EQ(joy.GetAxis(joy.kDefaultYChannel), 0.34);
  ASSERT_EQ(joy.GetAxis(joy.kDefaultZChannel), 1);
  ASSERT_EQ(joy.GetAxis(joy.kDefaultTwistChannel), 1);
  ASSERT_EQ(joy.GetAxis(joy.kDefaultThrottleChannel), 0.1);
}


// CIRCULARISATION TESTS

TEST (testing_Joystick, SingleMaxAxis) { // Testing when a single axis is at max magnitude
  testing::Joystick joy(0);

  // Front
  joy.SetAxis(joy.kDefaultYChannel, -1);
  joy.SetAxis(joy.kDefaultXChannel,  0);
  ASSERT_NEAR(joy.GetCircularisedAxis(joy.kDefaultYChannel), -1, EPS);
  ASSERT_NEAR(joy.GetCircularisedAxis(joy.kDefaultXChannel),  0, EPS);

  // Right
  joy.SetAxis(joy.kDefaultYChannel,  0);
  joy.SetAxis(joy.kDefaultXChannel,  1);
  ASSERT_NEAR(joy.GetCircularisedAxis(joy.kDefaultYChannel),  0, EPS);
  ASSERT_NEAR(joy.GetCircularisedAxis(joy.kDefaultXChannel),  1, EPS);

  // Back
  joy.SetAxis(joy.kDefaultYChannel,  1);
  joy.SetAxis(joy.kDefaultXChannel,  0);
  ASSERT_NEAR(joy.GetCircularisedAxis(joy.kDefaultYChannel),  1, EPS);
  ASSERT_NEAR(joy.GetCircularisedAxis(joy.kDefaultXChannel),  0, EPS);

  // Left
  joy.SetAxis(joy.kDefaultYChannel,  0);
  joy.SetAxis(joy.kDefaultXChannel, -1);
  ASSERT_NEAR(joy.GetCircularisedAxis(joy.kDefaultYChannel),  0, EPS);
  ASSERT_NEAR(joy.GetCircularisedAxis(joy.kDefaultXChannel), -1, EPS);
}

TEST (testing_Joystick, SingleHalfMaxAxis) { // Testing when a single axis is at half max magnitude
  testing::Joystick joy(0);

  // Front
  joy.SetAxis(joy.kDefaultYChannel, -0.5);
  joy.SetAxis(joy.kDefaultXChannel,  0);
  ASSERT_NEAR(joy.GetCircularisedAxis(joy.kDefaultYChannel), -0.5, EPS);
  ASSERT_NEAR(joy.GetCircularisedAxis(joy.kDefaultXChannel),  0, EPS);

  // Right
  joy.SetAxis(joy.kDefaultYChannel,  0);
  joy.SetAxis(joy.kDefaultXChannel,  0.5);
  ASSERT_NEAR(joy.GetCircularisedAxis(joy.kDefaultYChannel),  0, EPS);
  ASSERT_NEAR(joy.GetCircularisedAxis(joy.kDefaultXChannel),  0.5, EPS);

  // Back
  joy.SetAxis(joy.kDefaultYChannel,  0.5);
  joy.SetAxis(joy.kDefaultXChannel,  0);
  ASSERT_NEAR(joy.GetCircularisedAxis(joy.kDefaultYChannel),  0.5, EPS);
  ASSERT_NEAR(joy.GetCircularisedAxis(joy.kDefaultXChannel),  0, EPS);

  // Left
  joy.SetAxis(joy.kDefaultYChannel,  0);
  joy.SetAxis(joy.kDefaultXChannel, -0.5);
  ASSERT_NEAR(joy.GetCircularisedAxis(joy.kDefaultYChannel),  0, EPS);
  ASSERT_NEAR(joy.GetCircularisedAxis(joy.kDefaultXChannel), -0.5, EPS);
}

TEST (testing_Joystick, DualMaxAxis) { // Testing when both axi are at max magnitude
  testing::Joystick joy(0);

  // Front/Right
  joy.SetAxis(joy.kDefaultYChannel, -1);
  joy.SetAxis(joy.kDefaultXChannel,  1);
  ASSERT_NEAR(joy.GetCircularisedAxis(joy.kDefaultYChannel), -sqrt(0.5), EPS);
  ASSERT_NEAR(joy.GetCircularisedAxis(joy.kDefaultXChannel),  sqrt(0.5), EPS);

  // Back/Right
  joy.SetAxis(joy.kDefaultYChannel,  1);
  joy.SetAxis(joy.kDefaultXChannel,  1);
  ASSERT_NEAR(joy.GetCircularisedAxis(joy.kDefaultYChannel),  sqrt(0.5), EPS);
  ASSERT_NEAR(joy.GetCircularisedAxis(joy.kDefaultXChannel),  sqrt(0.5), EPS);

  // Back/Left
  joy.SetAxis(joy.kDefaultYChannel,  1);
  joy.SetAxis(joy.kDefaultXChannel, -1);
  ASSERT_NEAR(joy.GetCircularisedAxis(joy.kDefaultYChannel),  sqrt(0.5), EPS);
  ASSERT_NEAR(joy.GetCircularisedAxis(joy.kDefaultXChannel), -sqrt(0.5), EPS);

  // Front/Left
  joy.SetAxis(joy.kDefaultYChannel, -1);
  joy.SetAxis(joy.kDefaultXChannel, -1);
  ASSERT_NEAR(joy.GetCircularisedAxis(joy.kDefaultYChannel), -sqrt(0.5), EPS);
  ASSERT_NEAR(joy.GetCircularisedAxis(joy.kDefaultXChannel), -sqrt(0.5), EPS);
}

TEST (testing_Joystick, DualHalfMaxAxis) { // Testing when both axi are at half max magnitude
  testing::Joystick joy(0);

  // Front/Right
  joy.SetAxis(joy.kDefaultYChannel, -0.5);
  joy.SetAxis(joy.kDefaultXChannel,  0.5);
  ASSERT_NEAR(joy.GetCircularisedAxis(joy.kDefaultYChannel), -sqrt(0.125), EPS);
  ASSERT_NEAR(joy.GetCircularisedAxis(joy.kDefaultXChannel),  sqrt(0.125), EPS);

  // Back/Right
  joy.SetAxis(joy.kDefaultYChannel,  0.5);
  joy.SetAxis(joy.kDefaultXChannel,  0.5);
  ASSERT_NEAR(joy.GetCircularisedAxis(joy.kDefaultYChannel),  sqrt(0.125), EPS);
  ASSERT_NEAR(joy.GetCircularisedAxis(joy.kDefaultXChannel),  sqrt(0.125), EPS);

  // Back/Left
  joy.SetAxis(joy.kDefaultYChannel,  0.5);
  joy.SetAxis(joy.kDefaultXChannel, -0.5);
  ASSERT_NEAR(joy.GetCircularisedAxis(joy.kDefaultYChannel),  sqrt(0.125), EPS);
  ASSERT_NEAR(joy.GetCircularisedAxis(joy.kDefaultXChannel), -sqrt(0.125), EPS);

  // Front/Left
  joy.SetAxis(joy.kDefaultYChannel, -0.5);
  joy.SetAxis(joy.kDefaultXChannel, -0.5);
  ASSERT_NEAR(joy.GetCircularisedAxis(joy.kDefaultYChannel), -sqrt(0.125), EPS);
  ASSERT_NEAR(joy.GetCircularisedAxis(joy.kDefaultXChannel), -sqrt(0.125), EPS);
}

TEST (testing_Joystick, SplitMaxAxis) { // Testing when one axis is at max mag, and the other at half max mag
  testing::Joystick joy(0);

  // Bearing 26.6
  joy.SetAxis(joy.kDefaultYChannel, -1);
  joy.SetAxis(joy.kDefaultXChannel,  0.5);
  ASSERT_NEAR(joy.GetCircularisedAxis(joy.kDefaultYChannel), -sqrt(0.8), EPS);
  ASSERT_NEAR(joy.GetCircularisedAxis(joy.kDefaultXChannel),  sqrt(0.2), EPS);

  // Bearing 63.4
  joy.SetAxis(joy.kDefaultYChannel, -0.5);
  joy.SetAxis(joy.kDefaultXChannel,  1);
  ASSERT_NEAR(joy.GetCircularisedAxis(joy.kDefaultYChannel), -sqrt(0.2), EPS);
  ASSERT_NEAR(joy.GetCircularisedAxis(joy.kDefaultXChannel),  sqrt(0.8), EPS);


  // Bearing 116.6
  joy.SetAxis(joy.kDefaultYChannel, 0.5);
  joy.SetAxis(joy.kDefaultXChannel, 1);
  ASSERT_NEAR(joy.GetCircularisedAxis(joy.kDefaultYChannel),  sqrt(0.2), EPS);
  ASSERT_NEAR(joy.GetCircularisedAxis(joy.kDefaultXChannel),  sqrt(0.8), EPS);

  // Bearing 153.4
  joy.SetAxis(joy.kDefaultYChannel, 1);
  joy.SetAxis(joy.kDefaultXChannel, 0.5);
  ASSERT_NEAR(joy.GetCircularisedAxis(joy.kDefaultYChannel),  sqrt(0.8), EPS);
  ASSERT_NEAR(joy.GetCircularisedAxis(joy.kDefaultXChannel),  sqrt(0.2), EPS);


  // Bearing 203.6
  joy.SetAxis(joy.kDefaultYChannel,  1);
  joy.SetAxis(joy.kDefaultXChannel, -0.5);
  ASSERT_NEAR(joy.GetCircularisedAxis(joy.kDefaultYChannel),  sqrt(0.8), EPS);
  ASSERT_NEAR(joy.GetCircularisedAxis(joy.kDefaultXChannel), -sqrt(0.2), EPS);

  // Bearing 243.4
  joy.SetAxis(joy.kDefaultYChannel,  0.5);
  joy.SetAxis(joy.kDefaultXChannel, -1);
  ASSERT_NEAR(joy.GetCircularisedAxis(joy.kDefaultYChannel),  sqrt(0.2), EPS);
  ASSERT_NEAR(joy.GetCircularisedAxis(joy.kDefaultXChannel), -sqrt(0.8), EPS);


  // Bearing 293.6
  joy.SetAxis(joy.kDefaultYChannel, -0.5);
  joy.SetAxis(joy.kDefaultXChannel, -1);
  ASSERT_NEAR(joy.GetCircularisedAxis(joy.kDefaultYChannel), -sqrt(0.2), EPS);
  ASSERT_NEAR(joy.GetCircularisedAxis(joy.kDefaultXChannel), -sqrt(0.8), EPS);

  // Bearing 333.4
  joy.SetAxis(joy.kDefaultYChannel, -1);
  joy.SetAxis(joy.kDefaultXChannel, -0.5);
  ASSERT_NEAR(joy.GetCircularisedAxis(joy.kDefaultYChannel), -sqrt(0.8), EPS);
  ASSERT_NEAR(joy.GetCircularisedAxis(joy.kDefaultXChannel), -sqrt(0.2), EPS);
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
