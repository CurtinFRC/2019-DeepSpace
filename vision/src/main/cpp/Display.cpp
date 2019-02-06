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

Display::Display(Process &process) : _process(process), _capture(process.GetCapture()) {}

void Display::Init() {
  
  _videoMode = _capture.GetVideoMode();

  // Set up output
  _outputCam0 = frc::CameraServer::GetInstance()->PutVideo("USB CameraTape", _videoMode.width, _videoMode.height);
  _outputCam1 = frc::CameraServer::GetInstance()->PutVideo("USB CameraGamePeice", _videoMode.width, _videoMode.height);
}


void Display::Periodic() {
  _process.CopyProcessedTrack(_imgProcessedTrack);
  _process.CopyProcessedTrack(_imgProcessedTrackHatch);
  if (_capture.IsValidFrameThresh() && _capture.IsValidFrameTrack()) {
    if (_process.GetValidThresh() && _process.GetValidTrack()) {
      #ifdef __DESKTOP__
      if (_imgProcessedTrack.rows > 0) {
        imshow(_process.GetProcessType(), _imgProcessedTrack);
      }
      cv::waitKey(1000 / 30);
    
      #else
      _outputCam0.PutFrame(_imgProcessedTrack);
      _outputCam1.PutFrame(_imgProcessedTrackHatch);
      #endif
    }
  }

  else {
    std::cout << "Origin Image is Not Available" << std::endl;
  }
  std::this_thread::sleep_for(std::chrono::duration<double>(5.0 / 90));
}

