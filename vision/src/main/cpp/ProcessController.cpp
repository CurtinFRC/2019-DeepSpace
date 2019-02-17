#include "ProcessController.h"
#include "networktables/NetworkTableInstance.h"
#include "Capture.h"
#include "TapeProcessing.h"
#include "HatchProcessing.h"
#include "Display.h"
#include <iostream>

#include "threading/Runnable.h"

bool _tapeSet = true;
bool _lastTapeSet = false;

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