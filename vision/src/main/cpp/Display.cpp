#include "Display.h"

#include <opencv2/opencv.hpp>
#include "opencv2/objdetect.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/core/core.hpp"
#include <stdio.h>
#include <iostream>
#include <windows.h>

#include <cameraserver/CameraServer.h>
#include <cscore.h>

#include "devices/kinect.h"

Display::Display(Process &process) : _process(process) {}

void Display::Init() {
  Capture &capture = _process.GetCapture();
  _videoMode = capture.GetVideoMode();

  // Set up output
  cs::CvSource _output = frc::CameraServer::GetInstance()->PutVideo("USB Camera", _videoMode.width, _videoMode.height);
}

void Display::Periodic() {
  Capture &capture = _process.GetCapture();
  _process.CopyImgTrack(_imgTrack);
  if(capture.IsValidFrame()) {
    imshow("Output", _imgTrack);
    // Grab a frame. If it's not an error (!= 0), convert it to grayscale and send it to the dashboard.
    _output.PutFrame(_imgTrack);
		std::cout << "Origin Image Processed" << std::endl;
    // other output if needed
  }
  else {
    std::cout << "Origin Image is Not Available" << std::endl;
  }
}

