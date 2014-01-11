#ifndef DRIVESUBSYSTEM_H
#define DRIVESUBSYSTEM_H

#include "CORERobot/CORERobot.h"
#include "WPILib.h"

using namespace CORE;


class DriveSubsystem : public CORESubsystem {
	Jaguar leftFront;
	Jaguar leftBack;
	Jaguar rightFront;
	Jaguar rightBack;
	
	DoubleSpeed rightDrive;
	DoubleSpeed leftDrive;
	
	Encoder rightEncoder;
	Encoder leftEncoder;
	
	COREDrive drive;
	
public:
	std::string name(void){
		return "drive";
	}
	
	DriveSubsystem(CORERobot& robot):
		CORESubsystem(robot),
		//1 is just a placeholder for motor ports
		leftFront(1),
		leftBack(2),
		rightFront(3),
		rightBack(4),
		
		rightDrive(leftFront, leftBack),
		leftDrive(rightFront, rightBack),
		
		rightEncoder(1,2),
		leftEncoder(3,4),
		
		drive(leftDrive, rightDrive)
		
	{

	}
	
	void robotInit(void);
	void teleopInit(void);
	void teleop(void);
	
};

#endif
