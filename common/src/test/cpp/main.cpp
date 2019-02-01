#include "gtest/gtest.h"

#include <frc/Timer.h>
#include <frc/DriverStation.h>

#include <iostream>

int main(int argc, char** argv) {
  // Fixes wpilibsuite#1550
  frc::DriverStation::GetInstance();
  frc::Wait(0.25);

  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
