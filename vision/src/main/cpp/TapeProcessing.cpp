#include "Display.h"
#include "Capture.h"
#include "TapeProcessing.h"
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

#include "networktables/NetworkTable.h"
#include "networktables/NetworkTableEntry.h"
#include "networktables/NetworkTableInstance.h"

#include <cameraserver/CameraServer.h>
#include <cscore.h>

#include "devices/kinect.h"

cv::RNG rngTape(12345);

void TapeProcessing::Init() {
	Process::Init();
  processType = "TapeProcessing";

  auto inst = nt::NetworkTableInstance::GetDefault();
  auto table = inst.GetTable("TapeTable");
  TapeDistanceEntry = table->GetEntry("Distance");
  TapeAngleEntry = table->GetEntry("Angle");
  TapeTargetEntry = table->GetEntry("Target");
}

void TapeProcessing::Periodic() {
  Process::Periodic();
	if (_capture.IsValidFrameThresh() && _capture.IsValidFrameTrack()) {

    //_capture.CopyCaptureMat(_imgProcessedThresh);
    _capture.CopyCaptureMat(_imgProcessing);
		cv::cvtColor(_imgProcessing, _imgProcessing, cv::COLOR_BGR2HSV);
    //cv::cvtColor(_imgProcessedThresh, _imgProcessedThresh, cv::COLOR_BGR2HSV);
    //cv::inRange(_imgProcessing, cv::Scalar(40, 0, 75), cv::Scalar(75, 255, 125), _imgProcessedTrack);
    cv::inRange(_imgProcessing, cv::Scalar(40, 0, 75), cv::Scalar(75, 255, 125), _imgProcessing);
    cv::findContours(_imgProcessing, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_TC89_KCOS);

    

    filteredContours.clear();
		for (int i = 0; i < contours.size(); i++) {
			if (cv::contourArea(contours[i]) > 20)
				filteredContours.push_back(contours[i]);
		}
  
    //Get RotatedRectangles 
    centres.clear(); //clear the vectors
    heights.clear();
    lefts.clear();
    rights.clear();
    cv::Scalar blue = cv::Scalar(255, 0, 0);
    cv::Scalar green = cv::Scalar(0, 255, 0);
    _imgProcessedTrack = cv::Mat::zeros(_videoMode.height, _videoMode.width, CV_8UC3);

    for (int i = 0; i < filteredContours.size(); i++) {
      cv::drawContours(_imgProcessedTrack, filteredContours, (int)i, blue);
      
      cv::RotatedRect rotatedRect = cv::minAreaRect(filteredContours[i]);

      cv::Point2f centre = rotatedRect.center;

      cv::Point2f rectPoints[4];
      rotatedRect.points(rectPoints);

      float angle;

      cv::Point2f edge1 = cv::Vec2f(rectPoints[1].x, rectPoints[1].y) - cv::Vec2f(rectPoints[0].x, rectPoints[0].y);
      cv::Point2f edge2 = cv::Vec2f(rectPoints[2].x, rectPoints[2].y) - cv::Vec2f(rectPoints[1].x, rectPoints[1].y);

      cv::Point2f usedEdge = edge1;
      if(cv::norm(edge2) > cv::norm(edge1)) {
        usedEdge = edge2;
      }

      cv::Point2f reference = cv::Vec2f(1,0); // horizontal edge

      angle = 180.0f/CV_PI * acos((reference.x * usedEdge.x + reference.y * usedEdge.y) / (cv::norm(reference) * cv::norm(usedEdge)));

      float min = rectPoints[0].y;
      float max = rectPoints[0].y;

      for (int j = 1; j < 4; j++) { //find the minimum and maximum y-values of each rectangle
        if (rectPoints[j].y > max) {
          max = rectPoints[j].y;
        }
        if (rectPoints[j].y < min) {
          min = rectPoints[j].y;
        }
      }

      float height = max - min; //get the height of each rectangle

      centres.push_back(centre);
      heights.push_back(height);

      if (angle > 95 && angle < 125) { //angle range for right classification
        rights.push_back(true);
        lefts.push_back(false);
      } else if (angle < 85 && angle > 55) { //angle range for left classification
        rights.push_back(false);
        lefts.push_back(true);
      } else {
        rights.push_back(false);
        lefts.push_back(false);
      }
    }
  
    targets.clear();
    angles.clear();
    distances.clear();

    for (int i = 0; i < filteredContours.size(); i++) {
      int leftmost = -1;
      float leftPos = 640;
      if (lefts[i]) { //checks if current iteration is a left
        for (int j = 0; j < filteredContours.size(); j++) {
          if (rights[j] && centres[j].x < leftPos && centres[j].x > centres[i].x) { //checks if nested iteration is a right and left of the last checked one
            leftmost = j;
            leftPos = centres[j].x;
          }
        }

        if (leftmost > -1) {
          targets.push_back((centres[i]+centres[leftmost]) / 2); //adds the Points2f position of each target to a vector
          distances.push_back(184 / (heights[i] + heights[leftmost])); //adds the estimated distance to each target. Calibrate by changing the number.
          float widthAdjust = 0.0058 * distances[distances.size() - 1] * abs(centres[i].x - centres[leftmost].x); //Calibrate distance, then adjust the first number until robot facing target gives 0 degrees.
          if (widthAdjust > 1.0) {
            widthAdjust = 1.0;
          }
          try { //shoot, acos can throw an error if I've missed an edge-case, and I'm too tired to look for one.
            angles.push_back(heights[leftmost] > heights[i] ? 180.0f/CV_PI * acos(widthAdjust) : -180.0f/CV_PI * acos(widthAdjust));
          } catch (...) {
            angles.push_back(0);
          }
        }
      }
    }

    int centred = -1;
    int closeX = _videoMode.width;
    for (int i = 0; i < targets.size(); i++) { 
      if (abs(targets[i].x - _videoMode.width / 2) < closeX) {
        closeX = abs(targets[i].x - _videoMode.width / 2);
        centred = i;
      }
    }
  
    for (int i = 0; i < targets.size(); i++) {
      std::stringstream dis;	dis << distances[i];
      std::stringstream ang;	ang << angles[i];
      if (i == centred) {
        cv::rectangle(_imgProcessedTrack, targets[i] + cv::Point2f(-6,-6), targets[i] + cv::Point2f(6,6), green, 2); //draw small rectangle on target locations
      } else {
        cv::rectangle(_imgProcessedTrack, targets[i] + cv::Point2f(-6,-6), targets[i] + cv::Point2f(6,6), blue, 2); //draw small rectangle on target locations
      }
      cv::putText(_imgProcessedTrack, dis.str() + "m, " + ang.str() + "deg", targets[i] + cv::Point2f(-25,25), cv::FONT_HERSHEY_COMPLEX_SMALL, 1, cv::Scalar(255,0,255)); //text with distance and angle on target
      TapeDistanceEntry.SetDouble(distances[i]);
      TapeAngleEntry.SetDouble(angles[i]);
      TapeTargetEntry.SetDouble(targets[i].x);
    }
  }
}