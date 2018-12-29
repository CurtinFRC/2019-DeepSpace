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
int thresh = 100;
bool heightcentre;
bool widthcentre;
bool centre;
string centreOut;

double height_lower_between;
double height_higher_between;
double width_lower_between;
double width_higher_between;

double rectXpoint;
double rectYpoint;

double height_offset;
double width_offset;
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

  height_lower_between = height_goal - 20;
  height_higher_between = height_goal + 20;
  width_lower_between = width_goal - 20;
  width_higher_between = width_goal + 20;

  cout << "This Code Is Meant For The 2019 FRC Game" << endl;
	cout << "Centre May change depending on camera, or cap.set" << endl;
	cout << "Current cap.set = " << width << "x" << height << endl << endl;
	cout << "x,y Points for Centre Goal = (" << width_goal << "," << height_goal << ")" << endl;
	cout << "Centre is true when height goal is between " << height_lower_between << "-" << height_higher_between << endl;
	cout << "And Width goal is between " << width_lower_between << "-" << width_higher_between << endl << endl;

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
			break;
		}


		// Convert input image to HSV
		Mat img_HSV;

		cvtColor(imgOriginal, img_HSV, COLOR_BGR2HSV); //Convert the captugreen frame from BGR to HSV

		
		// Threshold the HSV image, keep only the green pixels
		//cv::Mat lower_green_hue_range;
		//cv::Mat upper_green_hue_range;
		cv::Mat green_hue_image;
		//cv::inRange(img_HSV, cv::Scalar(35, 100, 100), cv::Scalar(50, 255, 255), lower_green_hue_range);
		//cv::inRange(img_HSV, cv::Scalar(50, 100, 100), cv::Scalar(78, 255, 255), upper_green_hue_range);
		cv::inRange(img_HSV, cv::Scalar(35, 100, 100), cv::Scalar(78, 255, 255), green_hue_image);

	  //morphological opening (remove small objects from the foreground)
		erode(green_hue_image, green_hue_image, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
		dilate(green_hue_image, green_hue_image, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
		
		//morphological closing (fill small holes in the foreground)
		dilate(green_hue_image, green_hue_image, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
		erode(green_hue_image, green_hue_image, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
		
		
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
		findContours(canny_output, contours, RETR_TREE, CHAIN_APPROX_SIMPLE);

		/// Find the convex hull object for each contour
		vector<vector<Point> >hull(contours.size());
		for (size_t i = 0; i < contours.size(); i++)
		{
			convexHull(contours[i], hull[i]);
		}

		/// Draw contours + hull results
		Mat drawing = Mat::zeros(canny_output.size(), CV_8UC3);


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

		Mat threshold_output;
		vector<vector<Point> > contoursBox;
		vector<Vec4i> hierarchy;

		/// Detect edges using Threshold
		threshold( green_hue_image, threshold_output, thresh, 255, THRESH_BINARY );
		/// Find contoursBox
		findContours( threshold_output, contoursBox, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE, Point(0, 0) );


		/// Approximate contoursBox to polygons + get bounding rects and circles
		vector<vector<Point> > contours_poly( contoursBox.size() );
		vector<Rect> boundRect( contoursBox.size() );
		vector<Point2f>center( contoursBox.size() );
		vector<float>radius( contoursBox.size() );

		for( int i = 0; i < contoursBox.size(); i++ )
			{ approxPolyDP( Mat(contoursBox[i]), contours_poly[i], 3, true );
			boundRect[i] = boundingRect( Mat(contours_poly[i]) );
			minEnclosingCircle( (Mat)contours_poly[i], center[i], radius[i] );
			}


		/// Draw polygonal contour + bonding rects + circles
		Mat drawingBox = Mat::zeros( threshold_output.size(), CV_8UC3 );
		for( int i = 0; i< contoursBox.size(); i++ )
			{
			Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
			drawContours( drawing, contours_poly, i, color, 1, 8, vector<Vec4i>(), 0, Point() );
			rectangle( drawing, boundRect[i].tl(), boundRect[i].br(), color, 2, 8, 0 );
			circle( drawing, center[i], (int)radius[i], color, 2, 8, 0 );
			}


      // get the moments 
    vector<Moments> mu(contours.size());
    for( int i = 0; i<contours.size(); i++ )
      { mu[i] = moments( contours[i], false ); }
 
    // get the centroid of figures.
    vector<Point2f> mc(contours.size());
    for( int i = 0; i<contours.size(); i++)
      { mc[i] = Point2f( mu[i].m10/mu[i].m00 , mu[i].m01/mu[i].m00 ); }

    // draw contours
    Mat drawingCentre(canny_output.size(), CV_8UC3, Scalar(255,255,255));
    for( int i = 0; i<contours.size(); i++ )
      {
      Scalar color = Scalar(167,151,0); // B G R values
      drawContours(drawingCentre, contours, i, color, 2, 8, hierarchy, 0, Point());
      circle( drawingCentre, mc[i], 4, color, -1, 8, 0 );
    }

		//,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,



		// X & Y Calculator (Calculates the x,y offset from the centre)
		//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

		for (unsigned int Xpoints = 0; Xpoints < contoursBox.size(); Xpoints++)
		{

			for (unsigned int Ypoints = 0; Ypoints < contoursBox[Ypoints].size(); Ypoints++)
			{
				width_offset = width_goal - contoursBox[Xpoints][Ypoints].y;
				height_offset = height_goal - contoursBox[Xpoints][Ypoints].x;
        rectXpoint = contoursBox[Xpoints][Ypoints].x;
        rectYpoint = contoursBox[Xpoints][Ypoints].y;
        //cout << "Point(x,y)=" << contoursBox[Xpoints][Ypoints].x << "," << contoursBox[Xpoints][Ypoints].y << "\r";

				if (height_offset > -20 && height_offset < 20);
				{
					heightcentre = true;
				}
				if (width_offset > -20 && width_offset < 20);
				{
					widthcentre = true;
				}
				if (heightcentre == true && widthcentre == true);
				{
					centre = true;
				}
        if (centre == true)
        {
          centreOut = "True";
        }



        if (height_offset < -20);
				{
					heightcentre = false;
				}
        if (height_offset > 20);
				{
					heightcentre = false;
				}
				if (width_offset < -20);
				{
					widthcentre = false;
				}
        if (width_offset > 20);
				{
					widthcentre = false;
				}
				if (heightcentre == false && widthcentre == false);
				{
					centre = false;
				}
        if (centre == false)
        {
          centreOut = "False";
        }
				break;
			}
			break;
		}
		
		//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

		// User Outputs Block, uneeded for tracking on bot.
		//-------------------------------------------------------------------------------------------------------
		//-------------------------------------------------------------------------------------------------------



		//cv::addWeighted(green_hue_image, 1.0, drawing, 1.0, 0.0, green_track_output);
		
		/// Show in a window
		cout << "Point(x,y)=" << rectXpoint << "," << rectYpoint << "Offset: Height(" << height_offset << ") Width(" << width_offset << ")" << " Centre = " << centreOut << "\r";
		imshow("Shell & Bounding", drawing);
		imshow("HSV Image", img_HSV);
    imshow("Centre Calc", drawingCentre);
		//imshow("Contours", drawingBox);
		//imshow("Original", imgOriginal); //Shows the original image
		//imshow("Track Output", green_hue_image);//Shows the Threhold Image
		//imshow("Threshold Image upper", green_hue_image);
		//imshow("Threshold Image lower", lower_green_hue_range);

		//-------------------------------------------------------------------------------------------------------
		//-------------------------------------------------------------------------------------------------------
		
		
		if (waitKey(30) == 27) //wait for 'esc' key press for 30ms. If 'esc' key is pressed, break loop
		{
			cout << "esc key is pressed by user" << endl;
			break;
		}

	}
}