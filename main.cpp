#include "WPILib.h" 
#include "CORERobot/CORERobot.h"
#include "Subsystems.h"
#include "vision.h"

using namespace CORE;

class CORE2014: public SimpleRobot {

	CORERobot robot;

	DriveSubsystem drive;
	CageSubsystem cage;
	ShooterSubsystem shooter;
	PickupSubsystem pickup;

	AutoSequencer autoSeq;
	
	SendableChooser autoChoose;
public:
	CORE2014() :
		robot(),
		drive(robot),
		cage(robot),
		shooter(robot),
		pickup(robot),
		autoSeq(),
		autoChoose()
	{
			robot.add(drive);
			robot.add(shooter);
			robot.add(pickup);
			robot.add(cage);
	}

	void RobotInit() {
		robot.robotInit();
		
		SmartDashboard::PutNumber("drive-duration", 2.5);
		SmartDashboard::PutNumber("drive-tp-mid-duration", 2.5);
		SmartDashboard::PutNumber("drive-speed", .6);
		SmartDashboard::PutNumber("cage-delay", 0.2);
		
		SmartDashboard::PutBoolean("is-right", false);
		SmartDashboard::PutBoolean("hot-debug", false);
		
		SmartDashboard::PutNumber("sonic-dist-min", 0);
		SmartDashboard::PutNumber("sonic-dist-max", 5);
		
		SmartDashboard::PutBoolean("anti-defense", true);
		SmartDashboard::PutBoolean("return-ultra-true", false); // DEBUG
		
		autoChoose.AddDefault("Normal", new std::string("Normal"));
		autoChoose.AddObject("Two-Ball", new std::string("Two-Ball"));
		SmartDashboard::PutData("auto-chooser", &autoChoose);
	}
	void Autonomous() {
		GetWatchdog().SetEnabled(false);
		std::string choice = * (std::string*) autoChoose.GetSelected();		
		bool right_hot = true;
		
		if (choice == "Normal"){
			WindupAction wind_action(shooter);
			autoSeq.add_action(wind_action);
			DriveAction drive_action(drive, -SmartDashboard::GetNumber("drive-speed"),
					SmartDashboard::GetNumber("drive-duration"));
			autoSeq.add_action(drive_action);
			AntiDefenseAction anti_def_action(drive, 1);
			if (SmartDashboard::GetBoolean("anti-defense")){
				autoSeq.add_action(anti_def_action);
			}
			OpenArms open_arms_action(shooter, pickup, cage);
			autoSeq.add_action(open_arms_action);
			VisionAction hot (&right_hot);
			autoSeq.add_action(hot);
			WaitMidpoint wait_action;
			autoSeq.add_action(wait_action);
			ShootAction fire_shot(shooter);
			autoSeq.add_action(fire_shot);
		}else if (choice == "TwoBall"){
//			pickup down
			PickupAction pickup_out_0(pickup, -1);
//			roller in .1 sec
			RollerAction roller_in_0(pickup, 1, .5);
//			windup(background)
			WindupAction windup(shooter);
//			drive forward 10 ft
			DriveDistAction drive_ten (drive, 10*12);
//			turn right 30 degrees (need to go right for vision)
			RotateAction rotate_for_vision (drive, 30);
//			do vision
			VisionAction vision_action (&right_hot);
//			turn left if not hot
			RotateIfAction rotate_if_not_hot (drive, &right_hot, -60, true);
//			arms out, roller out
			RollerAction roller_out_1 (pickup, -1, .5);
//			shoot
			ShootAction first_shot (shooter);
//			intake roller .75 sec in
			RollerAction roller_in_2 (pickup, 1, .75);
//			Intake in and pickup up .25 sec
			PickupRollerAction pickup_in_2 (pickup, 1, .25);
//			turn 60 degrees
			RotateIfAction rotate_if_hot_2 (drive, &right_hot, -60);
			RotateIfAction rotate_if_not_hot_2 (drive, &right_hot, 60, true);
//			pickup down
			PickupAction pickup_out_2(pickup, -1);
//			wait until 5 sec
			WaitMidpoint wait_action;
//			shoot
			ShootAction second_shot (shooter);
			
			
		}

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
		Watchdog& wd = GetWatchdog();
		wd.SetExpiration(.5);
		wd.SetEnabled(true);
		robot.requirePneumatics();
		robot.compressor->Enabled();
		while(IsTest() && !IsDisabled()){
			wd.Feed();
			robot.compressor->Start();
			Wait(0.005);
		}
		robot.compressor->Stop();
	}
};

START_ROBOT_CLASS(CORE2014)
;

