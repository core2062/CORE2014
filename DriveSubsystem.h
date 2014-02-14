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
	
//	Encoder rightEncoder;
//	Encoder leftEncoder;
	
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
		
//		rightEncoder(1,2),
//		leftEncoder(3,4),
		
		drive(leftDrive, rightDrive),
		
		rightShift(1,5,6),
		leftShift(2,1,2),
		
		quickturn(false),
		
		driveChooser()
		
	{
		drive.SetSafetyEnabled(false);
	}
	
	void robotInit(void);
	void teleopInit(void);
	void teleop(void);
	
	void resetDistance(void);
	void arcade_drive (float mag, float turn);
	double getDistance(void);
	double getLeftEnc(void);
};
class DriveAction : public WaitAction{
	DriveSubsystem* drive;
	float speed;
public:
	DriveAction(DriveSubsystem& drive, float speed, double duration):
		WaitAction(duration),
		drive(&drive),
		speed(speed)
	{	}
	
	void init(void){
	}
	
	ControlFlow call(void){
		ControlFlow flow = WaitAction::call();
		if (flow == CONTINUE){
			drive->arcade_drive(speed, 0);
			return CONTINUE;
		} else {
			drive->arcade_drive(0, 0);
			return END;
		}
	}
	
};
class TurnAction : public Action{
	DriveSubsystem* drive;
	float speed;
	double ticks;
	//ticks is the amout of ticks from the encoder before it stops turning
	//int dir;
	//dir is direction
public:
	TurnAction(DriveSubsystem& drive, float speed, double ticks):
		drive(&drive),
		speed(speed),
		ticks(ticks)
	{	}
	
	void init(void){
		drive->resetDistance();
	}
	
	ControlFlow call(void){
		if (std::abs(drive->getDistance()) < ticks){
			drive->arcade_drive(0, speed);
			return CONTINUE;
		} else {
			drive->arcade_drive(0, 0);
			return END;
		}
	}
	
};

#endif
