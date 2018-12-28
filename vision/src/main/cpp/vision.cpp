#include "vision.h"

#include <iostream>

#include <opencv2/opencv.hpp>

// This is the main entrypoint into the CurtinFRC Vision Program!
void curtin_frc_vision::run() {
  // This is just a demonstration so you can see how this kind of code works. You'll be replacing this
  // with our actual vision tracking software!

  // Note that the first webcam plugged in is always on /dev/video0. Likewise, the second is on
  // /dev/video1, third on /dev/video2, etc.
  cv::VideoCapture cap{"/dev/video0"};

  if (!cap.isOpened()) {
    std::cout << "ERROR: Could not open camera!" << std::endl;
    return;
  }

  double width = cap.get(CV_CAP_PROP_FRAME_WIDTH);
  double height = cap.get(CV_CAP_PROP_FRAME_HEIGHT);

  std::cout << "Res: " << width << "x" << height << std::endl;

  cv::Mat frame;
  cap.read(frame);
  cv::imwrite("capture.png", frame);
}