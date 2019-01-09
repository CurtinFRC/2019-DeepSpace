#include "vision.h"
#include <iostream>
#include <stdio.h>
#include <opencv2/opencv.hpp>
#include "opencv2/objdetect.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/core/core.hpp"


using namespace std;

#ifndef RUNNING_FRC_TESTS
int main() {
  cout << "Starting CurtinFRC Vision & Depth Program" << endl << endl;;
  curtin_frc_vision vision;
  cout << "This Code Is Meant For The 2019 FRC Game" << endl;
	cout << "Center May change depending on camera, or cap.set" << endl;
  
 while (true) {
  vision.run();
}

  std::cout << "Vision Program Exited. uhhhh, issue??" << std::endl;
  return -1;
}
#endif