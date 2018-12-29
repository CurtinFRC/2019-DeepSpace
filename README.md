CurtinFRC 2019 Codebase
===

# Quick Commands
## Build
`./gradlew :5333:build`, `./gradlew :5663:build`, or `./gradlew :vision:build`  
Build will compile and get the code ready without deploying it. It will also run all automated tests, which is great for testing your code before it ever gets run on a robot (which also means you can build whenever)

## Deploy
`./gradlew :5333:deploy`, or `./gradlew :5663:build`  
Deploying will build your code (as above), and deploy it to the robot. You have to be connected to the robot for this to work. Just keep in mind that deploying _does not run any automated tests_.

## Simulation
`./gradlew sim5333`, or `./gradlew sim5663`  
Simulation will boot up a simulation of the robot code running on your own computer. This is used to see code running without a working robot, and includes a few simulation hooks for things like a tank drive and motor outputs.

NOTE: If you're running on Linux or MacOS, you should run `killall frcUserProgram` between calls to this. On windows, just ensure the command window that pops up is closed between runs. You can only have one simulator running at once!

## Running Vision
`./gradlew runVision`  
You can run vision on your local computer without needing a Raspberry Pi or Tinker Board to test on. As long as you have a webcam, you should be A OK.

# Projects
We break our code up into multiple projects, in order to teach concepts related to code organisation. If you're wondering where some code belongs, or where to put a new feature, here's your reference:

## [5333](5333) / [5663](5663)
This is the team code. The code in these projects should be _very minimal_, since most will be put into `common`. This code should only initialize the layout / configuration of the robot and hand off most control to `commonRobot`. This allows us to share code between the two teams.

5333 / 5663 both depend on `common`.

## [Common](common)
Common is code that is shared between both teams. It includes the bulk of our code. 
It also includes the simulation framework, as well as simulation 'wrappers' for vendor parts that don't have builds for desktop (like CTRE and NavX).
Almost all of the code should be inside of `common`, so if we move parts between the robots, or need to do quick repairs, we don't have to reinvent code that the other team has already written :)

### Where should my code go?
- Does it work on both simulation and the robot? -> `common/src/main/`
- Does it only work on the robot? -> `common/src/robot`
- Does it only work in simulation? -> `common/src/simulation`
- It is called from both simulation and the robot, but the behaviour is different: -> Headers in `common/src/main/include`, cpp files in `common/src/simulation/cpp` and `common/src/robot/cpp`
  - If you have a physical device (like a gyro), we're going to need different behaviour / logic depending on if we're in sim, vs on a real robot. In sim, we want a 'fake' gyro, but on a real robot we want to use the real gyro. This is how we split that code up! The header is the same for both, so it's callable from anyway, but the underlying behaviour is something we can make different depending on the platform.

## [Vision](vision)
Vision is the code that gets put on our vision system for the competition. Vision doesn't get deployed to the RoboRIO, but instead to a Raspberry Pi or Asus Tinkerboard.