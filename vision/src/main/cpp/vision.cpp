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
#include <cscore.h>

using namespace cv;
using namespace std;

RNG rng(12345);
Rect bounding_rect;
int thresh = 100;
float height_offset;
float width_offset;
int ResWidth = 640, ResHeight = 480;
float width_goal;
float height_goal;

bool usingTapeTrack = false;

// This is the main entrypoint into the Vision Program!
void curtin_frc_vision::run() {

  // This creates a webcam on USB, and dumps it into a sink. The sink allows us to access the image with sink.GrabFrame
  cs::UsbCamera cam{"USBCam", 0};
	/* Port 0 is default camera, but needs a webcam with a mac adress, usually an external camera, on either port 0 or 1
	And it's also to note, USB cameras for tinkerboards and Pi's start from 4 onwards, because weird reasons.*/
  cs::CvSink sink{"USB"};
  sink.SetSource(cam);
  
  cam.SetResolution(ResWidth, ResHeight);

  width_goal = ResWidth/2;
  height_goal = ResHeight/2;

	auto video_mode = cam.GetVideoMode();
  std::cout << "Width: " << video_mode.width << " Height: " << video_mode.height << std::endl;
  std::cout << "Goals: " << width_goal << "," << height_goal << endl; 

	
	// This lets us see the camera output on the robot dashboard. We give it a name, and a width and height.
  cs::CvSource output = frc::CameraServer::GetInstance()->PutVideo("USB Camera", video_mode.width, video_mode.height);

  cv::Mat imgOriginal{video_mode.height, video_mode.width, CV_8UC3};

	// Changes the exposure for detecting retroReflective Tape and gives minimal error (requires ring light, And only works once deployed to a Pi/Tinker)
	if (usingTapeTrack);
	{
		cam.SetExposureManual(-100);
	}

  while (true)
	{
		if (sink.GrabFrame(imgOriginal) != 0)
		{
			// Convert input image to HSV
			cv::Mat imgTracking;
			cvtColor(imgOriginal, imgTracking, COLOR_BGR2HSV); //Convert the captugreen frame from BGR to HSV

			
			// Threshold the HSV image, keep only the green pixels
			cv::inRange(imgTracking, cv::Scalar(35, 100, 100), cv::Scalar(78, 255, 255), imgTracking);
			
			// Forgets green pixels under a certain size. Good if you don't want many errors
			erode(imgTracking, imgTracking, getStructuringElement(MORPH_ELLIPSE, Size(10, 10)));
			dilate(imgTracking, imgTracking, getStructuringElement(MORPH_ELLIPSE, Size(10, 10)));

			// Detect edges using Canny
			Canny(imgTracking, imgTracking, thresh, thresh * 2);

			/// Find contours
			vector<vector<Point> > contours;
			vector<Vec4i> hierarchy;
			threshold( imgTracking, imgTracking, thresh, 255, THRESH_BINARY );
			findContours(imgTracking, contours, hierarchy, RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );


			/// Find the convex hull object for each contour
			vector<vector<Point> >hull(contours.size());
			for (size_t i = 0; i < contours.size(); i++)
			{
				convexHull(contours[i], hull[i]);
			}

			/// Draw contours + hull results
			imgTracking = cv::Mat::zeros(imgTracking.size(), CV_8UC3);
			vector<Rect> boundRect( contours.size() );


			for (size_t i = 0; i < contours.size(); i++)
			{
				Scalar color = Scalar(rng.uniform(0, 256), rng.uniform(0, 256), rng.uniform(0, 256));
				drawContours(imgTracking, contours, (int)i, color);
				drawContours(imgTracking, hull, (int)i, color);
			}

			/// Detect edges using Threshold
			threshold( imgTracking, imgTracking, thresh, 255, THRESH_BINARY );
			/// Find contoursBox


			/// Approximate contoursBox to polygons + get bounding rects and circles
			vector<vector<Point> > hull_poly( hull.size() );
			vector<Point2f>center( hull.size() );
			vector<float>radius( hull.size() );

			for( int i = 0; i < hull.size(); i++ )
				{ approxPolyDP( Mat(hull[i]), hull_poly[i], 3, true );
				boundRect[i] = boundingRect( Mat(hull_poly[i]) );
				minEnclosingCircle( (Mat)hull_poly[i], center[i], radius[i] );
				}


			/// Draw polygonal contour + bonding rects + circles
			for( int i = 0; i< hull.size(); i++ )
			{
				Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
				drawContours( imgTracking, hull_poly, i, color, 1, 8, vector<Vec4i>(), 0, Point() );
				bounding_rect=boundingRect(contours[i]); // Find the bounding rectangle for biggest contour
				rectangle( imgTracking, boundRect[i].tl(), boundRect[i].br(), color, 2, 8, 0 );
				circle( imgTracking, center[i], (int)radius[i], color, 2, 8, 0 );
			}
				

			// get the moments 
			vector<Moments> mu(hull.size());
			for( int i = 0; i<hull.size(); i++ )
			{ mu[i] = moments( hull[i], false ); }
	
			// get the centroid of figures.
			vector<Point2f> mc(hull.size());
			for( int i = 0; i<hull.size(); i++)
			{ mc[i] = Point2f( mu[i].m10/mu[i].m00 , mu[i].m01/mu[i].m00 ); }

			for( int i = 0; i<hull.size(); i++ )
			{
				Scalar color = Scalar(167,151,0); // B G R values
				//drawContours(imgTracking, hull_poly, i, color, 2, 8, hierarchy, 0, Point());
				circle( imgTracking, mc[i], 4, color, -1, 8, 0 );

				// offsets from center
				Point center = Point((mc[i].x), (mc[i].y));
				width_offset = width_goal - center.x;
				height_offset = height_goal - center.y;
		
      	std::stringstream offsetY;	offsetY << height_offset;
      	std::stringstream offsetX;	offsetX << width_offset;
      	cv::putText(imgTracking, "xy(" + offsetX.str() + "," + offsetY.str() + ")", mc[i] + cv::Point2f(-25,25), cv::FONT_HERSHEY_COMPLEX_SMALL, 1, cv::Scalar(255,0,255)); //text with distance and angle on target
			}

		#ifdef __DESKTOP__
    	cv::imshow("Output Image", imgTracking); // Tinkerboards and Pi's don't like imshow, so we get rid of it while running on dev boards
			cv::imshow("Original Image", imgOriginal);
		#endif
    	output.PutFrame(imgTracking);
			output.PutFrame(imgOriginal);

			waitKey(30);
		}
		else
		{	
			std::cout << "Cannot read a frame from video stream. Is the external WebCam Plugged in?" << endl;
		}
	}
}