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

  /**
   * Gains for the Pathfinder Control Loop
   * 
   * These gains are in terms of the output value of voltage.
   * kP should be tested, with initial value of 12 (12 volts per metre from target). This is the primary gain.
   * kI is unused.
   * kD should be tested, with initial value of 0 (0 volts per metre per second). This is used to correct for overshoot / unstable following
   * kV should be taken from the Drivetrain Characterization Tests, run with @ref DrivetrainCharacterizationStrategy.
   * kA should be taken from the Drivetrain Characterization Tests, run with @ref DrivetrainCharacterizationStrategy.
   * kG should be tested, starting with 12 / 90 (12 volts per 90 deg from target heading). This is used to correct heading.
   */
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

    /**
     * Get the Maximum Acceleration of the robot at 12V, according to kA
     */
    double GetMaximumAcceleration() const;

    /**
     * Get the Maximum Velocity of the robot at 12V, according to kV
     */
    double GetMaximumVelocity() const;

   private:
    std::shared_ptr<nt::NetworkTable> _table;
    double _kP, _kI, _kD, _kA, _kV, _kG;
    std::string _name;

    wpi::SmallVector<NTBoundDouble, 4> _ntbounds;
  };

  /**
   * Create a Controller for a Pathfinder Control Loop. This is analogous to PIDController, but for 
   * Pathfinder v1.
   */
  class PathfinderController {
   public:
    PathfinderController(PathfinderGains gains);
    
    /**
     * Load a Pathfinder path from file. 
     * 
     * @param project The project this path is on, e.g. 5333 or 5663
     * @param pathname The name of the path, as saved by Pathweaver
     */
    void Load(std::string project, std::string pathname);

    /**
     * Reset the controller. Reloads the gains, resets followers, but does not reset offsets
     * nor the paths loaded.
     */
    void Reset();

    /**
     * Set the offset of the PathfinderController. Use this when starting to follow a path such that the distance
     * starts from 0.
     */
    void SetOffset(double distanceLeft, double distanceRight);

    /**
     * Run a single iteration of the control loop calculation.
     * 
     * @param distanceLeft  The distance travelled by the left wheels, in metres.
     * @param distanceRight The distance travelled by the right wheels, in metres.
     * @param gyroAngle     The current angle of the gyroscope, in degrees, clockwise positive.
     * 
     * @return The left and right voltages respectively. May be larger in magnitude than 12.0 if the path is behind.
     */
    std::pair<double, double> Calculate(double distanceLeft, double distanceRight, double gyroAngle); 

    /**
     * Is the controller done?
     */
    bool IsDone();
   private:
    bool _isLoaded = false;
    PathfinderGains _gains;

    int _trajLen;
    Segment _segmentsL[8192];
    Segment _segmentsR[8192];

    FollowerConfig _cfg;
    DistanceFollower _followerL, _followerR;

    double _offsetL, _offsetR;
  };

} // ns curtinfrc
