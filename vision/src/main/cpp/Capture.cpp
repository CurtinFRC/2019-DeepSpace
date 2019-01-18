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
#include <networktables/NetworkTableInstance.h>
#include <cscore.h>

#include "devices/kinect.h"

using namespace cv;
using namespace std;

void Capture::Init() {
  _sink.SetSource(_cam);
  _cam.SetExposureManual(-100);

  // The camera defaults to a lower resolution, but you can choose any compatible resolution here.
  _cam.SetResolution(640, 480);

  auto video_mode = _cam.GetVideoMode();
  std::cout << "Width: " << video_mode.width << " Height: " << video_mode.height << std::endl;

  _captureMat = cv::Mat::zeros(video_mode.height, video_mode.width, CV_8UC3);
}

void Capture::Periodic() {
  _isValid = _sink.GrabFrame(_captureMat) == 0;
}

cv::Mat &Capture::GetCaptureMat() {
  return _captureMat;
}

bool Capture::IsValidFrame() {
  return _isValid;
}