#include "Display.h"
#include "Process.h"
#include "Capture.h"
#include <opencv2/opencv.hpp>
#include "opencv2/objdetect.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/core/core.hpp"
#include <stdio.h>
#include <iostream>
#include <cameraserver/CameraServer.h>
#include <cscore.h>
#include <chrono>
#include <thread>

#include "devices/kinect.h"

Display::Display(std::string name, Process &process) : _name(name), _process(process), _capture(process.GetCapture()) {}

void Display::Init() {
  
  _videoMode = _capture.GetVideoMode();

  // Set up output
  _output = frc::CameraServer::GetInstance()->PutVideo(_name, _videoMode.width, _videoMode.height);
}


void Display::Periodic() {
  _process.CopyProcessedTrack(_displayMat);
  if (_capture.IsValidFrameThresh() && _capture.IsValidFrameTrack()) {
    if (_process.GetValidThresh() && _process.GetValidTrack()) {
      #ifdef __DESKTOP__
      if (_displayMat.rows > 0) {
        imshow(_process.GetProcessType(), _displayMat);
      }
      cv::waitKey(1000 / 30);
    
      #else
      _output.PutFrame(_displayMat);
      #endif
    }
  }

  else {
    std::cout << "Origin Image is Not Available" << std::endl;
  }
  std::this_thread::sleep_for(std::chrono::duration<double>(1.0 / 30));
}

