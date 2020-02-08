// RobotBuilder Version: 2.0
//
// This file was generated by RobotBuilder. It contains sections of
// code that are automatically generated and assigned by robotbuilder.
// These sections will be updated in the future when you export to
// C++ from RobotBuilder. Do not put any code or make any change in
// the blocks indicating autogenerated code or it will be lost on an
// update. Deleting the comments indicating the section will prevent
// it from being updated in the future.


#include "Commands/VisionDrive.h"
#include <iostream>

// BEGIN AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=CONSTRUCTOR

VisionDrive::VisionDrive(): frc::Command() {
        // Use requires() here to declare subsystem dependencies
    // eg. requires(Robot::chassis.get());
    // BEGIN AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=REQUIRES
	Requires(Robot::vision.get());
    Requires(Robot::driveTrain.get());
    // END AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=REQUIRES
}
    // END AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=CONSTRUCTOR

/**
 * @brief Called just before this Command runs the first time
 */
void VisionDrive::Initialize() {
    Robot::vision->visionDrive.SetBoolean(false);
    omegaIntegrator = 0;
}

/**
 * @brief Called repeatedly when this Command is scheduled to run
 * 
 */
void VisionDrive::Execute() {
 
    if (Robot::vision->TargetIsLocked())
    {
        // Publish automove flag to true while this code is running
        Robot::vision->visionDrive.SetBoolean(true);

        // calulate distance error
        Robot::vision->optimalShootingDistance = frc::Preferences::GetInstance()->GetDouble("Optimal Shooting Distance", Robot::vision->optimalShootingDistance);
        double distanceFromTarget = Robot::vision->GetDistanceToPowerport();
        Robot::vision->distance.SetDouble(distanceFromTarget);
        double distanceError =  Robot::vision->optimalShootingDistance - distanceFromTarget;

        // get angle error
        double angleError = Robot::vision->GetXAngleToTarget();
        
        // deadband angle error
        if (angleError < Robot::vision->angleErrorDeadband && angleError > -Robot::vision->angleErrorDeadband) {
            angleError = 0;
        }
        else if(angleError > Robot::vision->angleErrorDeadband) {
            angleError -= Robot::vision->angleErrorDeadband;
        }
        else {
            angleError += Robot::vision->angleErrorDeadband;
        }


        Robot::vision->kP_Omega = Robot::vision->kP_Omega_Entry.GetDouble(Robot::vision->kP_Omega);
        Robot::vision->kI_Omega = Robot::vision->kI_Omega_Entry.GetDouble(Robot::vision->kI_Omega);
        Robot::vision->kP_Distance = Robot::vision->kP_Distance_Entry.GetDouble(Robot::vision->kP_Distance);
        double omega = 0.0;
        double speed = 0.0;

        omegaIntegrator += angleError * deltaTime;
        omega = Robot::vision->kP_Omega * angleError;
        omega += omegaIntegrator * Robot::vision->kI_Omega;
 
        if (omega > Robot::vision->omegaLimiter)
        {
            omega = Robot::vision->omegaLimiter;
        }
        else if (omega < -Robot::vision->omegaLimiter)
        {
            omega = -Robot::vision->omegaLimiter;
        }
        
        speed = Robot::vision->kP_Distance * distanceError;
        Robot::driveTrain->VelocityArcade(speed, omega);


        Robot::targetLocked = true;
        Robot::visionDriveActive = true;
        Robot::distance = distanceFromTarget;
        Robot::distanceError = distanceError;
        Robot::angleError = angleError;
        Robot::visionSpeed = speed;
        Robot::visionOmega = omega;
    }
    else
    {
        Robot::vision->visionDrive.SetBoolean(false);
        Robot::targetLocked = false;
        Robot::visionDriveActive = false;
        Robot::distance = 0;
        Robot::distanceError = 0;
        Robot::angleError = 0;
        Robot::visionSpeed = 0;
        Robot::visionOmega = 0;
        Robot::vision->TakeSnapshot();
    }
}

/**
 * @brief Make this return true when this Command no longer needs to run execute()
 * 
 * @return true when no target is locked
 * @return false when a target is locked
 */
bool VisionDrive::IsFinished() {
    return !Robot::vision->TargetIsLocked();
}

/**
 * @brief Called once after isFinished returns true
 */
void VisionDrive::End() {
     Robot::vision->visionDrive.SetBoolean(false);
     Robot::visionDriveActive = false;
     omegaIntegrator = 0;
}

/**
 * Called when another command which requires one or more of the same subsystems is scheduled to run
 */
void VisionDrive::Interrupted() {
    End();
}


