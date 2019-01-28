#pragma once

#include "Capture.h"
#include "Runnable.h"

#include <opencv2/opencv.hpp>
#include "opencv2/objdetect.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/core/core.hpp"

#include <mutex>

class Process : public Runnable {
 public:
  virtual void Init() override;
  virtual void Periodic() override;

  Process(Capture &capture);
  Capture &GetCapture();
  void CopyImgOriginal(cv::Mat &imgOriginal);

  void CopyImgBallThresh(cv::Mat &imgballThresh);
  void CopyImgBallTrack(cv::Mat &imgballtrack);

  void CopyImgHatchThresh(cv::Mat &imghatchThresh);
  void CopyImgHatchTrack(cv::Mat &imghatchTrack);

 protected:
  std::mutex _classMutex;
  Capture &_capture;
  cs::VideoMode _videoMode;

  cv::Mat _imgOriginal;

  cv::Mat _imgBallThresh;
  cv::Mat _imgBallTrack;

  cv::Mat _imgHatchThresh;
  cv::Mat _imgHatchTrack;
};
