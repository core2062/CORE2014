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
	
	DigitalOutput sonicOut;
	DigitalInput sonicIn;
	Ultrasonic sonic;
	
	Gyro gyro;
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
		
		driveChooser(),
		
		sonicOut(32),
		sonicIn(31),
		sonic(sonicOut, sonicIn, Ultrasonic::kInches),
		gyro(5)
	{
		drive.SetSafetyEnabled(false);
	}
	
	void robotInit(void);
	void teleopInit(void);
	void teleop(void);
	
	void resetDistance(void);
	void arcade_drive (float mag, float turn);
	double getSonicDist(void);
	float getRot(void);
	void resetRot(void);
	
};
/*class DriveAction : public Action{
	DriveSubsystem* drive;
	float speed;
	double distance;
	double currentDist;
public:
	DriveAction(DriveSubsystem& drive, float speed, double distance):
		drive(&drive),
		speed(speed),
		distance(distance)
	{
		currentDist = 0;
	}
	
	void init(void){
	}
	
	ControlFlow call(void){
		double velocity = drive->getVelocity();
		SmartDashboard::PutNumber("accel-velocity", velocity);
		currentDist += velocity;
		if (currentDist < SmartDashboard::GetNumber("two_ball_distance")){
			drive->arcade_drive(speed, 0);
			return CONTINUE;			
		}else{
			drive->arcade_drive(0, 0);
			return END;			
		}
			
	}
	
};*/
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
	double degrees;
	double rotation;
public:
	TurnAction(DriveSubsystem& drive, float speed, double degrees):
		drive(&drive),
		speed(speed),
		degrees(degrees)
	{
		rotation = 0;
	}
	
	void init(void){
		drive->resetRot();
	}
	
	ControlFlow call(void){
		rotation = drive->getRot();
		SmartDashboard::PutNumber("rotation", rotation);
		if (degrees<0){
			if (rotation>degrees){
				drive->arcade_drive(0, -speed);
				return CONTINUE;				
			}else{
				drive->arcade_drive(0, 0);
				return END;					
			}
		}else{
			if (rotation<degrees){
				drive->arcade_drive(0, speed);
				return CONTINUE;				
			}else{
				drive->arcade_drive(0, 0);
				return END;					
			}			
		}
	}
	
};

class AntiDefenseAction : public WaitAction{
	DriveSubsystem* drive;
	float speed;
	double dist;
public:
	AntiDefenseAction(DriveSubsystem& drive, double duration):
		WaitAction(duration),
		drive(&drive)
	{
		speed =1;
		dist = 0;
	}
	
	void init(void){

		dist = drive->getSonicDist();

	
	}
	ControlFlow call(void){
		ControlFlow flow = WaitAction::call();
		if (flow == CONTINUE){
			if ((dist > SmartDashboard::GetNumber("sonic-dist-min") && dist < SmartDashboard::GetNumber("sonic-dist-max")) || SmartDashboard::GetBoolean("return-ultra-true")){
				if (SmartDashboard::GetBoolean("is-right")){
					drive->arcade_drive(0, speed);
					return CONTINUE;
				}else {
					drive->arcade_drive(0, -speed);
					return CONTINUE;				
				}
			}else{
				drive->arcade_drive(0, 0);
				return END;						
			}
		} else {
			drive->arcade_drive(0, 0);
			return END;
		}

	}
};

#endif
