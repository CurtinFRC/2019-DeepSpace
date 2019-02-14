Imaging a new Vision Coprocessor
===

# Getting started
### 1. Grab the image:
  - [Tinkerboard Image](https://dlcdnets.asus.com/pub/ASUS/mb/Linux/Tinker_Board_S/20181023-tinker-board-linaro-stretch-alip-v2.0.8.img.zip)
    - Make sure you use version 2.0.8! v1.8 doesn't work with some USB cameras (nor the Kinect) due to a kernel problem.
  - [Raspberry Pi](https://downloads.raspberrypi.org/raspbian_lite_latest)
### 2. Flash the image onto a microsd card using etcher: https://etcher.io/
  - You will need to grab the .img file from the zip. The .img file is what you give to etcher.
  - After complete, insert the microSD card and startup the coprocessor. It may take a minute or two.
### 3. SSH into the Coprocessor
  - On Mac/Linux, use `ssh username@hostname`
  - On Windows, download [puTTY](https://the.earth.li/~sgtatham/putty/latest/w64/putty-64bit-0.70-installer.msi) and use that.
  - Tinkerboard:
    - Hostname: `tinkerboard`, Username: `linaro`, Password: `linaro`
  - Raspberry Pi:
    - Hostname: `raspberrypi`, Username: `pi`, Password: `raspberry`
### 4. Run the following command:
  - `wget --no-check-certificate -qO - https://github.com/CurtinFRC/2019-DeepSpace/blob/master/vision/bootstrap.sh?raw=1 | bash`
### 5. Your Coprocessor will now restart
### 6. Run `./gradlew :vision:deploy` (`./gradlew :vision:deploy -Praspberry` for the Raspberry Pi) to deploy your code!


### Competition Code ###

  - Variables are passed over via NetworkTables to the roborio and in turn can be accessed via shuffleboard
  - These Tables are located in the `VisionTracking` table, and include `HatchTracking` & `TapeTracking`

 # Variables For Tables -
  - The `TapeTracking` Table Located in `TapeProcessing.cpp` includes the           following entries:
  - `Distance`, `Angle` and `Target`
  - `Distance` Includes the Distance in meters
  - `Angle` Provides the Angle in Degrees
  - And `Target` Gives the X and Y offset in pixles

  - The `HatchTracking` Table Located in `HatchProcessing.cpp`includes the          following entries:
  - `Hatch X offset` Which gives the offset from the X axis
  - `Hatch Y offset` Which gives the offset from the Y axis
  - Distance can be provided as well via the `Hatch Distance` But is currently      unavailable, and realisticly is not requried
  
 # Debug Code

  - Normal Debug should be done before all else, but to take out the bulk           processing of the code both `Hatch` & `Tape` processing have a debug line in    `HatchProcessing.cpp/TapeProcessing.cpp on line 67 & 43` which when put in      should give a threshold output, This way you should be able to see if the       program is getting images and processing correctly.

 # Possible Code Implementations
 - The Code still technicaly has ball processing which gives an X and Y offset,
 - But it's not tested and is Not really a priority. You will need to uncomment     the code in main, and set up NetworkTables for it. It can be done... But it's    not recommended

 # Imaging

 - In extreme emergencies when all SD cards have been used in one day and you       don't have access to ethernet or monitors ect. And you need fresh files, you     can get them off of this repo:
 `https://github.com/CJBuchel/CurtinFRC_TinkerBoard_Image`
 or clone link via
 `https://github.com/CJBuchel/CurtinFRC_TinkerBoard_Image.git`
  Follow the instuctions in the git repo discription and copy those files over to the SD card and deploy your code to it. Jank i know, but it's emergency uses only i swear :)