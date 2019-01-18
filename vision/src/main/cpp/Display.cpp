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

void Display::Init() {
	_capture = _process.GetCapture(); //get Capture reference through process
}

Display::Display(Process &process) : _process(process) {};

void Display::Periodic() {

    if (sink.GrabFrame(imgHSV) != 0) {

		#ifdef __DESKTOP__
		imshow("Original", imgOriginal); //Shows the original image
		#endif

		output.PutFrame(imgHSV);
	}


	if (waitKey(30) == 27) //wait for 'esc' key press for 30ms. If 'esc' key is pressed, break loop
	{
		cout << "esc key is pressed by user" << endl;
		//break;
	}
}

