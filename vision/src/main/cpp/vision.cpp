#include "vision.h"

#include <iostream>

#include <opencv2/opencv.hpp>

#include <cameraserver/CameraServer.h>
#include <cscore.h>

// This is the main entrypoint into the CurtinFRC Vision Program!
void curtin_frc_vision::run() {
  // This is just a demonstration so you can see how this kind of code works. You'll be replacing this
  // with our actual vision tracking software!
  // You can view the vision output with Shuffleboard. Launch with `./gradlew :vision:ShuffleBoard`

  // This creates a webcam on USB, and dumps it into a sink. The sink allows us to access the image with sink.GrabFrame
  cs::UsbCamera cam{"USBCam", 0};
  cs::CvSink sink{"USB"};
  sink.SetSource(cam);

  // The camera defaults to a lower resolution, but you can choose any compatible resolution here.
  cam.SetResolution(640, 480);

  auto video_mode = cam.GetVideoMode();
  std::cout << "Width: " << video_mode.width << " Height: " << video_mode.height << std::endl;

  // This lets us see the camera output on the robot dashboard. We give it a name, and a width and height.
  cs::CvSource output = frc::CameraServer::GetInstance()->PutVideo("USB Camera", video_mode.width, video_mode.height);

  // The capMat is what comes from the camera, and the outMat is what goes to the dashboard. Note: 
  // the height - width order is reversed here (height first, width second), unlike other parts.
  cv::Mat capMat{video_mode.height, video_mode.width, CV_8UC3};
  cv::Mat outMat{video_mode.height, video_mode.width, CV_8UC1};

  while (true) {
    // Grab a frame. If it's not an error (!= 0), convert it to grayscale and send it to the dashboard.
    if (sink.GrabFrame(capMat) != 0) {
      cv::cvtColor(capMat, outMat, cv::COLOR_RGB2GRAY);
      output.PutFrame(outMat);
    }
  }
}