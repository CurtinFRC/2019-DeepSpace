#include "Display.h"
#include "TapeInit.h"
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

void curtin_frc_vision::Display() {
    if (sink.GrabFrame(imgOriginal) != 0) {
		imshow("Original", imgOriginal); //Shows the original image
	}


	if (waitKey(30) == 27) //wait for 'esc' key press for 30ms. If 'esc' key is pressed, break loop
	{
		cout << "esc key is pressed by user" << endl;
		//break;
	}
}