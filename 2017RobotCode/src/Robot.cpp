#include <iostream>
#include <fstream>
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
#include <ADXL362.h>
#include <Spark.h>
#include <NetworkTables/NetworkTable.h>
#include <DoubleSolenoid.h>
#include <CameraServer.h>
#include <XboxController.h>
#include <GenericHID.h>
#include <GamepadBase.h>
/*
struct ZedBarf{
	float distance;
	uint x;
	uint y;
	uint id;
} barf;
*/

class Robot: public SampleRobot {
	//Driving
	frc::RobotDrive myRobot { 2, 3, 0, 1};
	frc::Joystick stick { 1 };
	frc::XboxController Xbox { 0 };
	//Motors and Stuff
	frc::VictorSP Kicker { 0 };
	frc::VictorSP Shooter { 1 };
	frc::VictorSP Climber { 2 };
	frc::VictorSP Intake { 5 };
	frc::VictorSP Agitator { 4 };
	frc::DoubleSolenoid Shifter1 { 0, 1 };
	frc::DoubleSolenoid Shifter2 { 2, 3 };
	//Add ons
	frc::ADXRS450_Gyro gyro;

	frc::SendableChooser<std::string> side;
	frc::SendableChooser<std::string> start;

	const std::string red = "Red";
	const std::string blue = "Blue";
	const std::string basic = "Go Forward";
	const std::string startright = "Right Start";
	const std::string startmid = "Middle Start";
	const std::string startleft = "Left Start";
	//Jetson



	public:
	std::shared_ptr<NetworkTable> Steven;
	Robot() {
		Steven = NetworkTable::GetTable("SmartDashboard");//Change database to the location of the vision code
		myRobot.SetExpiration(0.1);
	}

	void RobotInit() {

		//double boo = 0.3;
		side.AddDefault(red, red);
		side.AddObject(blue, blue);
		side.AddObject(basic, basic);
		start.AddDefault(startright, startright);
		start.AddObject(startmid, startmid);
		start.AddObject(startleft, startleft);
		frc::SmartDashboard::PutData("Color Selected", &side);
		frc::SmartDashboard::PutData("Start Position Selected", &start);
		frc::CameraServer::GetInstance()->StartAutomaticCapture();
		gyro.Reset();
		/*
		Steven->SetTeam(5690);
		Steven->Initialize();
		Steven->SetUpdateRate(0.3);
		Steven->SetPort(5800);
		Steven->SetIPAddress("10.56.90.2");
		*/


	}

	//AUTONOMUS
	void Autonomous() {
		//std::string NTtableTest = Steven->GetString("ExampleDAB","");
		//std::cout << "This is the line your looking for" << NTta2bleTest << std::endl;

		// /home/admin/tracking

		auto Start = start.GetSelected();
		auto Color = side.GetSelected();
		// get heading
		std::cout << "Auto selected:Start: "<< Start << " Color: " << Color << std::endl;


		if ((Start == startleft) && (Color == blue)) {
			// Drives forward and then turns 45 degrees to the right
			std::cout << "Running Test Autonomous 1" << std::endl;
			myRobot.SetSafetyEnabled(false);
			gyro.Reset();
			myRobot.Drive(-0.5, 0.1);
			Wait(1);
			myRobot.Drive(0,0);
			Wait(0.25);
			while(IsAutonomous() && (gyro.GetAngle() <= 45)){
				myRobot.Drive(-0.25, 0.6);
			}
			//Vision Track
			myRobot.Drive(-0.25,0.1);
			Wait(2);
			myRobot.Drive(0, 0);
		}
		else if((Start == startmid) && (Color == blue)) {
			// Drives forward
			std::cout << "Running Test Autonomous 1" << std::endl;
			myRobot.SetSafetyEnabled(false);
			gyro.Reset();
			myRobot.Drive(-0.25, 0.1);
			Wait(2);
			//Vision Track
			myRobot.Drive(0, 0);
		}
		else if((Start == startright) && (Color == blue)) {
			// Drives forward and then turns 45 degrees to the left
			std::cout << "Running Test Autonomous 1" << std::endl;
			myRobot.SetSafetyEnabled(false);

			gyro.Reset();
			myRobot.Drive(-0.5, 0.1);
			Wait(1);
			myRobot.Drive(0,0);
			Wait(0.25);
			while(IsAutonomous() && gyro.GetAngle() > -45){
				myRobot.Drive(-0.25, -0.6);
			}
			//Vision Track
			myRobot.Drive(-0.25, 0.1);
			Wait(2);
			myRobot.Drive(0, 0);
		}
		else if((Start == startleft) && (Color == red)) {
					// Drives forward and then turns 45 degrees to the right
			std::cout << "Running Test Autonomous 1" << std::endl;
			myRobot.SetSafetyEnabled(false);
			gyro.Reset();
			myRobot.Drive(-0.5, 0.1);
			Wait(1);
			myRobot.Drive(0,0);
			Wait(0.25);
			while(IsAutonomous() && (gyro.GetAngle() <= 45)){
				myRobot.Drive(-0.25, 0.6);
			}
			//Vision Track
			myRobot.Drive(-0.25,0.1);
			Wait(2);
			myRobot.Drive(0, 0);
		}
		else if((Start == startmid) && (Color == red)) {
			// Drives forward
			std::cout << "Running Test Autonomous 1" << std::endl;
			myRobot.SetSafetyEnabled(false);
			gyro.Reset();
			myRobot.Drive(-0.25, 0.1);
			Wait(2);
			//Vision Track
			myRobot.Drive(0, 0);
		}
		else if((Start == startright) && (Color == red)) {
			// Drives forward and then turns 45 degrees to the right
			std::cout << "Running Test Autonomous 1" << std::endl;
			myRobot.SetSafetyEnabled(false);
			gyro.Reset();
			myRobot.Drive(-0.5, 0.1);
			Wait(1);
			myRobot.Drive(0,0);
			Wait(0.25);
			while(IsAutonomous() && gyro.GetAngle() > -45){
				myRobot.Drive(-0.25, -0.6);
			}
			//Vision Track
			myRobot.Drive(-0.25,0.1);
			Wait(2);
			myRobot.Drive(0, 0);
		}
		else {
			// Default Auto goes here
			std::cout << "Running default Autonomous" << std::endl;
			myRobot.SetSafetyEnabled(false);
			myRobot.Drive(-0.25, 0.0); // drive forwards half speed
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
		myRobot.ArcadeDrive( XboxY, XboxX/1.5, true);

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
		if (Xbox.GetTriggerAxis(XboxController::JoystickHand(1)))
		{
			Agitator.Set(Xbox.GetTriggerAxis(XboxController::JoystickHand(1)));
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
			Intake.Set(0.75); //do you want this to be 100%?
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
