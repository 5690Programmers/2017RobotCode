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
#include <GenericHID.h>
#include <GamepadBase.h>



class Robot: public frc::SampleRobot {
	//Driving
	frc::RobotDrive myRobot { 0, 1, 2, 3};
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
	//Add ons
	frc::ADXRS450_Gyro gyro;

	frc::SendableChooser<std::string> chooser;
	const std::string autoNameDefault = "Default";
	const std::string autoNameCustom = "My Auto";
	const std::string TestAuto1 = "Test 1";
	const std::string bluelefttoleft = "Blue Left to Left";
	const std::string bluelefttomid = "Blue Left to Mid";
	const std::string bluelefttoright = "Blue Left to Right";
	const std::string bluemidtoleft = "Blue Mid to Left";
	const std::string bluemidtomid = "Blue Mid to Mid";
	const std::string bluemidtoright = "Blue Mid to Right";
	const std::string bluerighttoleft = "Blue Right to Left";
	const std::string bluerighttomid = "Blue Right to Mid";
	const std::string bluerighttoright = "Blue Right to Right";
	//Jetson



public:
	std::shared_ptr<NetworkTable> Steven;
	Robot() {
		Steven = NetworkTable::GetTable("database");//Change database to the location of the vision code
		myRobot.SetExpiration(0.1);

	}

	void RobotInit() {

		//double boo = 0.3;


		chooser.AddDefault(autoNameDefault, autoNameDefault);
		chooser.AddObject(autoNameCustom, autoNameCustom);
		chooser.AddObject(TestAuto1, TestAuto1);
		chooser.AddObject(bluelefttoleft, bluelefttoleft);
		chooser.AddObject(bluelefttomid, bluelefttomid);
		chooser.AddObject(bluelefttoright, bluelefttoright);
		chooser.AddObject(bluemidtoleft, bluemidtoleft);
		chooser.AddObject(bluemidtomid, bluemidtomid);
		chooser.AddObject(bluemidtoright, bluemidtoright);
		chooser.AddObject(bluerighttoleft, bluerighttoleft);
		chooser.AddObject(bluerighttomid, bluerighttomid);
		chooser.AddObject(bluerighttoright, bluerighttoright);
		frc::SmartDashboard::PutData("Auto Modes", &chooser);
		frc::CameraServer::GetInstance()->StartAutomaticCapture();
		gyro.Reset();
		Steven->SetTeam(5690);
		Steven->Initialize();
		Steven->SetUpdateRate(0.3);
		Steven->SetPort(5800);
		Steven->SetIPAddress("10.56.90.2");

	}

	//AUTONOMUS
	void Autonomous() {
		auto autoSelected = chooser.GetSelected();
		static const float kP = 0.03;
		float angle = gyro.GetAngle();// get heading
		std::cout << "Auto selected: " << autoSelected << std::endl;

		if (autoSelected == autoNameCustom) {
				// Custom Auto goes here
			std::cout << "Running custom Autonomous" << std::endl;
			myRobot.SetSafetyEnabled(false);
			gyro.Reset();
			while (IsAutonomous())
		{
			myRobot.Drive(-1.0, -angle * kP); // turn to correct heading
			Wait(0.004);
			myRobot.Drive(-0.5, 0); //Drive straight at 100%
			Wait(2); //Drive forward for 2 seconds
			myRobot.Drive(0, 0); //Stop
		}
			myRobot.Drive(0.0, 0.0); // stop robot

		}if (autoSelected == TestAuto1) {
			// Drives forward and then turns 45 degrees to the right
			std::cout << "Running Test Autonomous 1" << std::endl;
			myRobot.SetSafetyEnabled(false);
			gyro.Reset();
			myRobot.Drive(0.5, 0);
			Wait(1.5);
			while(IsAutonomous() and gyro.GetAngle() < 45){
				myRobot.Drive(0.5, -0.6);
			}
			myRobot.Drive(0.5, 0);
			//Vision Track
			myRobot.Drive(0, 0);
		}
		if (autoSelected == bluelefttoleft) {
			// Drives forward and then turns 45 degrees to the right
			std::cout << "Running Test Autonomous 1" << std::endl;
			myRobot.SetSafetyEnabled(false);
			gyro.Reset();
			myRobot.Drive(0.5, 0);
			Wait(1.5);
			while(IsAutonomous() and gyro.GetAngle() <= 45){
				myRobot.Drive(0.5, -0.6);
			}
			//Vision Track
			myRobot.Drive(0, 0);
		}
		if (autoSelected == bluelefttomid) {
			//Turns 45 degrees to the right, drives forward and then turns 45 degrees to the left
			std::cout << "Running Test Autonomous 1" << std::endl;
			myRobot.SetSafetyEnabled(false);
			gyro.Reset();
			while(IsAutonomous() and gyro.GetAngle() <= 45){
				myRobot.Drive(0.5, -0.6);
			}
			myRobot.Drive(0.5, 0);
			Wait(1.5);
			while(IsAutonomous() and gyro.GetAngle() >= 0){
				myRobot.Drive(0.5, 0.6);
			}
			//Vision Track
			myRobot.Drive(0, 0);
		}
		if (autoSelected == bluelefttoright) {
			// Turns 75 degrees to the right, drives forward, then turns 115 degrees to the left
			std::cout << "Running Test Autonomous 1" << std::endl;
			myRobot.SetSafetyEnabled(false);
			gyro.Reset();
			while(IsAutonomous() and gyro.GetAngle() <= 75){
				myRobot.Drive(0.5, -0.6);
			}
			myRobot.Drive(0.5, 0);
			Wait(3);
			while(IsAutonomous() and gyro.GetAngle() >= -45){
				myRobot.Drive(0.5, 0.6);
			}
			//Vision Track
			myRobot.Drive(0, 0);
		}
		if (autoSelected == bluemidtoleft) {
			//Turns 45 degrees to the left, drives forward, then turns 90 degrees to the right
			std::cout << "Running Test Autonomous 1" << std::endl;
			myRobot.SetSafetyEnabled(false);
			gyro.Reset();
			while(IsAutonomous() and gyro.GetAngle() > -45){
				myRobot.Drive(0.5, 0.6);
			}
			myRobot.Drive(0.5, 0);
			Wait(1.5);
			while(IsAutonomous() and gyro.GetAngle() < 45){
				myRobot.Drive(0.5, -0.6);
			}
			//Vision Track
			myRobot.Drive(0, 0);
		}
		if (autoSelected == bluemidtomid) {
			// Drives forward and then turns 45 degrees to the right
			std::cout << "Running Test Autonomous 1" << std::endl;
			myRobot.SetSafetyEnabled(false);
			gyro.Reset();
			myRobot.Drive(0.5, 0);
			Wait(0.5);
			//Vision Track
			myRobot.Drive(0, 0);
		}
		if (autoSelected == bluemidtoright) {
			//Turns 45 degrees to the right, drives forward, then turns 90 degrees to the left
			std::cout << "Running Test Autonomous 1" << std::endl;
			myRobot.SetSafetyEnabled(false);
			gyro.Reset();
			while(IsAutonomous() and gyro.GetAngle() < 45){
				myRobot.Drive(0.5, -0.6);
			}
			myRobot.Drive(0.5, 0);
			Wait(1.5);
			while(IsAutonomous() and gyro.GetAngle() > -45){
				myRobot.Drive(0.5, 0.6);
			}
			//Vision Track
			myRobot.Drive(0, 0);
		}
		if (autoSelected == bluerighttoleft) {
			// Turns 75 degrees to the left, drives forward, then turns 115 degrees to the right
			std::cout << "Running Test Autonomous 1" << std::endl;
			myRobot.SetSafetyEnabled(false);
			gyro.Reset();
			while(IsAutonomous() and gyro.GetAngle() >= -75){
				myRobot.Drive(0.5, 0.6);
			}
			myRobot.Drive(0.5, 0);
			Wait(3);
			while(IsAutonomous() and gyro.GetAngle() <= 45){
				myRobot.Drive(0.5, -0.6);
			}
			//Vision Track
			myRobot.Drive(0, 0);
		}
		if (autoSelected == bluerighttomid) {
			//Turns 45 degrees to the left, drives forward and then turns 45 degrees to the right
			std::cout << "Running Test Autonomous 1" << std::endl;
			myRobot.SetSafetyEnabled(false);
			gyro.Reset();
			while(IsAutonomous() and gyro.GetAngle() >= -45){
				myRobot.Drive(0.5, 0.6);
			}
			myRobot.Drive(0.5, 0);
			Wait(1.5);
			while(IsAutonomous() and gyro.GetAngle() >= 0){
				myRobot.Drive(0.5, -0.6);
			}
			//Vision Track
			myRobot.Drive(0, 0);
		}
		if (autoSelected == bluerighttoright) {
			// Drives forward and then turns 45 degrees to the right
			std::cout << "Running Test Autonomous 1" << std::endl;
			myRobot.SetSafetyEnabled(false);
			gyro.Reset();
			myRobot.Drive(0.5, 0);
			Wait(1.5);
			while(IsAutonomous() and gyro.GetAngle() > -45){
				myRobot.Drive(0.5, 0.6);
			}
			//Vision Track
			myRobot.Drive(0, 0);
		}
		else {
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
		double deadzone = 0.3;
		double XboxY;
		double XboxX;

		if(Xbox.GetX(XboxController::JoystickHand(0)) > deadzone || Xbox.GetX(XboxController::JoystickHand(0)) < -deadzone) {
			XboxX = Xbox.GetX(XboxController::JoystickHand(0));
		}else
		{
			XboxX = 0;
		}
		if(Xbox.GetY(XboxController::JoystickHand(0)) > deadzone || Xbox.GetY(XboxController::JoystickHand(0)) < -deadzone) {
			XboxY = Xbox.GetY(XboxController::JoystickHand(0));
		}else
		{
			XboxY = 0;
		}


		// drive with arcade style (use right stick)
		myRobot.ArcadeDrive( XboxY, XboxX/2, true);

		//myRobot.ArcadeDrive(Xbox.GetY(XboxController::JoystickHand(0)), Xbox.GetX(XboxController::JoystickHand(1)));

		// wait for a motor update time
		frc::Wait(0.005);



		//Kicker Piston for Gear
		if (Xbox.GetTriggerAxis(XboxController::JoystickHand(0)))
		{
			Kicker.Set(DoubleSolenoid::Value(1));
			Wait(0.005);
		}
		else
		{
			Kicker.Set(DoubleSolenoid::Value(2));
			Wait(0.005);
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
			Climber.Set(-1); //do you want this to be 100%?
		}else
		{
			Climber.Set(0);
		}

		//Climber Up
		if (Xbox.GetStartButton())
		{
			Climber.Set(1); //do you want this to be 100%?
		}else
		{
			Climber.Set(0);
		}

		//Shifter
		if (Xbox.GetXButton() && Shifter1.Get()!=0 && Shifter1.Get()!=1)
		{
			Shifter1.Set(DoubleSolenoid::Value(1));
			Shifter2.Set(DoubleSolenoid::Value(1));

			Wait(0.5);
		}
		else if (Xbox.GetXButton() && Shifter1.Get() != 2)
		{
			Shifter1.Set(DoubleSolenoid::Value(2));
			Shifter2.Set(DoubleSolenoid::Value(2));
			Wait(0.5);
		}

		 //Intake
		if (Xbox.GetYButton())
		{
			Intake.Set(-1); //do you want this to be 100%?
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
