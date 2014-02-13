#include "WPILib.h" 
#include "CORERobot/CORERobot.h"
#include "Subsystems.h"
#include "vision.h"

using namespace CORE;

class CORE2014: public SimpleRobot {

	CORERobot robot;

	DriveSubsystem drive;
	//	CageSubsystem cage;
	ShooterSubsystem shooter;
	PickupSubsystem pickup;

	AutoSequencer autoSeq;
public:
	CORE2014() :
		robot(), drive(robot),
		//		cage(robot),
				shooter(robot), pickup(robot), autoSeq() {
		robot.add(drive);
		robot.add(shooter);
		robot.add(pickup);
		//		robot.add(cage);
	}

	void RobotInit() {
		robot.robotInit();
		SmartDashboard::PutNumber("drive-duration", 2);
		SmartDashboard::PutNumber("drive-speed", .6);
		SmartDashboard::PutBoolean("is-right", false);

	}
	void Autonomous() {
		//		visionMain();

		GetWatchdog().SetEnabled(false);
				Windup wind_action(shooter);
				autoSeq.add_action(wind_action);
		//		Move pickup down so we can shoot
		DriveAction drive_action(drive,
				-SmartDashboard::GetNumber("drive-speed"),
				SmartDashboard::GetNumber("drive-duration"));
		autoSeq.add_action(drive_action);
		//		HotAction hot;
		//		autoSeq.add_action(hot);
				WaitAction wait(2);
				autoSeq.add_action(wait);
				FireShot fire_shot(shooter);
				autoSeq.add_action(fire_shot);
		while (IsAutonomous() and !IsDisabled()) {
			autoSeq.iter();
			Wait(0.05); // wait for a motor update time
		}
	}

	void OperatorControl() {
		robot.teleopInit();
		Watchdog& wd = GetWatchdog();
		wd.SetExpiration(.5);
		wd.SetEnabled(true);
		while (IsOperatorControl() && !IsDisabled()) {
			wd.Feed();
			SmartDashboard::PutBoolean("compressor-running",
					robot.compressor->GetPressureSwitchValue());
			robot.teleop();
			Wait(0.005); // wait for a motor update time
		}
	}

	/**
	 * Runs during test mode
	 */
	void Test() {
		robot.requirePneumatics();
		robot.compressor->Enabled();

	}
};

START_ROBOT_CLASS(CORE2014)
;

