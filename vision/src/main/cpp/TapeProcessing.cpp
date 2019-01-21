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

}

void TapeProcessing::Periodic() {
	if (_capture.IsValidFrame()) {
		cv::Mat bgrThreshInput = _capture.GetCaptureMat();
		double bgrThreshBlue[] = {0.0, 127.0};
		double bgrThreshGreen[] = {200.0, 255.0};		//thresholding values for finding green
		double bgrThreshRed[] = {0.0, 127.0};
	}
}