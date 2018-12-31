// Code was put togethor by CJ... just in case someone was wondering.                          ...not that many would
#include "vision.h"

#include <iostream>
#include <opencv2/opencv.hpp>
#include "opencv2/objdetect.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/core/core.hpp"
#include <stdio.h>

using namespace cv;
using namespace std;

RNG rng(12345);
int largest_area=0;
int largest_contour_index=0;
Rect bounding_rect;
int thresh = 100;
float rectXpoint;
float rectYpoint;
float height_offset;
float width_offset;
// This is the main entrypoint into the CurtinFRC Vision Program!
void curtin_frc_vision::run() {

  // Note that the first webcam plugged in is always on /dev/video0. Likewise, the second is on
  // /dev/video1, third on /dev/video2, etc.
  // In our case, we just say '0'. This lets it work on mac, linux and windows!
  VideoCapture cap{0};
  cap.set(CAP_PROP_FRAME_WIDTH, 640);
  cap.set(CAP_PROP_FRAME_HEIGHT, 360);
  float width_goal;
  float height_goal;
  double width = cap.get(CV_CAP_PROP_FRAME_WIDTH);
  double height = cap.get(CV_CAP_PROP_FRAME_HEIGHT);
  
  if (!cap.isOpened()) {
    std::cout << "ERROR: Could not open camera!" << std::endl;
    return;
  }
  width_goal = width / 2;
  height_goal = height / 2;

  	cout << "This Code Is Meant For The 2019 FRC Game" << endl;
	cout << "Center May change depending on camera, or cap.set" << endl;
	cout << "Current cap.set = " << width << "x" << height << endl << endl;
	cout << "x,y Points for center Goal = (" << width_goal << "," << height_goal << ")" << endl;
  while (true)
	{

		//Green Hue Processing Block
		//========================================================================================================
		//--------------------------------------------------------------------------------------------------------
		//========================================================================================================
		Mat imgOriginal;
		//Mat green_track_output;

		bool bSuccess = cap.read(imgOriginal); // read a new frame from video

		if (!bSuccess) //if not success, break loop
		{
			cout << "Cannot read a frame from video stream" << endl;
			//break;
		}


		// Convert input image to HSV
		Mat img_HSV;

		cvtColor(imgOriginal, img_HSV, COLOR_BGR2HSV); //Convert the captugreen frame from BGR to HSV

		
		// Threshold the HSV image, keep only the green pixels
		
		cv::Mat green_hue_image;
		cv::inRange(img_HSV, cv::Scalar(35, 100, 100), cv::Scalar(78, 255, 255), green_hue_image);
	  //morphological opening (remove small objects from the foreground)
		/*
		erode(green_hue_image, green_hue_image, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
		dilate(green_hue_image, green_hue_image, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
		//morphological closing (fill small holes in the foreground)
		dilate(green_hue_image, green_hue_image, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
		erode(green_hue_image, green_hue_image, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
		*/
		//========================================================================================================
		//--------------------------------------------------------------------------------------------------------
		//========================================================================================================


		




		// Contours Blocks (Draws a convex shell over the thresholded image.
		//________________________________________________________________________________________________________
		//________________________________________________________________________________________________________
		/// Detect edges using Canny
		Mat canny_output;
		Canny(green_hue_image, canny_output, thresh, thresh * 2);

		/// Find contours
		vector<vector<Point> > contours;
		Mat threshold_output;
		vector<Vec4i> hierarchy;
		threshold( green_hue_image, threshold_output, thresh, 255, THRESH_BINARY );
		//findContours( threshold_output, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE, Point(0, 0) );
		findContours(green_hue_image, contours, hierarchy, RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );
		/// Find the convex hull object for each contour
	
		
		vector<vector<Point> >hull(contours.size());
		for (size_t i = 0; i < contours.size(); i++)
		{
			convexHull(contours[i], hull[i]);
		}

		/// Draw contours + hull results
		Mat drawing = Mat::zeros(canny_output.size(), CV_8UC3);
		vector<Rect> boundRect( contours.size() );


		for (size_t i = 0; i < contours.size(); i++)
		{
			Scalar color = Scalar(rng.uniform(0, 256), rng.uniform(0, 256), rng.uniform(0, 256));
			drawContours(drawing, contours, (int)i, color);
			drawContours(drawing, hull, (int)i, color);
		}
		//namedWindow("hull", WINDOW_AUTOSIZE);
		//________________________________________________________________________________________________________
		//________________________________________________________________________________________________________








		

		// Bounding Box Block, (Draws a border around the processed image)
		//,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,
		
		//vector<vector<Point> > contoursBox;

		/// Detect edges using Threshold
		threshold( green_hue_image, threshold_output, thresh, 255, THRESH_BINARY );
		/// Find contoursBox
		//findContours( threshold_output, contoursBox, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE, Point(0, 0) );


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
			bounding_rect=boundingRect(contours[i]); // Find the bounding rectangle for biggest contour
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

    // draw contours
    //Mat drawingcenter(canny_output.size(), CV_8UC3, Scalar(255,255,255));

	  for( int i = 0; i< contours.size(); i++ ) // iterate through each contour. 
  {
   double a=contourArea( contours[i],false);  //  Find the area of contour

   if(a>largest_area)
   {
   largest_area=a;
   largest_contour_index=i;                //Store the index of largest contour
   //bounding_rect=boundingRect(contours[i]); // Find the bounding rectangle for biggest contour
   circle( drawing, mc[largest_contour_index], 4, Scalar(0,0,255), -1, 8, 0 );
   std::cout << "x1 " << mc[i] << std::endl; 
   }

  }
  
    for( int i = 0; i<hull.size(); i++ )
      {
      Scalar color = Scalar(167,151,0); // B G R values
      drawContours(drawing, hull_poly, i, color, 2, 8, hierarchy, 0, Point());
      circle( drawing, mc[i], 4, color, -1, 8, 0 );

			// offsets from center
			Point center = Point((mc[i].x), (mc[i].y));
			width_offset = width_goal - center.y;
			height_offset = height_goal - center.x;
			cout << "Offset From Center x,y =" << width_offset << "," << height_offset << endl; //The output values... are a bit strange, need to look into that
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
		//imshow("Original", imgOriginal); //Shows the original image
		//imshow("Track Output", green_hue_image);//Shows the Threhold Image
		imshow("Threshold Image", green_hue_image);

		//-------------------------------------------------------------------------------------------------------
		//-------------------------------------------------------------------------------------------------------
		
		
		if (waitKey(30) == 27) //wait for 'esc' key press for 30ms. If 'esc' key is pressed, break loop
		{
			cout << "esc key is pressed by user" << endl;
			//break;
		}
	}
}