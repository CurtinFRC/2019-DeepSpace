#include "Display.h"
#include "Capture.h"
#include "HatchProcessing.h"
// #include "ProcessController.h"

#include <opencv2/opencv.hpp>
#include "opencv2/objdetect.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/core/core.hpp"
#include <stdio.h>
#include <iostream>

#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"

#include "networktables/NetworkTableInstance.h"

#include <cameraserver/CameraServer.h>
#include <cscore.h>

#include "devices/kinect.h"

cv::RNG rngHatch(12345);
cv::Rect hatchBoundingRect;
int hatchThresh = 100;
float hatchHeightOffset;
float hatchWidthOffset;
float hatchWidthGoal = 320;
float hatchHeightGoal = 240;
std::string HatchDistance = "Sumthin";

HatchProcessing::HatchProcessing(Capture &capture) : _capture(capture) {}

void HatchProcessing::Init() {

  auto inst = nt::NetworkTableInstance::GetDefault();
  auto visionTable = inst.GetTable("VisionTracking");
  auto table = visionTable->GetSubTable("HatchTracking");
  HatchDistanceEntry = table->GetEntry("Hatch Distance");
  HatchXOffsetEntry = table->GetEntry("Hatch X Offset");
  HatchYOffsetEntry = table->GetEntry("Hatch Y Offset");

  _capture.SetExposure(40);
}

void HatchProcessing::Periodic() {
  if (_capture.IsValidFrame()) {
    _capture.CopyCaptureMat(_imgProcessing);
    _imgProcessing.copyTo(_imgProcessedTrack);
    cv::cvtColor(_imgProcessing, _imgProcessing, cv::COLOR_BGR2HSV);

    // Contours Blocks (Draws a convex shell over the thresholded image.)

    std::vector<std::vector<cv::Point>> contours;
    std::vector<std::vector<cv::Point>> filteredContoursHatch;
    std::vector<std::vector<cv::Point>> filteredHullsHatch;
    std::vector<cv::Rect> ir_rects;
    int active_contour;
    bool show_window;

    double largestArea = 0.0;
    active_contour = -1;
    // cv::inRange(_imgProcessing, cv::Scalar(15, 100, 100), cv::Scalar(34, 255, 255), _imgProcessedTrack); // <- Debug Code
    cv::inRange(_imgProcessing, cv::Scalar(15, 100, 100), cv::Scalar(34, 255, 255), _imgProcessing);
    
    cv::findContours(_imgProcessing, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_TC89_KCOS);
    
    for (int i = 0; i < contours.size(); i++) {
      std::vector<cv::Point> contour = contours[i];
      cv::Rect r = cv::boundingRect(contour);
      
      double area = cv::contourArea(contour);
      if (area > 450.0) {
        std::vector<cv::Point> hull;
        cv::convexHull(contour, hull);
        double solidity = 10 * area / contourArea(hull);

        if (solidity > 2) {
          if (area > largestArea) {
            largestArea = area;
            active_contour = filteredContoursHatch.size();
          }
          filteredContoursHatch.push_back(contour);
          filteredHullsHatch.push_back(hull);
          ir_rects.push_back(r);
        }
      }
    }

    /// Detect edges using Canny
    cv::Canny(_imgProcessing, _imgProcessing, hatchThresh, hatchThresh * 2);

    /// Find contours
    std::vector<cv::Vec4i> hierarchy;

    /// Find the convex hull object for each contour
    std::vector<std::vector<cv::Point>> hull(filteredContoursHatch.size());
    for (size_t i = 0; i < filteredContoursHatch.size(); i++) {
      cv::convexHull(filteredContoursHatch[i], hull[i]);
    }
  
    /// Draw filteredContours + hull results
    _imgProcessing = cv::Mat::zeros(_imgProcessing.size(), CV_8UC3);
    std::vector<cv::Rect> boundRectHatch( filteredContoursHatch.size() );

    for (size_t i = 0; i < filteredContoursHatch.size(); i++) {
      cv::Scalar color = cv::Scalar(rngHatch.uniform(0, 256), rngHatch.uniform(0, 256), rngHatch.uniform(0, 256));
      cv::drawContours(_imgProcessing, filteredContoursHatch, (int)i, color, -1);
      cv::drawContours(_imgProcessing, hull, (int)i, color, -1);
    }
    
    /// Find contoursBox
    /// Approximate contoursBox to polygons + get bounding rects and circles
    std::vector<std::vector<cv::Point>> hullHatchPoly(hull.size());
    std::vector<cv::Point2f> centerHatch(hull.size());
    std::vector<float> radiusHatch(hull.size());

    for(int i = 0; i < hull.size(); i++) {
      approxPolyDP(cv::Mat(hull[i]), hullHatchPoly[i], 3, true);
      boundRectHatch[i] = cv::boundingRect(cv::Mat(hullHatchPoly[i]));
      cv::minEnclosingCircle((cv::Mat)hullHatchPoly[i], centerHatch[i], radiusHatch[i]);
    }
  
    /// Draw polygonal contour + bonding rects + circles
    for(int i = 0; i < hull.size(); i++) {
      cv::Scalar color = cv::Scalar(rngHatch.uniform(0, 255), rngHatch.uniform(0,255), rngHatch.uniform(0,255));
      cv::drawContours(_imgProcessing, hullHatchPoly, i, color, 1, 8, std::vector<cv::Vec4i>(), 0, cv::Point());
      hatchBoundingRect = cv::boundingRect(filteredContoursHatch[i]); // Find the bounding rectangle for biggest contour
      // _imgProcessedTrack = cv::Mat::zeros(_videoMode.height, _videoMode.width, CV_8UC3);
      cv::rectangle(_imgProcessedTrack, boundRectHatch[i].tl(), boundRectHatch[i].br(), color, 2, 8, 0);
      cv::circle(_imgProcessedTrack, centerHatch[i], (int)radiusHatch[i], color, 2, 8, 0);
    }
    
    
    //_____________________Center Calcs______(Calculates the center from Border Box, And calculates X,Y Offset)_______ Ok.. it's suppose to calculate from borderbox, but not yet. using hull instead
    
    std::vector<cv::Moments> muHatch(hullHatchPoly.size()); // do we need this if we have mutex ? *
    for(int i = 0; i < hullHatchPoly.size(); i++) {
      muHatch[i] = moments(hullHatchPoly[i], false);
    }

    // get the centroid of figures.
    std::vector<cv::Point2f> mcHatch(hullHatchPoly.size());
    for(int i = 0; i < hullHatchPoly.size(); i++) {
      mcHatch[i] = cv::Point2f(muHatch[i].m10/muHatch[i].m00 , muHatch[i].m01/muHatch[i].m00);
    }

    for(int i = 0; i < hullHatchPoly.size(); i++) {
      cv::Scalar color = cv::Scalar(167,151,0); // B G R values
      cv::circle(_imgProcessedTrack, mcHatch[i], 4, color, -1, 8, 0);
    
      // offsets from centerHatch
      cv::Point centerHatch = cv::Point((mcHatch[i].x), (mcHatch[i].y));
      hatchWidthOffset = hatchWidthGoal - centerHatch.x;
      hatchHeightOffset = hatchHeightGoal - centerHatch.y;
      std::cout << "Offset From CenterHatch x,y = " << hatchWidthOffset << "," << hatchHeightOffset << std::endl;
      HatchDistanceEntry.SetString(HatchDistance);
      HatchXOffsetEntry.SetDouble(hatchWidthOffset);
      HatchYOffsetEntry.SetDouble(hatchHeightOffset);
      std::stringstream offsetY;	offsetY << hatchHeightOffset;
      std::stringstream offsetX;	offsetX << hatchWidthOffset;
      cv::putText(_imgProcessedTrack, "xy(" + offsetX.str() + "," + offsetY.str() + ")", mcHatch[i] + cv::Point2f(-25,25), cv::FONT_HERSHEY_COMPLEX_SMALL, 1, cv::Scalar(255,0,255)); //text with distance and angle on target
    }
    
  }
  
}

void HatchProcessing::CopyProcessedTrack(cv::Mat &imgProcessedTrack) {
  std::lock_guard<std::mutex> lock(_classMutex);
  _imgProcessedTrack.copyTo(imgProcessedTrack);
}

void HatchProcessing::GetDisplayMat(cv::Mat &displayMat) {
  CopyProcessedTrack(displayMat);
}

cv::Size HatchProcessing::GetDisplaySize() {
  return _capture.GetDisplaySize();
}