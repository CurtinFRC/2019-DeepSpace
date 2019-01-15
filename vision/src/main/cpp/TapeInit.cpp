#include "TapeInit.h"
#include "Run.h"
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
using namespace std;\

void curtin_frc_vision::TapeInit()  {

  RNG rng(12345);
  Rect bounding_rect;
  int thresh = 100;
  float height_offset;
  float width_offset;
  float width_goal;
  float height_goal;

  vector<bool> lefts;
  vector<bool> rights;
  vector<float> angles;
  vector<float> heights;
  vector<cv::Point2f> centres;
  vector<cv::Point2f> targets;
}