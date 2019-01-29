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
#include <cameraserver/CameraServer.h>
#include <cscore.h>
#include <chrono>
#include <thread>

#include "devices/kinect.h"

Display::Display(Process &process) : _process(process), _capture(process.GetCapture()) {}

void Display::Init() {
  
  _videoMode = _capture.GetVideoMode();

  // Set up output
  _output = frc::CameraServer::GetInstance()->PutVideo("USB Camera", _videoMode.width, _videoMode.height);
}

void Display::Periodic() {
  // Capture &capture = _process.GetCapture();
  _process.CopyProcessedTrack(_imgProcessedTrack);
  // _process.CopyProcessedThresh(_imgProcessedThresh);
  // _capture.CopyCaptureMat(_imgOriginal);
  // if (_capture.IsValidFrameThresh() && _capture.IsValidFrameTrack()) {
  if (_process.GetValidThresh() && _process.GetValidTrack()) {
#ifdef __DESKTOP__
      //imshow("OutputOrigin", _imgOriginal);

      imshow(_process.GetProcessType(), _imgProcessedTrack);

      // imshow("OutputBallThresh", _imgBallThresh);
      // imshow("OutputBallTrack", _imgBallTrack);
      // cv::waitKey(500 / 30);
      // imshow("OutputHatchThresh", _imgHatchThresh);
      // imshow("OutputHatchTrack", _imgHatchTrack);
      // imshow("OutputTapeThresh", _imgTapeThresh);
      // imshow("OutputTapeTrack", _imgTapeTrack);
      cv::waitKey(1000 / 30);
    
#else
      // Grab a frame. If it's not an error (!= 0), convert it to grayscale and send it to the dashboard.
      _output.PutFrame(_imgProcessedTrack);
#endif
    // }
		std::cout << "Origin Image Processed" << std::endl;
    // other output if needed
  }
  else {
    std::cout << "Origin Image is Not Available" << std::endl;
  }
  std::this_thread::sleep_for(std::chrono::duration<double>(0.1 / 90));
}

