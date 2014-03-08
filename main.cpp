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
		
		SmartDashboard::PutBoolean("auto-hot-debug", false);
		
		autoChoose.AddDefault("One Ball", new std::string("one-ball"));
		autoChoose.AddObject("Two Ball", new std::string("two-ball"));
		SmartDashboard::PutData("auto-chooser", &autoChoose);

		SmartDashboard::PutNumber("auto-drive-duration", 0);
		SmartDashboard::PutNumber("auto-drive-speed", .6);
		SmartDashboard::PutNumber("auto-cage-delay", .5);
		SmartDashboard::PutNumber("auto-pickup-delay", .5);
		SmartDashboard::PutNumber("auto-drive-after-duration", .6);
		
		AxisCamera& camera = AxisCamera::GetInstance("10.20.62.11");
		camera.WriteBrightness(10);
	}
	void Autonomous() {
		Watchdog& wd = GetWatchdog();
		std::string choice = * (std::string*) autoChoose.GetSelected();		
		bool right_hot = true;
		autoSeq.clear();
		AxisCamera& camera = AxisCamera::GetInstance("10.20.62.11");
		camera.WriteBrightness(10);
		cout << "Auto mode: " <<choice << endl;
		if (choice == "one-ball"){
//			WindupAction wind_action(shooter);
//			autoSeq.add_action(wind_action);
//			DriveAction drive_action(drive, -SmartDashboard::GetNumber("auto-drive-speed"),
//					SmartDashboard::GetNumber("auto-drive-duration"));
//			autoSeq.add_action(drive_action);
			CageAction cage_action (shooter, cage, -1, SmartDashboard::GetNumber("auto-cage-delay"));
			autoSeq.add_action(cage_action);
			PickupAction pickup_action (pickup, -1, SmartDashboard::GetNumber("auto-pickup-delay"));
			autoSeq.add_action(pickup_action);
			VisionWaitAction hot;
			autoSeq.add_action(hot);
			ShootAction fire_shot(shooter);
			autoSeq.add_action(fire_shot);
			WaitAction wait_before_drive(1);
			autoSeq.add_action(wait_before_drive);
			DriveAction drive_after(drive, -SmartDashboard::GetNumber("auto-drive-speed"),
								SmartDashboard::GetNumber("auto-drive-after-duration"));
			autoSeq.add_action(drive_after);

			autoSeq.init();
			wd.Feed();
			while (IsAutonomous() and !IsDisabled()) {
				autoSeq.iter();
				wd.Feed();
				Wait(0.05); // wait for a motor update time
			}
		} else if (choice == "two-ball"){
			return;
//			pickup down
			PickupAction pickup_out_0(pickup, -1);
			autoSeq.add_action(pickup_out_0);
//			roller in - pickup ball
			RollerAction roller_in_0(pickup, -1, 1);
			autoSeq.add_action(roller_in_0);
//			windup(background)
			WindupAction windup(shooter);
			autoSeq.add_action(windup);
//			drive forward 10 ft
			DriveAction drive_action (drive, -SmartDashboard::GetNumber("auto-drive-speed"),
					SmartDashboard::GetNumber("auto-drive-duration"));
			autoSeq.add_action(drive_action);
//			turn right 30 degrees (need to go right for vision)
			RotateAction rotate_for_vision (drive, .8, 30);
			autoSeq.add_action(rotate_for_vision);
//			do vision
			VisionAction vision_action (&right_hot);
			autoSeq.add_action(vision_action);
//			turn left if not hot
			RotateIfAction rotate_if_not_hot (drive, &right_hot, -60, .5, true);
			autoSeq.add_action(rotate_if_not_hot);
//			arms out, roller out
			RollerAction roller_out_1 (pickup, 1, .5);
			autoSeq.add_action(roller_out_1);
//			shoot
			ShootAction first_shot (shooter);
			autoSeq.add_action(first_shot);
//			wind up for second shot
			WindupAction windup_2(shooter);
			autoSeq.add_action(windup_2);
//			intake roller .75 sec in
			RollerAction roller_in_2 (pickup, -1, .75);
			autoSeq.add_action(roller_in_2);
//			Intake in and pickup up .25 sec
			PickupRollerAction pickup_in_2 (pickup, 1, .25);
			autoSeq.add_action(pickup_in_2);
//			turn 60 degrees
			RotateIfAction rotate_if_hot_2 (drive, &right_hot, -60, .5);
			RotateIfAction rotate_if_not_hot_2 (drive, &right_hot, 60, .5, true);
			autoSeq.add_action(rotate_if_hot_2);
			autoSeq.add_action(rotate_if_not_hot_2);
//			pickup down
			PickupAction pickup_out_2(pickup, -1);
			autoSeq.add_action(pickup_out_2);
//			wait until 5 sec
			WaitMidpointAction wait_action;
			autoSeq.add_action(wait_action);
//			shoot
			ShootAction second_shot (shooter);
			autoSeq.add_action(second_shot);

			autoSeq.init();
			wd.Feed();
			while (IsAutonomous() and !IsDisabled()) {
				autoSeq.iter();
				wd.Feed();
				Wait(0.05); // wait for a motor update time
			}
		} else {
			cout << "Bad auto type" <<endl;
		}
	}

	void OperatorControl() {
		robot.teleopInit();
		Watchdog& wd = GetWatchdog();
		wd.SetExpiration(.5);
		wd.SetEnabled(true);
		AxisCamera& camera = AxisCamera::GetInstance("10.20.62.11");
		camera.WriteBrightness(50);
		
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

