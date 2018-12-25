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

## 5333 / 5663
This is the team code. The code in these projects should be _very minimal_, since most will be put into `commonRobot`. This code should only initialize the layout / configuration of the robot and hand off most control to `commonRobot`. This allows us to share code between the two teams.

5333 / 5663 both depend on `commonRobot`.

## Common Robot
Common Robot contains shared code that interfaces directly with WPILib (which provides things like `frc::Spark`, `frc::XboxController`, etc). Most of your code should live here, things like classes to control elevators and drivetrains, or code to flash pretty lights. The code should be designed such that both teams can use it. 

CommonRobot depends on `common`.

## Common
Common is shared code that _doesn't_ depend on WPILib. The code in here is pretty limited, but if you write some code that doesn't necessarily apply to the robot (for example, could be used in vision _and_ the robot, or in future projects) it should go here. 

## Vision
Vision is the code that gets put on our vision system for the competition. Both teams use the same vision system, and so `vision` depends on `common`. Vision doesn't get deployed to the RoboRIO, but instead to a Raspberry Pi or Asus Tinkerboard.

Vision depends on `common`.

## Simulation (special)
Simulation is special, and is only used when running on desktop. This is what provides all the nice windows that represent the various systems on the robot when you run `./gradlew sim5333` or `./gradlew sim5663`. You can't directly access simulation from any other projects - it's automatically handled. See Jaci for more info on how this works if you're really curious.

`Common` is both accessible by `Simulation`. `Common` allow the robot program to 'talk' to simulation to tell it what windows to pop up, without directly accessing simulation (so we can ignore it when we go on the actual robot!).