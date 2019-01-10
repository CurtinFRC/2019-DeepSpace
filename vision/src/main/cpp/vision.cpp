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
#include <cscore.h>

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

void curtin_frc_vision::run() {

  // This is just a demonstration so you can see how this kind of code works. You'll be replacing this
  // with our actual vision tracking software!
  // You can view the vision output with Shuffleboard. Launch with `./gradlew :vision:ShuffleBoard`

  vector<cv::Point2f> centres;
  vector<float> heights;
  vector<bool> lefts;
  vector<bool> rights;
  vector<cv::Point2f> targets;
  
  // This creates a webcam on USB, and dumps it into a sink. The sink allows us to access the image with sink.GrabFrame
  cs::UsbCamera cam{"USBCam", 1};
  cs::CvSink sink{"USB"};
  sink.SetSource(cam);

  // The camera defaults to a lower resolution, but you can choose any compatible resolution here.
  cam.SetResolution(640, 480);

  width_goal = 320;
  height_goal = 240;


  auto video_mode = cam.GetVideoMode();
  std::cout << "Width: " << video_mode.width << " Height: " << video_mode.height << std::endl;

  // This lets us see the camera output on the robot dashboard. We give it a name, and a width and height.
  cs::CvSource output = frc::CameraServer::GetInstance()->PutVideo("USB Camera", video_mode.width, video_mode.height);

  // The capMat is what comes from the camera, and the outMat is what goes to the dashboard. Note: 
  // the height - width order is reversed here (height first, width second), unlike other parts.
  cv::Mat imgOriginal{video_mode.height, video_mode.width, CV_8UC3};
  cv::Mat img_HSV{video_mode.height, video_mode.width, CV_8UC3};


  while (true) {
    // Grab a frame. If it's not an error (!= 0), convert it to grayscale and send it to the dashboard.
    if (sink.GrabFrame(imgOriginal) != 0) {
      //cv::cvtColor(imgOriginal, img_HSV, COLOR_RGB2HSV);
      cv::cvtColor(imgOriginal, img_HSV, cv::COLOR_RGB2HSV);
      //output.PutFrame(drawing);
    }
    //Green Hue Processing Block
		//========================================================================================================
		//--------------------------------------------------------------------------------------------------------
		//========================================================================================================
	



		
		// Threshold the HSV image, keep only the green pixels
		
		cv::Mat green_hue_image;
		cv::inRange(img_HSV, cv::Scalar(35, 100, 100), cv::Scalar(78, 255, 255), green_hue_image);

//__________________________________________________________________________VERY PROCCESSING HEAVY use low numbers or don't at if all if you can.__________



	  //morphological opening (remove small objects from the foreground)
	  /*
		erode(green_hue_image, green_hue_image, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
		dilate(green_hue_image, green_hue_image, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
*/
	/*
		//morphological closing (fill small holes in the foreground)
		dilate(green_hue_image, green_hue_image, getStructuringElement(MORPH_ELLIPSE, Size(2, 2)));
		erode(green_hue_image, green_hue_image, getStructuringElement(MORPH_ELLIPSE, Size(2, 2)));
*/

		//========================================================================================================
		//--------------------------------------------------------------------------------------------------------
		//========================================================================================================




		// Contours Blocks (Draws a convex shell over the thresholded image.)
		//________________________________________________________________________________________________________
		//________________________________________________________________________________________________________

    
    //KinectProcessor();
    //void processRGB(Mat mat);
    //void processIR(Mat mat);
        
    //kinectCondition t_condition;
    vector<vector<Point> > contours;
    vector<vector<Point> > filteredContours;
    vector<vector<Point> > filteredHulls;
    vector<Rect> ir_rects;
    int active_contour;
    Scalar hsl_low, hsl_high;
    bool show_window;

    //cv::Mat tmp(image);
    
    // Usually this is used to convert it to HLS, but since it's an IR (monochrome) image,
    // there's no need to waste the CPU Time doing this 
    // cvtColor(tmp, tmp, CV_RGB2HLS);
    
    double largestArea = 0.0;
    active_contour = -1;
    
    findContours(green_hue_image, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_TC89_KCOS);
    int i;
    for (i = 0; i < contours.size(); i++) {
        vector<Point> contour = contours[i];
        Rect r = boundingRect(contour);
       
        double area = contourArea(contour);
        if (area > 300.0) {
            vector<Point> hull;
            convexHull(contour, hull);
            double solidity = 100 * area / contourArea(hull);
            
            if (solidity < 60.0) {
                if (area > largestArea) {
                    largestArea = area;
                    active_contour = filteredContours.size();
                }
                filteredContours.push_back(contour);
                filteredHulls.push_back(hull);
                ir_rects.push_back(r);
            }
        }
    }

		/// Detect edges using Canny
		cv::Mat canny_output;
		Canny(green_hue_image, canny_output, thresh, thresh * 2);

		/// Find contours
		//vector<vector<Point> > contours;
		cv::Mat threshold_output;
		vector<Vec4i> hierarchy;
		threshold( green_hue_image, threshold_output, thresh, 255, THRESH_BINARY );
		//findContours( threshold_output, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE, Point(0, 0) );
		//findContours(green_hue_image, contours, hierarchy, RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );

    
		/// Find the convex hull object for each contour
		vector<vector<Point> >hull(filteredContours.size());
		for (size_t i = 0; i < filteredContours.size(); i++)
		{
			convexHull(filteredContours[i], hull[i]);
		}

		/// Draw filteredContours + hull results
		cv::Mat drawing = Mat::zeros(canny_output.size(), CV_8UC3);
		vector<Rect> boundRect( filteredContours.size() );


		for (size_t i = 0; i < filteredContours.size(); i++)
		{
			Scalar color = Scalar(rng.uniform(0, 256), rng.uniform(0, 256), rng.uniform(0, 256));
			drawContours(drawing, filteredContours, (int)i, color);
			drawContours(drawing, hull, (int)i, color);
		}
    
		//________________________________________________________________________________________________________
		//________________________________________________________________________________________________________

		
		//Get RotatedRectangles 
		centres.clear(); //clear the vectors
		heights.clear();
		lefts.clear();
		rights.clear();

		for (int i=0; i<contours.size(); i++) {
			
			cv::RotatedRect rotatedRect = cv::minAreaRect(contours[i]);

			float angle = rotatedRect.angle;

			cv::Point2f centre = rotatedRect.center;

			cv::Point2f rectPoints[4];
			rotatedRect.points(rectPoints);

			float min = rectPoints[0].y;
			float max = rectPoints[0].y;
			
			for (int j=1; j<4; j++) { //find the minimum and maximum y-values of each rectangle
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

			if (angle > 10 && angle < 19) {
				rights.push_back(true);
				lefts.push_back(false);
			} else if (angle < -10 && angle > -19) {
				rights.push_back(false);
				lefts.push_back(true);
			} else {
				rights.push_back(false);
				lefts.push_back(false);
			}

			std::stringstream ss;	ss<<angle; //magic shit, idk
			std::stringstream hei;	hei<<height;	
			cv::putText(green_hue_image, ss.str() + " height:" + hei.str(), centre + cv::Point2f(-25,25), cv::FONT_HERSHEY_COMPLEX_SMALL, 1, cv::Scalar(255,0,255)); //label the angle on each rectangle
		}

		int leftmost = -1;
		float leftPos = 1280;
		targets.clear();	

		for (int i=0; i<contours.size(); i++) {
			if (lefts[i]) { //checks if current iteration is a left
				for (int j=0; j<contours.size(); j++) {
					if (rights[j] && centres[j].x < leftPos) { //checks if nested iteration is a right and left of the last checked one
						leftmost = j;
					}
				}

				if (leftmost > -1) {
					targets.push_back((centres[i]+centres[leftmost])/2);
				}
			}
		}

		Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );

		for (int i=0; i<targets.size(); i++) {
			cv::rectangle(green_hue_image, targets[i] + Point2f(-3,-3), targets[i] + Point2f(3,3), color, 2); //draw small rectangle on target locations
		}



		

		// Bounding Box Block, (Draws a border around the processed image)
		//,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,
		
		//vector<vector<Point> > contoursBox;

		/// Detect edges using Threshold
		threshold( green_hue_image, threshold_output, thresh, 255, THRESH_BINARY );
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
			drawContours( drawing, hull_poly, i, color, 1, 8, vector<Vec4i>(), 0, Point() );
			bounding_rect=boundingRect(filteredContours[i]); // Find the bounding rectangle for biggest contour
			rectangle( drawing, boundRect[i].tl(), boundRect[i].br(), color, 2, 8, 0 );
			circle( drawing, center[i], (int)radius[i], color, 2, 8, 0 );
			}
			

    //_____________________Center Calcs______(Calculates the center from Border Box, And calculates X,Y Offset)_______ Ok.. it's suppose to calculate from borderbox, but not yet. using hull instead
      // get the moments 
	 vector<Moments> mu(hull.size());
    for( int i = 0; i<hull.size(); i++ )
      { mu[i] = moments( hull[i], false ); }
 
    // get the centroid of figures.
    vector<Point2f> mc(hull.size());
    for( int i = 0; i<hull.size(); i++)
      { mc[i] = Point2f( mu[i].m10/mu[i].m00 , mu[i].m01/mu[i].m00 ); }

    // draw filteredContours
    //Mat drawingcenter(canny_output.size(), CV_8UC3, Scalar(255,255,255));
  

    for( int i = 0; i<hull.size(); i++ )
      {
      Scalar color = Scalar(167,151,0); // B G R values
      //drawContours(drawing, hull_poly, i, color, 2, 8, hierarchy, 0, Point());
	  circle( drawing, mc[i], 4, color, -1, 8, 0 );

			// offsets from center
			Point center = Point((mc[i].x), (mc[i].y));
			width_offset = width_goal - center.x;
			height_offset = height_goal - center.y;
			cout << "Offset From Center x,y =" << height_offset << "," << width_offset << endl; //The output values... are a bit strange, need to look into that
			
    }
		//,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,










		// User Outputs Block, uneeded for tracking on bot.
		//-------------------------------------------------------------------------------------------------------
		//-------------------------------------------------------------------------------------------------------




		/// Show in a window
		//cout << "Point(x,y)=" << center.x << "," << center.y << " Offset: Height(" << height_offset << ") Width(" << width_offset << ")" << "\r";
		
    imshow("Shell & Bounding", drawing);
		//imshow("HSV Image", img_HSV);
    	//imshow("center Calc", drawingcenter);
		//imshow("Contours", drawingBox);
		imshow("Original", imgOriginal); //Shows the original image
		//imshow("Track Output", green_hue_image);//Shows the Threhold Image
		imshow("Threshold Image", green_hue_image);
    
    output.PutFrame(drawing);

		//-------------------------------------------------------------------------------------------------------
		//-------------------------------------------------------------------------------------------------------
		
		
		if (waitKey(30) == 27) //wait for 'esc' key press for 30ms. If 'esc' key is pressed, break loop
		{
			cout << "esc key is pressed by user" << endl;
			//break;
		}



  }
}