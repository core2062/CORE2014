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
		
		SmartDashboard::PutBoolean("auto-arm", true);
		SmartDashboard::PutBoolean("auto-drive", true);
		SmartDashboard::PutBoolean("auto-open-arms", true);
		SmartDashboard::PutBoolean("auto-hot", true);
		SmartDashboard::PutBoolean("auto-shoot", true);
		SmartDashboard::PutBoolean("use-ultra", true);
		SmartDashboard::PutBoolean("return-ultra-true", false);
		
		std::string* twoBallString = new std::string("Two-Ball");
		//std::string* catchString = new std::string("Catch");
		std::string* normalString = new std::string("Normal");
		
		autoChoose.AddDefault("Normal", normalString);
		autoChoose.AddObject("Two-Ball", twoBallString);
		//autoChoose.AddObject("Catch", catchString);
		SmartDashboard::PutData("auto-chooser", &autoChoose);
		
		
	}
	void Autonomous() {
		GetWatchdog().SetEnabled(false);
		std::string choice = * (std::string*) autoChoose.GetSelected();
		Windup wind_action(shooter);
		DriveAction drive_action(drive, -SmartDashboard::GetNumber("drive-speed"), SmartDashboard::GetNumber("drive-duration"));
		DriveAction drive_action_two(drive, -SmartDashboard::GetNumber("drive-speed"), SmartDashboard::GetNumber("drive-to-mid-duration"));		
		OpenArms open_arms_action(shooter, pickup, cage);
		HotAction hot;
		FireShot fire_shot(shooter);
		Windup wind_action_2(shooter);
		AntiDefenseAction anti_def_action(drive, 1);
		FireShot fire_shot_2(shooter);
		PickupOut pickup_action(pickup);
		
		//autoSeq.add_action(anit_def_action);
		autoSeq.add_action(wind_action);
		if (choice == "Normal"){
			
			autoSeq.add_action(drive_action);
			if (SmartDashboard::GetBoolean("use-ultra")){
				autoSeq.add_action(anti_def_action);
			}
			autoSeq.add_action(open_arms_action);
			autoSeq.add_action(hot);
			autoSeq.add_action(fire_shot);
		}else{
//			autoSeq.add_action(anti_def_action);
//			autoSeq.add_action(test_vision);
			autoSeq.add_action(pickup_action);
			autoSeq.add_action(drive_action_two);
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

