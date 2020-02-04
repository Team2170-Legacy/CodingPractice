/*----------------------------------------------------------------------------*/
/* Copyright (c) 2017-2018 FIRST. All Rights Reserved.                        */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

#include "ToggleCameraMode.h"

bool toggle = false;

ToggleCameraMode::ToggleCameraMode() {
  // Use Requires() here to declare subsystem dependencies
  // eg. Requires(Robot::chassis.get());
  Requires(Robot::vision.get());
}

// Called just before this Command runs the first time
void ToggleCameraMode::Initialize() {
  Robot::vision->SetCamMode(toggle);
  toggle = !toggle;
}

// Called repeatedly when this Command is scheduled to run
void ToggleCameraMode::Execute() {}

// Make this return true when this Command no longer needs to run execute()
bool ToggleCameraMode::IsFinished() { return false; }

// Called once after isFinished returns true
void ToggleCameraMode::End() {}

// Called when another command which requires one or more of the same
// subsystems is scheduled to run
void ToggleCameraMode::Interrupted() {}
