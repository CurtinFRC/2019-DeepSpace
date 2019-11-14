#include "Process.h"

#include <stdio.h>
#include <iostream>

#include <cameraserver/CameraServer.h>
#include <cscore.h>

#include "networktables/NetworkTableInstance.h"
#include "devices/kinect.h"

using namespace cv;
using namespace std;

//Set _capture as a Capture object reference
Process::Process(Capture &capture) : _capture(capture) {}

Capture &Process::GetCapture() {
  return _capture;
}

std::string Process::GetProcessType() {
  return "";
}

void Process::GetDisplayMat(cv::Mat &displayMat) {
  
}
cv::Size Process::GetDisplaySize() {
  return _capture.GetDisplaySize();
}

void Process::Init() {
}

void Process::Periodic() {
  
}
