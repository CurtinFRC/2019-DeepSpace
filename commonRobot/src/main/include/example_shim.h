#pragma once

// This is an example 'shim'
// This include file is the same for desktop simulation and on the robot, but the actual logic
// in the .cpp files is different. See src/main/sim and src/main/robot to see how the implementations
// differ.
// If the logic is the same for both, it can go in src/main/cpp.

// This is really useful since some parts we use (like the Talon SRX and NavX) only have code for the RoboRIO,
// and don't have any simulation code. Therefore, we can write our own 'wrapper' (abstraction layer) in order
// to provide simulation support!

#include <string>

// You can see these in action by running `./gradlew :commonRobot:build`, but you'll only see the desktop ones.
// You'll just have to take my word for it that it works correctly on the robot.

class example_shim {
 public:
  // This changes depending on whether we're in simulation (src/main/sim), or on an actual robot (src/main/robot)
  std::string who_am_i();
  // This is the same in both cases! (src/main/cpp)
  std::string who_are_you();
};