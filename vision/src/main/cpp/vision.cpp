//To build Program use .\gradlew vision:build
//To Run Program use .\gradlew vision:runvision
//To deploy to a tinkerboard or pi us .\gradlew vision:deploy  Note that when using tinkerboard deploy might not work to OS, you might need to use Raspbian ISO properly installed to the sd
#include "vision.h"
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


RNG rng(12345);
Rect bounding_rect;
int thresh = 100;
float height_offset;
float width_offset;
float width_goal;
float height_goal;

// This is the main entrypoint into the CurtinFRC Vision Program!

<<<<<<< HEAD
=======
void curtin_frc_vision::run() {
	// Create and setup the first kinect present on the system
	kinect_device *kinect = kinect_driver::instance()->create(0);
	kinect->open();
	kinect->set_video(kinect_device::video_mode::IR);

	auto table = nt::NetworkTableInstance::GetDefault().GetTable("TestTable");
	table->GetEntry("KinectAngle").AddListener([&](const nt::EntryNotification &ev) {
		kinect->set_tilt(ev.value->GetDouble());
	}, NT_NotifyKind::NT_NOTIFY_UPDATE);

	cs::CvSource outputimg = frc::CameraServer::GetInstance()->PutVideo("Kinect Camera", 640, 480);

	kinect->set_video_callback([&](kinect_device &kin) {
		outputimg.PutFrame(*kin.video());
	});

	// Run kinect capture loop
	kinect_driver::instance()->run();

>>>>>>> master

  // This is just a demonstration so you can see how this kind of code works. You'll be replacing this
  // with our actual vision tracking software!
  // You can view the vision output with Shuffleboard. Launch with `./gradlew :vision:ShuffleBoard`

  vector<float> angles;
  vector<cv::Point2f> centres;
  vector<float> heights;
  vector<bool> lefts;
  vector<bool> rights;
  vector<cv::Point2f> targets;
  
  void curtin_frc_vision::run() {
  // This creates a webcam on USB, and dumps it into a sink. The sink allows us to access the image with sink.GrabFrame
  cs::UsbCamera cam{"USBCam", 1};
  cs::CvSink sink{"USB"};
  sink.SetSource(cam);

  // The camera defaults to a lower resolution, but you can choose any compatible resolution here.
  cam.SetResolution(640, 480);

  		auto video_mode = cam.GetVideoMode();
  		std::cout << "Width: " << video_mode.width << " Height: " << video_mode.height << std::endl;

  		// This lets us see the camera output on the robot dashboard. We give it a name, and a width and height.
  		cs::CvSource output = frc::CameraServer::GetInstance()->PutVideo("USB Camera", video_mode.width, video_mode.height);
  	
  		cv::Mat imgOriginal{video_mode.height, video_mode.width, CV_8UC3};
  		cv::Mat img_HSV{video_mode.height, video_mode.width, CV_8UC3};


 while (true) {
    	// Grab a frame. If it's not an error (!= 0), convert it to HSV and send it to the dashboard.
    	if (sink.GrabFrame(imgOriginal) != 0) {
      		cv::cvtColor(imgOriginal, img_HSV, cv::COLOR_BGR2HSV);
    	}
	
		imshow("Original", imgOriginal); //Shows the original image
	
		if (waitKey(30) == 27) //wait for 'esc' key press for 30ms. If 'esc' key is pressed, break loop
		{
			cout << "esc key is pressed by user" << endl;
			//break;
		}
  	}
}