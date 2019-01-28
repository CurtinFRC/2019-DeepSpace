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
  cs::CvSource _output = frc::CameraServer::GetInstance()->PutVideo("USB Camera", _videoMode.width, _videoMode.height);
}

void Display::Periodic() {
  //Capture &capture = _process.GetCapture();
  _process.CopyImgTrack(_imgTrack);
  _process.CopyImgOriginal(_imgOriginal);
  _process.CopyImgThresh(_imgThresh);
  if (_capture.IsValidFrame()) {
#ifdef __DESKTOP__
    imshow("OutputTrack", _imgTrack);
    imshow("OutputOrigin", _imgOriginal);
    imshow("OutputThresh", _imgThresh);
    cv::waitKey(1000 / 30);
#else
    // Grab a frame. If it's not an error (!= 0), convert it to grayscale and send it to the dashboard.
    _output.PutFrame(_imgOriginal);
    //_output.PutFrame(_imgTrack);
    //_output.PutFrame(_imgThresh);
#endif
		std::cout << "Origin Image Processed" << std::endl;
    // other output if needed
  } else {
    std::cout << "Origin Image is Not Available" << std::endl;
  }
  std::this_thread::sleep_for(std::chrono::duration<double>(1.0 / 90));
}

