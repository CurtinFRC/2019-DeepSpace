#include "Display.h"
#include "Capture.h"
#include "TapeProcessing.h"

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

void TapeProcessing::Init() {
	hsvImg = cv::Mat::zeros(_capture.GetHeight(), _capture.GetWidth(), CV_8UC3);
}

void TapeProcessing::Periodic() {
	if (_capture.IsValidFrame()) {
		cv::Mat hsvThreshInput = _capture.GetCaptureMat();
		cv::cvtColor(hsvThreshInput, hsvImg, cv::COLOR_RGB2HSV);
	}
}