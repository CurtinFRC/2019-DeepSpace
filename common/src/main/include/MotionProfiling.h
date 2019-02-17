#pragma once

#include <frc/SpeedController.h>
#include <pathfinder.h>

#include "sensors/Encoder.h"
#include "Files.h"
#include "NTUtil.h"

namespace curtinfrc {

  class CurtinPathfinder {
  public:
    /**
    * Load a pathfinder trajectory from a .csv file.
    */
    static int LoadDeployedFile(std::string project, std::string filename, Segment *segments) {
      FILE *fp;
      std::string base_path = curtinfrc::files::GetDeployDirectory(project);
      fp = fopen((base_path + "paths/output/" + filename + ".pf1.csv").c_str(), "r"); 
      int len = pathfinder_deserialize_csv(fp, segments); 
      fclose(fp);
      return len;
    }
  };

  class PathfinderGains {
   public:
    PathfinderGains(std::string name, double kP = 0, double kI = 0, double kD = 0, double kV = 0, double kA = 0, double kG = 0);

    // Needed since we need to reinit NT Bound Doubles with new address
    PathfinderGains(const PathfinderGains &other) : PathfinderGains(other._name, other._kP, other._kI, other._kD, other._kV, other._kA, other._kG) {}

    double GetkP() const;
    double GetkI() const;
    double GetkD() const;
    double GetkA() const;
    double GetkV() const;
    double GetkG() const;

   private:
    std::shared_ptr<nt::NetworkTable> _table;
    double _kP, _kI, _kD, _kA, _kV, _kG;
    std::string _name;

    wpi::SmallVector<NTBoundDouble, 4> _ntbounds;
  };

  class PathfinderController {
   public:
    PathfinderController(PathfinderGains gains);
    
    void Load(std::string project, std::string pathname);

    void Reset();
    std::pair<double, double> Calculate(double distanceLeft, double distanceRight, double gyroAngle); 
   private:
    bool _isLoaded = false;
    PathfinderGains _gains;

    int _trajLen;
    Segment _segmentsL[8192];
    Segment _segmentsR[8192];

    FollowerConfig _cfg;
    DistanceFollower _followerL, _followerR;
  };

} // ns curtinfrc
