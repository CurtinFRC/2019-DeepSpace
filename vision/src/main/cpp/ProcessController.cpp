#include "ProcessController.h"
#include "networktables/NetworkTableInstance.h"
#include "Capture.h"
#include "TapeProcessing.h"
#include "HatchProcessing.h"
#include "Display.h"
#include <iostream>

#include "threading/Runnable.h"

void Processing::Init() {
    _tapeSet = true;
    _lastTapeSet = false;
}

void Processing::Periodic() {

}

void Processing::ProcessPick() {
    auto inst = nt::NetworkTableInstance::GetDefault();
    auto visionTable = inst.GetTable("VisionTracking");
    TapeCamSet = visionTable->GetEntry("Camera Set");

    _tapeSet = TapeCamSet.GetBoolean(true);

    if (_tapeSet) {
        // Use tape
        if (_tapeSet != _lastTapeSet)
            _tape.Init();
        _tape.Periodic();
    } else {
        // Use hatch
        if (_tapeSet != _lastTapeSet)
            _hatch.Init();
        _hatch.Periodic();
    }
    
    _lastTapeSet = _tapeSet;    
}

void Processing::GetDisplayMat(cv::Mat &displayMat) {
    if (_tapeSet) {
        _tape.GetDisplayMat(_imgProcessedTrack);
        _imgProcessedTrack.copyTo(displayMat);
    } else {
        _hatch.GetDisplayMat(_imgProcessedTrack);
        _imgProcessedTrack.copyTo(displayMat);
    }
}

cv::Size Processing::GetDisplaySize() {
    if (_tapeSet) {
        return _tape.GetDisplaySize();
    } else {
        return _hatch.GetDisplaySize();
    }
}