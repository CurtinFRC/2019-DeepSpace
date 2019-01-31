#include "Display.h"
#include "Capture.h"
#include "HatchProcessing.h"

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
  Process::Periodic();
  if (_capture.IsValidFrameThresh() && _capture.IsValidFrameTrack()) {
    /* cv::Mat bgrThreshInput = _capture.CopyCaptureMat();
    double bgrThreshBlue[] = {0.0, 127.0};
    double bgrThreshGreen[] = {200.0, 255.0};		//thresholding values for finding green
    double bgrThreshRed[] = {0.0, 127.0}; */
    
    _capture.CopyCaptureMat(_imgProcessing);
    _imgProcessedTrack = cv::Mat::zeros(_videoMode.height, _videoMode.width, CV_8UC3);
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
    // Filters size for Reflective Hatch
    //cv::inRange(_imgProcessing, cv::Scalar(15, 110, 100), cv::Scalar(34, 255, 255), _imgProcessedTrack);
    cv::inRange(_imgProcessing, cv::Scalar(15, 110, 100), cv::Scalar(34, 255, 255), _imgProcessing);
    cv::findContours(_imgProcessing, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_TC89_KCOS);
    //cv::findContours(_imgProcessedThresh, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_TC89_KCOS); // Is this redundant ?
    /*
    for (int i = 0; i < contours.size(); i++) {
      std::vector<cv::Point> contour = contours[i];
      cv::Rect r = cv::boundingRect(contour);
      
      double area = cv::contourArea(contour);
      if (area > 50.0) {
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
    */
    std::vector<cv::Vec3f> circles;
    cv::HoughCircles(_imgProcessing, circles, CV_HOUGH_GRADIENT, 1,
      _imgProcessing.rows/16,  // change this value to detect circles with different distances to each other
      100, 30, 1, 30 // change the last two parameters
      // (min_radius & max_radius) to detect larger circles
    );

    /// Draw the circles detected
    for( size_t i = 0; i < circles.size(); i++ )
    {
      cv::Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
      int radius = cvRound(circles[i][2]);
      // circle center
      cv::circle( _imgProcessedTrack, center, 3, cv::Scalar(0,0,255), -1, 8, 0 );
      // circle outline
      cv::circle( _imgProcessedTrack, center, radius, cv::Scalar(0,0,255), 3, 8, 0 );
    }
    /*
    /// Detect edges using Canny
    cv::Canny(_imgProcessing, _imgProcessing, hatch_thresh, hatch_thresh * 2);

    /// Find contours
    std::vector<cv::Vec4i> hierarchy;

    /// Find the convex hull object for each contour
    std::vector<std::vector<cv::Point>> hullHatch(filteredContoursHatch.size());
    for (size_t i = 0; i < filteredContoursHatch.size(); i++) {
      cv::convexHull(filteredContoursHatch[i], hullHatch[i]);
    }

    /// Draw filteredContours + hull results
    _imgProcessing = cv::Mat::zeros(_imgProcessing.size(), CV_8UC3);
    std::vector<cv::Rect> boundRectHatch( filteredContoursHatch.size() );

    for (size_t i = 0; i < filteredContoursHatch.size(); i++) {
      cv::Scalar color = cv::Scalar(rngHatch.uniform(0, 256), rngHatch.uniform(0, 256), rngHatch.uniform(0, 256));
      cv::drawContours(_imgProcessing, filteredContoursHatch, (int)i, color);
      cv::drawContours(_imgProcessing, hullHatch, (int)i, color);
    }

    for (size_t i = 0; i < filteredContoursHatch.size(); i++) {
      cv::Scalar color = cv::Scalar(rngHatch.uniform(0, 256), rngHatch.uniform(0, 256), rngHatch.uniform(0, 256));
      cv::drawContours(_imgProcessing, filteredContoursHatch, (int)i, color);
      cv::drawContours(_imgProcessedTrack, hullHatch, (int)i, color);
    }
    
    /// Find contoursBox
    /// Approximate contoursBox to polygons + get bounding rects and circles
    std::vector<std::vector<cv::Point>> hullHatch_poly(hullHatch.size());
    std::vector<cv::Point2f> centerHatch(hullHatch.size());
    std::vector<float> radiusHatch(hullHatch.size());

    for(int i = 0; i < hullHatch.size(); i++) {
      approxPolyDP(cv::Mat(hullHatch[i]), hullHatch_poly[i], 3, true);
      boundRectHatch[i] = cv::boundingRect(cv::Mat(hullHatch_poly[i]));
      cv::minEnclosingCircle((cv::Mat)hullHatch_poly[i], centerHatch[i], radiusHatch[i]);
    }
    
    
    /// Draw polygonal contour + bonding rects + circles
    for(int i = 0; i < hullHatch.size(); i++) {
      cv::Scalar color = cv::Scalar(rngHatch.uniform(0, 255), rngHatch.uniform(0,255), rngHatch.uniform(0,255));
      cv::drawContours(_imgProcessing, hullHatch_poly, i, color, 1, 8, std::vector<cv::Vec4i>(), 0, cv::Point());
      hatch_bounding_rect = cv::boundingRect(filteredContoursHatch[i]); // Find the bounding rectangle for biggest contour
      cv::rectangle(_imgProcessedTrack, boundRectHatch[i].tl(), boundRectHatch[i].br(), color, 2, 8, 0);
      cv::circle(_imgProcessedTrack, centerHatch[i], (int)radiusHatch[i], color, 2, 8, 0);
    }
    

    //_____________________Center Calcs______(Calculates the center from Border Box, And calculates X,Y Offset)_______ Ok.. it's suppose to calculate from borderbox, but not yet. using hull instead
    
    std::vector<cv::Moments> muHatch(hullHatch_poly.size()); // do we need this if we have mutex ? *
    for(int i = 0; i < hullHatch_poly.size(); i++) {
      muHatch[i] = moments(hullHatch_poly[i], false);
    }

    // get the centroid of figures.
    std::vector<cv::Point2f> mcHatch(hullHatch_poly.size());
    for(int i = 0; i < hullHatch_poly.size(); i++) {
      mcHatch[i] = cv::Point2f(muHatch[i].m10/muHatch[i].m00 , muHatch[i].m01/muHatch[i].m00);
    }

    for(int i = 0; i < hullHatch_poly.size(); i++) {
      cv::Scalar color = cv::Scalar(167,151,0); // B G R values
      cv::circle(_imgProcessedTrack, mcHatch[i], 4, color, -1, 8, 0);
    
      // offsets from centerHatch
      cv::Point centerHatch = cv::Point((mcHatch[i].x), (mcHatch[i].y));
      hatch_width_offset = hatch_width_goal - centerHatch.x;
      hatch_height_offset = hatch_height_goal - centerHatch.y;
      std::cout << "Offset From CenterHatch x,y = " << hatch_width_offset << "," << hatch_height_offset << std::endl;
    }
   */
  }
}