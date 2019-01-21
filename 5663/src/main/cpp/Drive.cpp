#include "Drive.h"
#include "frc/WPILib.h"
#include "CurtinCtre.h"
using namespace curtinfrc;
using namespace frc;

Drive::Drive(int l1, int l2, int r1, int r2) {
    leftMotor1 = new TalonSrx(l1, 2048);
    leftMotor2 = new VictorSpx(l2);    
    rightMotor1 = new TalonSrx(r1, 2048);
    rightMotor2 = new VictorSpx(r2);

    leftMotor2->Set(curtinfrc::VictorSpx::ControlMode::Follower, l1);
    rightMotor2->Set(curtinfrc::VictorSpx::ControlMode::Follower, r1);

    leftMotor1->SetSensorPhase(true);
    rightMotor1->SetSensorPhase(true);

    timeoutCheck = new Timer();
    timeoutCheck->Start();
}

void Drive::Stop() {
    leftMotor1->Set(ControlMode::PercentOutput, 0);
    rightMotor1->Set(ControlMode::PercentOutput, 0);
}

void Drive::TankDrive(double l, double r) {
    if(fabs(l) < deadzone) l = 0;
    if(fabs(r) < deadzone) r = 0;
    double left = l * fabs(l);
    double right = r * fabs(r);
    leftMotor1->Set(ControlMode::PercentOutput, left);
    rightMotor1->Set(ControlMode::PercentOutput, right);
}  

bool Drive::DriveForward(double distance, double speed, double timeout) {
    double leftEncoderFinal = distance * encoderConst;
    double rightEncoderFinal = distance * encoderConst;
    double driveTolerance = 50;
    double F = 3.5, P = 4.0, I = 0, D = 0;
    int acceleration = 200;

    if(driving == false) {

        leftMotor1->SetSelectedSensorPosition(0,0,10);
        rightMotor1->SetSelectedSensorPosition(0,0,10);

        //setup PID and start driving...
        leftMotor1->ConfigNominalOutputForward(0,0); //configuring the left encoder PID
        leftMotor1->ConfigNominalOutputReverse(0,0);
        leftMotor1->ConfigPeakOutputForward(1,10);
        leftMotor1->ConfigPeakOutputReverse(-1,10);
        leftMotor1->ConfigMotionCruiseVelocity(620*speed, 0);
        leftMotor1->ConfigMotionAcceleration(acceleration, 0);

        leftMotor1->Config_kF(0,F,0); //set left PID-F values
        leftMotor1->Config_kP(0,P,0);
        leftMotor1->Config_kI(0,I,0);
        leftMotor1->Config_kD(0,D,0);

        rightMotor1->ConfigNominalOutputForward(0,0); //configuring the right encoder PID
        rightMotor1->ConfigNominalOutputReverse(0,0);
        rightMotor1->ConfigPeakOutputForward(1,10);
        rightMotor1->ConfigPeakOutputReverse(-1,10);
        rightMotor1->ConfigMotionCruiseVelocity(620*speed, 0);
        rightMotor1->ConfigMotionAcceleration(acceleration, 0);

        rightMotor1->Config_kF(0,F,0); //set right PID-F values
        rightMotor1->Config_kP(0,P,0);
        rightMotor1->Config_kI(0,I,0);
        rightMotor1->Config_kD(0,D,0);

        leftMotor1->Set(ControlMode::MotionMagic, leftEncoderFinal); //drive code in this format
        rightMotor1->Set(ControlMode::MotionMagic, rightEncoderFinal);
        driving = true;
        timeoutCheck->Reset();
    } else {
        if((abs(leftMotor1->GetSelectedSensorPosition(0)) == 1 || leftMotor1->GetSelectedSensorPosition(0) == 0) && abs(rightMotor1->GetSelectedSensorPosition(0)) > 500) {
            //encoder broke on left
           //SmartDashboard::PutBoolean("Encoder Broke: ", true);
            leftMotor1->Set(ControlMode::PercentOutput, rightMotor1->GetMotorOutputPercent());
            leftEncoderFinal = 0;
        }
        else if((abs(rightMotor1->GetSelectedSensorPosition(0)) == 1 || rightMotor1->GetSelectedSensorPosition(0) == 0) && abs(leftMotor1->GetSelectedSensorPosition(0)) > 500) {
        //encoder broke on right
        //SmartDashboard::PutBoolean("Encoder Broke: ", true);
        rightMotor1->Set(ControlMode::PercentOutput, leftMotor1->GetMotorOutputPercent());
        rightEncoderFinal = 0;
        } else {
        //SmartDashboard::PutBoolean("Encoder Broke: ", false);
        }

        if(abs(leftEncoderFinal) + driveTolerance > abs(leftMotor1->GetSelectedSensorPosition(0)) &&  abs(leftEncoderFinal) - driveTolerance < abs(leftMotor1->GetSelectedSensorPosition(0)) && leftMotor1->GetSelectedSensorPosition(0) * leftEncoderFinal >= 0) {
        if(abs(rightEncoderFinal) + driveTolerance > abs(rightMotor1->GetSelectedSensorPosition(0)) &&  abs(rightEncoderFinal) - driveTolerance < abs(rightMotor1->GetSelectedSensorPosition(0)) && rightMotor1->GetSelectedSensorPosition(0) * rightEncoderFinal >= 0) {
            Stop();
            driving = false;
            return true;
        }
        }
        if(timeoutCheck->HasPeriodPassed(timeout) && timeout != 0) {
            Stop();
            driving = false;
            return true;
        }
    return false;
    }
}


