#include "WPILib.h" 
#include "CORERobot/CORERobot.h"
#include "Subsystems.h"
#include "vision.h"

using namespace CORE;


class CORE2014 : public SimpleRobot {
	
	CORERobot robot;

	DriveSubsystem drive;
//	CageSubsystem cage;
	ShooterSubsystem shooter;
//	PickupSubsystem pickup;
	
	AutoSequencer sequencer;
public:
	CORE2014():
		robot(),
		drive(robot),
//		cage(robot),
		shooter(robot),
//		pickup(robot),
		sequencer()
	{
		robot.add(drive);
		robot.add(shooter); 
//		robot.add(pickup);
//		robot.add(cage);
	}

	void RobotInit(){
		robot.robotInit();
		SmartDashboard::PutNumber("drive-distance", 15);
		SmartDashboard::PutBoolean("is-right", false);
	
	}
	void Autonomous(){
		visionMain();
		
		// drive forward
		//DriveAction drive_action (drive, .7, SmartDashboard::GetNumber("drive-distance"));
		// hot detection
		// turn (dependant on hot or not)
		// windup and shoot
		//FireShot fire_shot (shooter);
	}


	void OperatorControl(){
		robot.teleopInit();
		Watchdog& wd = GetWatchdog();
		wd.SetExpiration(.5);
		wd.SetEnabled(true);
		while (IsOperatorControl() && !IsDisabled()){
			wd.Feed();
			SmartDashboard::PutBoolean("compressor-running", robot.compressor->GetPressureSwitchValue());
			robot.teleop();
			Wait(0.005);				// wait for a motor update time
		}
	}
	
	/**
	 * Runs during test mode
	 */
	void Test() {
		
	}
};

START_ROBOT_CLASS(CORE2014);

