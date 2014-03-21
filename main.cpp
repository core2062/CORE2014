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
	
	bool firstRun;
public:
	CORE2014() :
		robot(),
		drive(robot),
		cage(robot),
		shooter(robot),
		pickup(robot),
		autoSeq(),
		autoChoose(),
		firstRun(false)
	{
			robot.add(drive);
			robot.add(shooter);
			robot.add(pickup);
			robot.add(cage);
	}

	void RobotInit() {
		robot.robotInit();
		
		SmartDashboard::PutBoolean("auto-hot-debug", false);
		SmartDashboard::PutBoolean("alt-operator-config", true);
		
		autoChoose.AddDefault("1 Ball", new std::string("one-ball"));
		autoChoose.AddObject("2 Ball", new std::string("two-ball"));
		autoChoose.AddObject("2 Ball 2 Hot", new std::string("two-hot"));
		SmartDashboard::PutData("auto-chooser", &autoChoose);

		SmartDashboard::PutNumber("auto-drive-duration", 1.45);
		SmartDashboard::PutNumber("auto-drive-speed", .6);
		SmartDashboard::PutNumber("auto-cage-delay", .4);
		SmartDashboard::PutNumber("auto-pickup-delay", .4);
		SmartDashboard::PutNumber("auto-drive-after-duration", 1);
		SmartDashboard::PutNumber("auto-roller-1", .5);
		SmartDashboard::PutNumber("auto-roller-2", 1);
		SmartDashboard::PutNumber("auto-wait-for-windup", 3.7);
		SmartDashboard::PutNumber("auto-drive-while", 0);
		
		AxisCamera& camera = AxisCamera::GetInstance("10.20.62.11");
		camera.WriteBrightness(10);
	}
	void Autonomous() {
		Watchdog& wd = GetWatchdog();
		
		std::string choice = * (std::string*) autoChoose.GetSelected();		
		shooter.setArmed(shooter.getSwitchRaw());
		autoSeq.clear();
		cout << "auto init armed --> " << shooter.isArmed() <<endl; 
		bool right_hot = true;
		
		drive.autoInit();
		
		cout << "Auto mode: " <<choice << endl;
		if (choice == "one-ball"){
			WindupAction wind_action(shooter);
			autoSeq.add_action(wind_action);
			VisionAction hot (&right_hot);
			autoSeq.add_action(hot);
			DriveAction drive_after(drive, -SmartDashboard::GetNumber("auto-drive-speed"),
				SmartDashboard::GetNumber("auto-drive-after-duration"));
			autoSeq.add_action(drive_after);
			PickupAction pickup_action (pickup, 1, SmartDashboard::GetNumber("auto-pickup-delay"));
			autoSeq.add_action(pickup_action);
			CageAction cage_action (cage, 1, SmartDashboard::GetNumber("auto-cage-delay"));
			autoSeq.add_action(cage_action);
			WaitIfAction wait_if_hot(&right_hot);
			autoSeq.add_action(wait_if_hot);
			ShootAction fire_shot(shooter);
			autoSeq.add_action(fire_shot);

			autoSeq.init();
			wd.Feed();
			while (IsAutonomous() and !IsDisabled()) {
				autoSeq.iter();
				wd.Feed();
				Wait(0.05); // wait for a motor update time
			}
		} else if (choice == "two-ball" || choice == "two-hot"){
			WindupAction wind_action(shooter);
			autoSeq.add_action(wind_action);
			PickupAction pickup_out(pickup, 1, SmartDashboard::GetNumber("auto-pickup-delay"));
			autoSeq.add_action(pickup_out);
			DriveAction drive_action(drive, -SmartDashboard::GetNumber("auto-drive-speed"),
					SmartDashboard::GetNumber("auto-drive-duration"));
			autoSeq.add_action(drive_action);
			CageAction cage_out(cage, 1, SmartDashboard::GetNumber("auto-cage-delay"));
			autoSeq.add_action(cage_out);
			ShootAction shoot(shooter);
			autoSeq.add_action(shoot);
			RollerIn intake_in(pickup);
			autoSeq.add_action(intake_in);
			WindupAction windup(shooter, false);
			autoSeq.add_action(windup);
			DriveAction drive_while_windup(drive, -SmartDashboard::GetNumber("auto-drive-speed"), SmartDashboard::GetNumber("auto-drive-while"));
			autoSeq.add_action(drive_while_windup);
			WaitAction wait_for_windup(SmartDashboard::GetNumber("auto-wait-for-windup"));
			autoSeq.add_action(wait_for_windup);
//			RollerAction roller_in(pickup, -1, SmartDashboard::GetNumber("auto-roller-1"));
//			autoSeq.add_action(roller_in);
			PickupRollerAction pickup_in(pickup, -1, SmartDashboard::GetNumber("auto-roller-2"));
			autoSeq.add_action(pickup_in);
			PickupAction pickup_out1(pickup, 1, .5);
			autoSeq.add_action(pickup_out1);
			WaitMidpointAction mid;
			autoSeq.add_action(mid);
			ShootAction shoot_2(shooter);
			autoSeq.add_action(shoot_2);
			wd.Feed();
			while (IsAutonomous() and !IsDisabled()) {
				autoSeq.iter();
				wd.Feed();
				Wait(0.05); // wait for a motor update time
			}
		} else if (choice == "two-hot"){
//			pickup down
//			PickupAction pickup_out_0(pickup, -1);
//			autoSeq.add_action(pickup_out_0);
//			roller in - pickup ball
//			RollerAction roller_in_0(pickup, -1, 1);
//			autoSeq.add_action(roller_in_0);
//			windup(background)
//			WindupAction windup(shooter);
//			autoSeq.add_action(windup);
//			drive forward 10 ft
//			DriveAction drive_action (drive, -SmartDashboard::GetNumber("auto-drive-speed"),
//					SmartDashboard::GetNumber("auto-drive-duration"));
//			autoSeq.add_action(drive_action);
//			turn right 30 degrees (need to go right for vision)
//			RotateAction rotate_for_vision (drive, .8, 30);
//			autoSeq.add_action(rotate_for_vision);
//			do vision
			VisionAction vision_action (&right_hot);
			autoSeq.add_action(vision_action);
//			pickup down
			PickupAction pickup_out_0(pickup, -1);
			autoSeq.add_action(pickup_out_0);
			DriveAction drive_action(drive, -SmartDashboard::GetNumber("auto-drive-speed"),
					SmartDashboard::GetNumber("auto-drive-duration"));
			autoSeq.add_action(drive_action);
			CageAction cage_out(cage, -1, .5);
			autoSeq.add_action(cage_out);
//			turn left if not hot
			RotateIfAction rotate_if_not_hot (drive, &right_hot, -60, .5, true);
			autoSeq.add_action(rotate_if_not_hot);
//			arms out, roller out
//			RollerAction roller_out_1 (pickup, 1, .5);
//			autoSeq.add_action(roller_out_1);
//			shoot
			WaitAction wait_for_shoot(2);
			autoSeq.add_action(wait_for_shoot);
			ShootAction first_shot (shooter);
			autoSeq.add_action(first_shot);
//			wind up for second shot
			WindupAction windup_2(shooter, false);
			autoSeq.add_action(windup_2);
//			intake roller .75 sec in
			RollerAction roller_in_2 (pickup, -1, 1.5);
			autoSeq.add_action(roller_in_2);
//			Intake in and pickup up .25 sec
			PickupRollerAction pickup_in_2 (pickup, 1, .75);
			autoSeq.add_action(pickup_in_2);
//			wait for 2 sec (testing)
			WaitAction wait_for_shot_1(2);
			autoSeq.add_action(wait_for_shot_1);
//			turn 60 degrees
			RotateIfAction rotate_if_hot_2 (drive, &right_hot, -60, .5);
			RotateIfAction rotate_if_not_hot_2 (drive, &right_hot, 60, .5, true);
			autoSeq.add_action(rotate_if_hot_2);
			autoSeq.add_action(rotate_if_not_hot_2);
//			pickup down
			PickupAction pickup_out_2(pickup, -1);
			autoSeq.add_action(pickup_out_2);
//			wait until 5 sec
//			WaitMidpointAction wait_action;
//			autoSeq.add_action(wait_action);
//			wait for 2 sec (testing)
			WaitAction wait_for_shot_2(2);
			autoSeq.add_action(wait_for_shot_2);
//			shoot
			ShootAction second_shot(shooter);
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

