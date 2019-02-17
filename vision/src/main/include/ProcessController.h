#pragma once
#include "threading/Runnable.h"
#include "Display.h"
#include "TapeProcessing.h"
#include "HatchProcessing.h"

#include "networktables/NetworkTableInstance.h"

class Processing : public Runnable, public Displayable {
 public:
	void ProcessPick();
	nt::NetworkTableEntry TapeCamSet;
	std::string GetProcessType();
	Processing(Capture &capture) {}
	virtual void Init() override;
  	virtual void Periodic() override;

	virtual void GetDisplayMat(cv::Mat &displayMat) override {};
	virtual cv::Size GetDisplaySize() override {};

 private:
	Capture _capture{"HatchSide", 0};
	TapeProcessing _tape{_capture};
	HatchProcessing _hatch{_capture};
	std::string processType;

 protected:
  std::mutex _classMutex;
  std::string processType;
  Capture &_capture;
  cs::VideoMode _videoMode;

  cv::Mat _imgProcessedTrack;
  cv::Mat _imgProcessedThresh;
  cv::Mat _imgProcessing;
}





class Process : public Runnable, public Displayable {
 public:
  virtual void Init() override;
  virtual void Periodic() override;

  Process(Capture &capture);
  Capture &GetCapture();

  void CopyProcessedTrack(cv::Mat &imgProcessedTrack);
  void CopyProcessedThresh(cv::Mat &imgProcessedThresh);
  
  void GetDisplayMat(cv::Mat &displayMat) override;
  cv::Size GetDisplaySize() override;

  std::string GetProcessType();
  bool GetValidThresh();
  bool GetValidTrack();
};
