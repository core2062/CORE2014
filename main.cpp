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

		SmartDashboard::PutNumber("auto-drive-minimum-duration", 1);
		SmartDashboard::PutNumber("auto-drive-speed", .6);
		SmartDashboard::PutNumber("auto-cage-delay", .4);
		SmartDashboard::PutNumber("auto-pickup-delay", .4);
		SmartDashboard::PutNumber("auto-2-roller-duration", 1);
		SmartDashboard::PutNumber("auto-2-wait-for-windup", 2.7);
		SmartDashboard::PutNumber("ultra-volt-in", 5.0);
		
		SmartDashboard::PutNumber("auto-1-drive-dist", 138);
		SmartDashboard::PutNumber("auto-2-drive-dist", 138);
		
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
		
		cout << "Auto mode: " <<choice << endl;
		if (choice == "one-ball"){
			WindupAction wind_action(shooter);
			autoSeq.add_action(wind_action);
			VisionAction hot (&right_hot);
			autoSeq.add_action(hot);
			DriveActionUltra drive_after(drive, -SmartDashboard::GetNumber("auto-drive-speed"),
				SmartDashboard::GetNumber("auto-1-drive-dist"), SmartDashboard::GetNumber("auto-drive-minimum-duration"));
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
			DriveActionUltra drive_action(drive, -SmartDashboard::GetNumber("auto-drive-speed"),
					SmartDashboard::GetNumber("auto-2-drive-dist"), SmartDashboard::GetNumber("auto-drive-minimum-duration"));
			autoSeq.add_action(drive_action);
			CageAction cage_out(cage, 1, SmartDashboard::GetNumber("auto-cage-delay"));
			autoSeq.add_action(cage_out);
			ShootAction shoot(shooter);
			autoSeq.add_action(shoot);
			RollerIn intake_in(pickup);
			autoSeq.add_action(intake_in);
			WindupAction windup(shooter, false);
			autoSeq.add_action(windup);
			WaitAction wait_for_windup(SmartDashboard::GetNumber("auto-2-wait-for-windup"));
			autoSeq.add_action(wait_for_windup);
			PickupRollerAction pickup_in(pickup, -1, SmartDashboard::GetNumber("auto-2-roller-duration"));
			autoSeq.add_action(pickup_in);
			PickupAction pickup_out_2(pickup, 1, .5);
			autoSeq.add_action(pickup_out_2);
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
			SmartDashboard::PutBoolean("compressor-full",
					(robot.compressor->GetPressureSwitchValue()));
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

