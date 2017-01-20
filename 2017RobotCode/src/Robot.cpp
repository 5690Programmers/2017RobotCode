#include <iostream>
#include <memory>
#include <string>

#include <VictorSP.h>
#include <Joystick.h>
#include <SampleRobot.h>
#include <SmartDashboard/SendableChooser.h>
#include <SmartDashboard/SmartDashboard.h>
#include <RobotDrive.h>
#include <Timer.h>
#include <ADXRS450_Gyro.h>
#include <Spark.h>
#include <NetworkTables/NetworkTable.h>
#include <DoubleSolenoid.h>
#include <CameraServer.h>
#include <XboxController.h>


class Robot: public frc::SampleRobot {
	//Driving
	frc::RobotDrive myRobot { 0, 1, 2, 3 };
	frc::Joystick stick { 1 };
	frc::XboxController Xbox { 0 };
	//Motors and Stuff
	frc::VictorSP Kicker { 0 };
	frc::VictorSP Shooter { 1 };
	frc::VictorSP Climber { 2 };
	frc::VictorSP Intake { 3 };
	frc::VictorSP Agitator { 4 };
	frc::DoubleSolenoid Shifter1 { 0, 1 };
	frc::DoubleSolenoid Shifter2 { 2, 3 };
	//Addons
	frc::ADXRS450_Gyro gyro;
	frc::SendableChooser<std::string> chooser;
	const std::string autoNameDefault = "Default";
	const std::string autoNameCustom = "My Auto";

public:
	Robot() {

		myRobot.SetExpiration(0.1);
	}

	void RobotInit() {
		chooser.AddDefault(autoNameDefault, autoNameDefault);
		chooser.AddObject(autoNameCustom, autoNameCustom);
		frc::SmartDashboard::PutData("Auto Modes", &chooser);
		frc::CameraServer::GetInstance()->StartAutomaticCapture();
		gyro.Reset(); // sets front to 0 degrees 0 degrees
	}

	//AUTONOMUS
	void Autonomous() {
		auto autoSelected = chooser.GetSelected();

		std::cout << "Auto selected: " << autoSelected << std::endl;

		if (autoSelected == autoNameCustom) {
			// Custom Auto goes here
			std::cout << "Running custom Autonomous" << std::endl;
			myRobot.SetSafetyEnabled(false);
			Shifter1.Set(DoubleSolenoid::Value(2));
			Shifter2.Set(DoubleSolenoid::Value(2));
			Wait(0.0005);
			myRobot.Drive(0, 0.5);
			Wait(2);
			Shifter1.Set(DoubleSolenoid::Value(1));
			Shifter2.Set(DoubleSolenoid::Value(1));
			Wait(0.0005);
			myRobot.Drive(0, 0.5);
			Wait(2);
			myRobot.Drive(0, 0);
		} else {
			// Default Auto goes here
			std::cout << "Running default Autonomous" << std::endl;
			myRobot.SetSafetyEnabled(false);
			myRobot.Drive(-0.5, 0.0); // drive forwards half speed
			frc::Wait(2.0);                // for 2 seconds
			myRobot.Drive(0.0, 0.0);  // stop robot
		}
	}

	//TELEOP
	// FIXED: Vision Tracking on Bumpers and Shooting Mechanisms(Kicker and BallShooter) on respective Triggers

	//**DRIVER PREF**
	//Agitator on A
	//Climber on Start/Back
	//Shifting on X
	//Intake on Y
	//Left or Right Joystick for driving


	void OperatorControl() override
	{
		myRobot.SetSafetyEnabled(true);
		while (IsOperatorControl() && IsEnabled())
		{
			// drive with arcade style (use right stick)
			myRobot.ArcadeDrive( Xbox.GetX(XboxController::JoystickHand(0)), Xbox.GetY(XboxController::JoystickHand(0)));

			// wait for a motor update time
			frc::Wait(0.005);

			//NOT TESTED
			//Kicker Piston for Gear
			if (Xbox.GetTriggerAxis(XboxController::JoystickHand(0)))
			{
				Kicker.Set(DoubleSolenoid::Value(1));
			}
			else
			{
				Kicker.Set(DoubleSolenoid::Value(2));
			}

			//Vision Tracking for Gear
			if (Xbox.GetBumper(XboxController::JoystickHand(0)))
			{

			}

			//Shooter Trigger
			if (Xbox.GetTriggerAxis(XboxController::JoystickHand(1)))
			{
				Shooter.Set(1);
			}else
			{
				Shooter.Set(0);
			}

			//Vision Tracking for Shooter
			if (Xbox.GetBumper(XboxController::JoystickHand(1)))
			{

			}

			//Hooper Alligator ** Firmly cup the ball**
			if (Xbox.GetAButton())
			{
				Agitator.Set(0.2);
			}else
			{
				Agitator.Set(0);
			}

			//Climber Down
			if (Xbox.GetBackButton())
			{
				Climber.Set(-1);
			}else
			{
				Climber.Set(0);
			}

			//Climber Up
			if (Xbox.GetStartButton())
			{
				Climber.Set(1);
			}else
			{
				Climber.Set(0);
			}

			//Shifter
			if (Xbox.GetXButton()&& Shifter1.Get()!=0&& Shifter2.Get()!=0)
			{
				Shifter1.Set(DoubleSolenoid::Value(1));
				Shifter2.Set(DoubleSolenoid::Value(1));
			}
			else
			{
				Shifter1.Set(DoubleSolenoid::Value(2));
				Shifter2.Set(DoubleSolenoid::Value(2));
			}

			//Intake
			if (Xbox.GetYButton())
			{
				Intake.Set(-1);
			}else
			{
				Intake.Set(0);
			}
		}

	}

	/*
	 * Runs during test mode
	 */
	void Test()
    {
	}
};
START_ROBOT_CLASS(Robot)
