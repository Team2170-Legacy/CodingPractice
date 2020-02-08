/*----------------------------------------------------------------------------*/
/* Copyright (c) 2017-2018 FIRST. All Rights Reserved.                        */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

#include <frc/Joystick.h>
//#include <frc/PWMVictorSPX.h>
#include <frc/TimedRobot.h>
#include "frc/WPILib.h"
#include "ctre/Phoenix.h"
#include "rev/CANSparkMax.h"
#include <iostream>
using namespace std;

/**
 * This sample program shows how to control a motor using a joystick. In the
 * operator control part of the program, the joystick is read and the value is
 * written to the motor.
 *
 * Joystick analog values range from -1 to 1 and speed controller inputs as
 * range from -1 to 1 making it easy to work together.
 */
class Robot : public frc::TimedRobot {
 public:
  void TeleopPeriodic() override {
    //cout << "Joystick Y: " << m_stick.GetY() << endl; 
    //double speed = frc::Preferences::GetInstance()->GetDouble("Motor Power:", 0.0);
    double desiredRpm = frc::Preferences::GetInstance()->GetDouble("Desired Shooter RPM", 0.0);
    frc::SmartDashboard::PutNumber("Shooter RPM", encoderController->GetVelocity());
    frc::SmartDashboard::PutNumber("Shooter Voltage", sparkMax->GetAnalog().GetVoltage());

    //sparkMax->Set(speed);
    //sparkMax->Set(m_stick.GetY());
    cout << "rpm: " << desiredRpm << endl;
    pidController->SetReference(desiredRpm, rev::ControlType::kVelocity);
  }

  void RobotInit() override {
    maxAccelPerSec = frc::Preferences::GetInstance()->GetDouble("maxAccelPerSec", 2000.0);
    kP = frc::Preferences::GetInstance()->GetDouble("Shooter kP", 0.0);
    kI = frc::Preferences::GetInstance()->GetDouble("Shooter kI", 0.0);
    kD = frc::Preferences::GetInstance()->GetDouble("Shooter kD", 0.0);
    kFF = frc::Preferences::GetInstance()->GetDouble("Shooter kFF", 0.04);
    kMaxRPM = frc::Preferences::GetInstance()->GetDouble("maxRPM", 2000.0);

    sparkMax.reset(new rev::CANSparkMax(1, rev::CANSparkMax::MotorType::kBrushless));
    sparkMax->SetPeriodicFramePeriod(rev::CANSparkMaxLowLevel::PeriodicFrame::kStatus0, 20);

    //sparkAnalog.reset(new rev::CANAnalog(*sparkMax, rev::CANAnalog::AnalogMode::kAbsolute));

    pidController.reset(new rev::CANPIDController(sparkMax->GetPIDController()));
    encoderController.reset(new rev::CANEncoder(sparkMax->GetEncoder()));
    encoderController->SetVelocityConversionFactor(1);

    // Set min/max powers
    pidController->SetOutputRange(kMinOutput, kMaxOutput);
    pidController->SetSmartMotionMaxVelocity(kMaxRPM);
    pidController->SetSmartMotionMaxAccel(maxAccelPerSec);

    // Set kFF and kP (after tuning, since gains have already been determined)
    pidController->SetP(kP);
    pidController->SetFF(kFF);
  }

 private:
  // PID Gains for closed-loop velocity control
	double kP = 0, kI = 0, kD = 0, kIz = 0, kFF, kMaxOutput = 1, kMinOutput = -1;
  double kMaxRPM = 0.0;
  double maxAccelPerSec = 0.0;
  frc::Joystick m_stick{0};
  std::shared_ptr<rev::CANSparkMax> sparkMax;
  //std::shared_ptr<rev::CANAnalog> sparkAnalog;
  std::shared_ptr<rev::CANPIDController> pidController;
  std::shared_ptr<rev::CANEncoder> encoderController;
  
};


#ifndef RUNNING_FRC_TESTS
int main() { return frc::StartRobot<Robot>(); }
#endif

