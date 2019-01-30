#include "Display.h"
#include "Capture.h"
#include "HatchProcessing.h"

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

#include "devices/kinect.h"

cv::RNG rngHatch(12345);
cv::Rect hatch_bounding_rect;
int hatch_thresh = 100;
float hatch_height_offset;
float hatch_width_offset;
float hatch_width_goal = 320;
float hatch_height_goal = 240;

void HatchProcessing::Init() {
  Process::Init();
  processType = "HatchProcessing";
}

void HatchProcessing::Periodic() {
  std::lock_guard<std::mutex> lock(_classMutex);
  if (_capture.IsValidFrame()) {
    /* cv::Mat bgrThreshInput = _capture.CopyCaptureMat();
    double bgrThreshBlue[] = {0.0, 127.0};
    double bgrThreshGreen[] = {200.0, 255.0};		//thresholding values for finding green
    double bgrThreshRed[] = {0.0, 127.0}; */
    
    _capture.CopyCaptureMat(_imgOriginal);
    cv::cvtColor(_imgOriginal, _imgHatchThresh, cv::COLOR_RGB2HSV);
    cv::cvtColor(_imgOriginal, _imgProcessed, cv::COLOR_RGB2HSV);
    std::cout << "Origin Image Found For Hatch" << std::endl;
    // Threshold the HSV image, keep only the green pixels (RetroBall)

    // Contours Blocks (Draws a convex shell over the thresholded image.)

    std::vector<std::vector<cv::Point>> contours;
    std::vector<std::vector<cv::Point>> filteredContoursBall;
    std::vector<std::vector<cv::Point>> filteredHullsBall;
    std::vector<cv::Rect> ir_rects;
    int active_contour;
    cv::Scalar hsl_low, hsl_high;
    bool show_window;

    double largestArea = 0.0;
    active_contour = -1;
    // Filters size for Reflective Ball
    cv::inRange(_imgProcessed, cv::Scalar(0, 100, 100), cv::Scalar(100, 255, 255), _imgProcessed);
    cv::inRange(_imgProcessed, cv::Scalar(0, 100, 100), cv::Scalar(100, 255, 255), _imgHatchThresh);
    cv::findContours(_imgProcessed, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_TC89_KCOS);
    cv::findContours(_imgHatchThresh, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_TC89_KCOS);

    for (int i = 0; i < contours.size(); i++) {
      std::vector<cv::Point> contour = contours[i];
      cv::Rect r = cv::boundingRect(contour);
      
      double area = cv::contourArea(contour);
      if (area > 300.0) {
        std::vector<cv::Point> hull;
        cv::convexHull(contour, hull);
        double solidity = 100 * area / contourArea(hull);

        if (solidity < 60.0) {
          if (area > largestArea) {
            largestArea = area;
            active_contour = filteredContoursBall.size();
          }
          filteredContoursBall.push_back(contour);
          filteredHullsBall.push_back(hull);
          ir_rects.push_back(r);
        }
      }
    }

    /// Detect edges using Canny
    cv::Canny(_imgProcessed, _imgProcessed, hatch_thresh, hatch_thresh * 2);

    /// Find contours
    std::vector<cv::Vec4i> hierarchy;

    /// Find the convex hull object for each contour
    std::vector<std::vector<cv::Point>> hullBall(filteredContoursBall.size());
    for (size_t i = 0; i < filteredContoursBall.size(); i++) {
      cv::convexHull(filteredContoursBall[i], hullBall[i]);
    }

    /// Draw filteredContours + hull results
    _imgProcessed = cv::Mat::zeros(_imgProcessed.size(), CV_8UC3);
    std::vector<cv::Rect> boundRectBall( filteredContoursBall.size() );

    for (size_t i = 0; i < filteredContoursBall.size(); i++) {
      cv::Scalar color = cv::Scalar(rngHatch.uniform(0, 256), rngHatch.uniform(0, 256), rngHatch.uniform(0, 256));
      cv::drawContours(_imgProcessed, filteredContoursBall, (int)i, color);
      cv::drawContours(_imgProcessed, hullBall, (int)i, color);
    }

    for (size_t i = 0; i < filteredContoursBall.size(); i++) {
      cv::Scalar color = cv::Scalar(rngHatch.uniform(0, 256), rngHatch.uniform(0, 256), rngHatch.uniform(0, 256));
      cv::drawContours(_imgProcessed, filteredContoursBall, (int)i, color);
      cv::drawContours(_imgProcessed, hullBall, (int)i, color);
    }

    /// Find contoursBox
    /// Approximate contoursBox to polygons + get bounding rects and circles
    std::vector<std::vector<cv::Point>> hullBall_poly(hullBall.size());
    std::vector<cv::Point2f> centerBall(hullBall.size());
    std::vector<float> radiusBall(hullBall.size());

    for(int i = 0; i < hullBall.size(); i++) {
      approxPolyDP(cv::Mat(hullBall[i]), hullBall_poly[i], 3, true);
      boundRectBall[i] = cv::boundingRect(cv::Mat(hullBall_poly[i]));
      cv::minEnclosingCircle((cv::Mat)hullBall_poly[i], centerBall[i], radiusBall[i]);
    }


    /// Draw polygonal contour + bonding rects + circles
    for(int i = 0; i < hullBall.size(); i++) {
      cv::Scalar color = cv::Scalar(rngHatch.uniform(0, 255), rngHatch.uniform(0,255), rngHatch.uniform(0,255));
      cv::drawContours(_imgProcessed, hullBall_poly, i, color, 1, 8, std::vector<cv::Vec4i>(), 0, cv::Point());
      hatch_bounding_rect = cv::boundingRect(filteredContoursBall[i]); // Find the bounding rectangle for biggest contour
      cv::rectangle(_imgProcessed, boundRectBall[i].tl(), boundRectBall[i].br(), color, 2, 8, 0);
      cv::circle(_imgProcessed, centerBall[i], (int)radiusBall[i], color, 2, 8, 0);
    }


    //_____________________Center Calcs______(Calculates the center from Border Box, And calculates X,Y Offset)_______ Ok.. it's suppose to calculate from borderbox, but not yet. using hull instead

    std::vector<cv::Moments> muBall(hullBall_poly.size()); // do we need this if we have mutex ? *
    for(int i = 0; i < hullBall_poly.size(); i++) {
      muBall[i] = moments(hullBall_poly[i], false);
    }

    // get the centroid of figures.
    std::vector<cv::Point2f> mcBall(hullBall_poly.size());
    for(int i = 0; i < hullBall_poly.size(); i++) {
      mcBall[i] = cv::Point2f(muBall[i].m10/muBall[i].m00 , muBall[i].m01/muBall[i].m00);
    }

    for(int i = 0; i < hullBall_poly.size(); i++) {
      cv::Scalar color = cv::Scalar(167,151,0); // B G R values
      cv::circle(_imgProcessed, mcBall[i], 4, color, -1, 8, 0);

      // offsets from centerBall
      cv::Point centerHatch = cv::Point((mcBall[i].x), (mcBall[i].y));
      hatch_width_offset = hatch_width_goal - centerHatch.x;
      hatch_height_offset = hatch_height_goal - centerHatch.y;
      std::cout << "Offset From CenterBall x,y =" << hatch_height_offset << "," << hatch_width_offset << std::endl;
    }
  }
}