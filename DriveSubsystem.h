#ifndef DRIVESUBSYSTEM_H
#define DRIVESUBSYSTEM_H

#include "CORERobot/CORERobot.h"
#include "WPILib.h"
#include <cmath>
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
		
		quickturn(false),
		
		driveChooser()
		
	{
	}
	
	void robotInit(void);
	void teleopInit(void);
	void teleop(void);
	
	void resetDistance(void);
	void arcade_drive (float mag, float turn);
	double getDistance(void);
};
class DriveAction : public Action{
	DriveSubsystem* drive;
	float speed;
	double distance;
public:
	DriveAction(DriveSubsystem& drive, float speed, double distance):
		drive(&drive),
		speed(speed),
		distance(distance)
	{	}
	
	void init(void){
		drive->resetDistance();
	}
	
	ControlFlow call(void){
		if (std::abs(drive->getDistance()) < distance){
			drive->arcade_drive(speed, 0);
			return CONTINUE;
		} else {
			drive->arcade_drive(0, 0);
			return END;
		}
	}
	
};

#endif
