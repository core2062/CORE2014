#ifndef DRIVESUBSYSTEM_H
#define DRIVESUBSYSTEM_H

#include "CORERobot/CORERobot.h"
#include "WPILib.h"

using namespace CORE;


class DriveSubsystem : public CORESubsystem {
	Victor leftFront;
	Victor leftBack;
	Victor rightFront;
	Victor rightBack;
	
	DoubleSpeed rightDrive;
	DoubleSpeed leftDrive;
	
	Encoder rightEncoder;
	Encoder leftEncoder;
	
	COREDrive drive;
	DoubleSolenoid rightShift;
	DoubleSolenoid leftShift;
	bool quickturn;
	bool quickturn_old;
	
	SendableChooser driveChooser;
public:
	std::string name(void){
		return "drive";
	}
	
	DriveSubsystem(CORERobot& robot):
		CORESubsystem(robot),
		
		leftFront(1),
		leftBack(3),
		rightFront(2),
		rightBack(4),
		
		rightDrive(leftFront, leftBack),
		leftDrive(rightFront, rightBack),
		
		rightEncoder(1,2),
		leftEncoder(3,4),
		
		drive(leftDrive, rightDrive),
		
		rightShift(1,2),
		leftShift(3,4),
		
		driveChooser()
		
	{
		quickturn = false;
		quickturn_old = false;
	}
	
	void robotInit(void);
	void teleopInit(void);
	void teleop(void);
	
};

#endif
