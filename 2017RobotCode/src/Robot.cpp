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
#include <DoubleSolenoid.h>

/* Basic needs */
#include <CameraServer.h>
#include <XboxController.h>
#include <GenericHID.h>
#include <GamepadBase.h>

/*Lights*/
#include "I2C.h"


/* Vision */
#include <open_sockets.h>
#include <packets.h>
#include <inet.h>

//#define I2C_SLAVE_ADR 0x08 // ADXL345 I2C device address

class Robot: public SampleRobot {
	//Driving
	frc::RobotDrive myRobot { 3, 2, 1, 0}; //2, 3, 0, 1 is for Nigel Thronberry
	frc::XboxController Xbox { 0 };
	//Motors and Stuff
	frc::VictorSP Climber { 4 };
	frc::DoubleSolenoid Shifter1 { 0, 1 };
	frc::DoubleSolenoid Shifter2 { 2, 3 };
	//Add ons
	frc::ADXRS450_Gyro gyro;
	frc::ADXL362 accel;

	frc::SendableChooser<std::string> side;
	frc::SendableChooser<std::string> start;

	const std::string red = "Red";
	const std::string blue = "Blue";
	const std::string basic = "Go Forward";
	const std::string startright = "Right Start";
	const std::string startmid = "Middle Start";
	const std::string startleft = "Left Start";
	const std::string gearright = "Right Gear";
	const std::string gearmid = "Middle Gear";
	const std::string gearleft = "Left Gear";
	const std::string testing1 = "Testing1";
	const std::string testing2 = "Testing2";



	I2C *I2Channel;
	//Jetson




	public:
	Robot() {
		myRobot.SetExpiration(0.1);
	}




	void visionTrack(struct track_packet *Steven);
		  int					sockfd=0;             /* listen socket file descriptor */
		  struct sockaddr_in	cli_addr;           /* write-to-client socket address */
		  char                  inbuffer[MAXLINE];  /* incoming data */
		  unsigned short        packid = 0;         /* incoming packet ID */
		  int                   n=0;                  /* packet length */
		  struct track_packet   track;              /* a track packet */
		  int 					time;				/* time of packet*/


	void RobotInit() {

		//double boo = 0.3;
		side.AddDefault(red, red);
		side.AddObject(blue, blue);
		side.AddObject(basic, basic);
		start.AddDefault(startright, startright);
		start.AddObject(startmid, startmid);
		start.AddObject(startleft, startleft);
		start.AddObject(testing1, testing1);
		start.AddObject(testing2, testing2);
		frc::SmartDashboard::PutData("Color Selected", &side);
		frc::SmartDashboard::PutData("Start Position Selected", &start);
		frc::CameraServer::GetInstance()->StartAutomaticCapture();
		gyro.Reset();


		I2Channel = new I2C(I2C::kOnboard, I2C_SLAVE_ADR);



		/* Create Socket for getting data from Zed */
		if (open_serverside_socket(&sockfd)) {
			    fprintf(stderr,"Can't open socket\n");
				exit(1);
				} else {
				std::cout << "Socket opened on "<< sockfd << std::endl;
				}

		/*Start Zed on Jetson*/
				std::cout <<"...CONNECTING..." << std::endl;
				system("ssh ubuntu@tegra-ubuntu.local \"killall zed.sh zed > /home/ubuntu/zed.log 2>&1; /home/ubuntu/zed.sh >> /home/ubuntu/zed.log 2>&1 &\"");
				std::cout <<"...CONNECTED..." << std::endl;
	}

	/* Autonomous */









	void Autonomous() {



		auto Start = start.GetSelected();
		auto Color = side.GetSelected();
		// get heading
		std::cout << "Auto selected:Start: "<< Start << " Color: " << Color << std::endl;

		Shifter1.Set(DoubleSolenoid::Value(1));
		Shifter2.Set(DoubleSolenoid::Value(1));
		if (Color == blue){
			I2Channel->Write(I2C_SLAVE_ADR, 111);
		}
		else{
			I2Channel->Write(I2C_SLAVE_ADR, 114);
		}
		if ((Start == startleft) && (Color == blue)) {
			// Drives forward and then turns 45 degrees to the right
			std::cout << "Running Test Autonomous 1" << std::endl;
			myRobot.SetSafetyEnabled(false);
			myRobot.Drive(-0.5, 0.015);
			Wait(0.75);
			myRobot.Drive(0,0);
			Wait(0.5);
			gyro.Reset();
			while(IsAutonomous() && (gyro.GetAngle() <= 60)){
				frc::SmartDashboard::PutNumber("Angle", gyro.GetAngle());
				myRobot.Drive(-0.25, 0.8);
			}
			//Vision Track
			Wait(0.5);
			myRobot.Drive(-0.25,0.03);
			Wait(2);
			myRobot.Drive(0, 0);
		}
		else if((Start == startmid) && (Color == blue)) {
			// Drives forward
			std::cout << "Running Test Autonomous 1" << std::endl;
			myRobot.SetSafetyEnabled(false);
			gyro.Reset();
			myRobot.Drive(-0.25, 0.03);
			Wait(8);
			//Vision Track
			myRobot.Drive(0, 0);
		}
		else if((Start == startright) && (Color == blue)) {
			// Drives forward and then turns 45 degrees to the left
			std::cout << "Running Test Autonomous 1" << std::endl;
			myRobot.SetSafetyEnabled(false);
			myRobot.Drive(-0.5, 0.015);
			Wait(0.75);
			myRobot.Drive(0,0);
			Wait(0.5);
			gyro.Reset();
			while(IsAutonomous() && gyro.GetAngle() > -60){
				frc::SmartDashboard::PutNumber("Angle", gyro.GetAngle());
				myRobot.Drive(-0.25, -0.8);
			}
			Wait(0.5);
			//Vision Track
			myRobot.Drive(-0.25, 0.03);
			Wait(2);
			myRobot.Drive(0, 0);
		}
		else if((Start == startleft) && (Color == red)) {
					// Drives forward and then turns 45 degrees to the right
			std::cout << "Running Test Autonomous 1" << std::endl;
			myRobot.SetSafetyEnabled(false);
			myRobot.Drive(-0.5, 0.015);
			Wait(0.75);
			myRobot.Drive(0,0);
			Wait(0.5);
			gyro.Reset();
			while(IsAutonomous() && (gyro.GetAngle() <= 60)){
				frc::SmartDashboard::PutNumber("Angle", gyro.GetAngle());
				myRobot.Drive(-0.25, 0.8);
			}
			Wait(0.5);
			//Vision Track
			myRobot.Drive(-0.25,0.03);
			Wait(3);
			myRobot.Drive(0, 0);
		}
		else if((Start == startmid) && (Color == red)) {
			// Drives forward
			std::cout << "Running Test Autonomous 1" << std::endl;
			myRobot.SetSafetyEnabled(false);
			gyro.Reset();
			myRobot.Drive(-0.25, 0.03);
			Wait(8);
			//Vision Track
			myRobot.Drive(0, 0);
		}
		else if((Start == startright) && (Color == red)) {
			// Drives forward and then turns 45 degrees to the right
			std::cout << "Running Test Autonomous 1" << std::endl;
			myRobot.SetSafetyEnabled(false);
			myRobot.Drive(-0.5, 0.015);
			Wait(0.75);
			myRobot.Drive(0,0);
			Wait(0.5);
			gyro.Reset();
			while(IsAutonomous() && gyro.GetAngle() > -60){
				frc::SmartDashboard::PutNumber("Angle", gyro.GetAngle());
				myRobot.Drive(-0.25, -0.8);
			}
			Wait(0.5);
			//Vision Track
			myRobot.Drive(-0.25,0.03);
			Wait(2);
			myRobot.Drive(0, 0);
		}if ((Start == testing1 ) && (Color == testing1 )){

		}else {
			// Default Auto goes here
			std::cout << "Running default Autonomous" << std::endl;
			myRobot.SetSafetyEnabled(false);
			myRobot.Drive(-0.25, 0.0); // drive forwards half speed
			frc::Wait(2.0);                // for 2 seconds
			myRobot.Drive(0.0, 0.0);  // stop robot
		}
	}

	/*TELEOP*/



	void OperatorControl() override
	{
		int pixelPosition = 0;
		int count = 0;
		double deadzone = 0.3;
		double XboxY;
		double XboxX;

		myRobot.SetSafetyEnabled(true);
		while (IsOperatorControl() && IsEnabled())
		{
			/*Read info from zed socket */

		n = get_packet(sockfd, &cli_addr, inbuffer);

		 if (n>0) { // There was a message!

		memcpy(&packid,inbuffer,sizeof(track.packet_id));
		packid = ntohs(packid);
		/* deal with that packet */

		switch (packid) {
			case PID_TRACK:
			handle_track(inbuffer,n,&track);
				if (count%20 == 0) {
					std::cout <<"packet length : " << n << std::endl;
					std::cout <<"packet_id : " << track.packet_id << std::endl; /* print packet to stdout */
					std::cout <<"sequence : "<< track.sequence << std::endl;
					std::cout <<"range : "<< track.range << std::endl;
					std::cout <<"x1 : "<< track.x1 << std::endl;
					std::cout <<"x2 : "<< track.x2 << std::endl;
					 std::cout <<"time: "<< track.time << std::endl;
					 std::cout <<"\n"<< std::endl;
					 }
			//visionTrack(&track);

		break;
		default:
		fprintf(stderr,"Unknown packet type %d and length %d\n",packid,n);
		}

		//Vision Tracking for Gear
				if (Xbox.GetBumper(XboxController::JoystickHand(0)))
				{
					std::cout <<"...ACTIVATING..." << std::endl;
				    visionTrack(&track);
				}else
				{
					myRobot.ArcadeDrive(Xbox);
				}
		 }


		frc::SmartDashboard::PutNumber("Angle", gyro.GetAngle());

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

		// wait for a motor update time
		frc::Wait(0.005);
/*
	if(Xbox.GetAButton()){
		I2Channel->Write(I2C_SLAVE_ADR, 111);
}

	if(Xbox.GetBButton()){
		I2Channel->Write(I2C_SLAVE_ADR, 114);
}*/
	/*
switch(pixelPosition){
	case 0:
		I2Channel->Write(I2C_SLAVE_ADR, 111);
		break;
	case 1:
		I2Channel->Write(I2C_SLAVE_ADR, 114);
		break;
	case 2:
		I2Channel->Write(I2C_SLAVE_ADR, 103);
		break;
	case 3:
		I2Channel->Write(I2C_SLAVE_ADR, 98);
		break;
	case 4:
		I2Channel->Write(I2C_SLAVE_ADR, 117);
		break;
	case 5:
		I2Channel->Write(I2C_SLAVE_ADR, 99);
		break;
	case 6:
		I2Channel->Write(I2C_SLAVE_ADR, 104);
		break;
	case 7:
		I2Channel->Write(I2C_SLAVE_ADR, 116);
		break;
	case 8:
		I2Channel->Write(I2C_SLAVE_ADR, 112);
		break;
	case 9:
		I2Channel->Write(I2C_SLAVE_ADR, 115);
		break;
}*/

		//Shooter Trigger
		if (Xbox.GetTriggerAxis(XboxController::JoystickHand(1)))
		{
				Climber.Set(Xbox.GetTriggerAxis(XboxController::JoystickHand(1)));
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

		// read from zed socket
		n = get_packet(sockfd, &cli_addr, inbuffer);
		if (n>0) { // There was a message!

		/* Find packet type */

		memcpy(&packid,inbuffer,sizeof(track.packet_id));
		packid = ntohs(packid);
		count++;
		/* deal with that packet */
		switch (packid) {
			case PID_TRACK:
			handle_track(inbuffer,n,&track);
				if (count%20 == 0) {
				    std::cout <<"packet length : " << n << std::endl;
				    std::cout <<"packet_id : " << track.packet_id << std::endl; /* print packet to std::cout */
				    std::cout <<"sequence : "<< track.sequence << std::endl;
				    std::cout <<"range : "<< track.range << std::endl;
				    std::cout <<"x1 : "<< track.x1 << std::endl;
				    std::cout <<"x2 : "<< track.x2 << std::endl;
				    std::cout <<"time: "<< track.time << std::endl;
				    std::cout <<"\n"<< std::endl;
				}
				break;
				default:
				fprintf(stderr,"Unknown packet type %d and length %d\n",packid,n);
			}
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



void Robot::visionTrack(struct track_packet *Steven) {

//static unit x1


short avgx;  //average of the two contours

static const short midx = 366;
static const short deadzone = 45;

	avgx = (Steven->x1 + Steven->x2)/2;


	if (Steven->range <= 10 && Steven->range >= 100){
		myRobot.ArcadeDrive(0.0, 0.0);
		return;
	}

	if(avgx < (midx-deadzone)) //when average is less than the mid point turn left
	{
		myRobot.ArcadeDrive(-0.0,0.65); //check direction
		Wait(.05);
		std::cout <<"LEFT" << std::endl;
	}
	else if(avgx > (midx+deadzone))
	{
		myRobot.ArcadeDrive(-0.0, -0.65);
		Wait(.05);
		std::cout <<"RIGHT" << std::endl;
	}
	else if ((-deadzone < avgx) && (avgx > deadzone))
	{
		myRobot.ArcadeDrive(-0.5 ,0); //check direction
		Wait(.05);
		std::cout <<"FORWARD" << std::endl;
}
}
/*
packets.c
*/
void send_packet(int sockfd, struct sockaddr_in	*pserv_addr, char *buffer, int buffsiz)
{
  char	sendline[MAXLINE], recvline[MAXLINE + 1];
  int   n;

/* copy the data to be sent */

  memcpy(sendline,buffer,buffsiz);

/* Send the buffer */

  if (sendto(sockfd, sendline, buffsiz, 0, (struct sockaddr *) pserv_addr,
		  sizeof(*pserv_addr)) != buffsiz)
	  	  fprintf(stderr,"dg_cli: sendto error on socket\n");

  	  return;
}

/*
   This routine fills a track packet with data, and copies that data
   into the space at outbuffer.  The size of the output buffer is returned.
 */
int pack_track(char *outbuffer, struct track_packet *outpacket)
/*    char *outbuffer;               buffer to be filled */
/*    struct track_packet *outpacket; the data packet */
{
int buffsize;                     /* size of the buffer */

/* Fill packet to send */

/* convert to network byte order */

  outpacket->packet_id = htons(outpacket->packet_id);
  outpacket->sequence = htonl(outpacket->sequence);
  outpacket->range = htonl(outpacket->range);
  outpacket->x1 = htons(outpacket->x1);
  outpacket->x2 = htons(outpacket->x2);
  outpacket->time = htonl(outpacket->time);


/* pack send info into the buffer */

  buffsize = sizeof(*outpacket);
  memcpy(outbuffer,outpacket,buffsize);
  return(buffsize);
}


int get_packet(int sockfd, struct sockaddr_in *pcli_addr, char *mesg)
/*     int		sockfd;          the socket to listen on */
/*     struct sockaddr_in	*pcli_addr;	 ptr to client sockaddr structure */
/*     char	        *mesg;           input buffer */
{
  int	       n=1;
  socklen_t clilen;
  char readbuffer[MAXLINE+1];

/* read a buffer from the socket */

  clilen = sizeof(*pcli_addr);
/*while (n>0) { // read until we can't read anymore, to get freshest data
  n = (int)recvfrom(sockfd, readbuffer, MAXLINE, MSG_DONTWAIT, (struct sockaddr *)pcli_addr,
	       &clilen);
 if (n > 0) {
   memcpy(mesg,readbuffer,n);
 }
}*/
  n = (int)recvfrom(sockfd, mesg, MAXLINE, MSG_DONTWAIT, (struct sockaddr *)pcli_addr,
	       &clilen);

  if (n>0) mesg[n] = 0;           /* null terminate */
  //if (n>0) printf("From %s:\n",inet_ntoa(pcli_addr->sin_addr));
  return(n);
}

/* This routine deals with a track packet */

void handle_track(char *inbuffer, int n,struct track_packet *inpacket)
/*     char *inbuffer;               the input buffer */
/*    int n;                        the size of that buffer */
/*    struct track_packet *inpacket; a track packet */
{

/* Check that the packet size is correct, and fill the structure if it is */

  if (n != sizeof(*inpacket)) {
    fprintf(stderr,"Whoa!  %d is wrong packet size for this type (%d)\n",
	    n,sizeof(*inpacket));
    return;
  } else {
    memcpy(inpacket,inbuffer,n);
  }

/* Convert to local byte order */

  inpacket->packet_id = ntohs(inpacket->packet_id);
  inpacket->sequence = ntohl(inpacket->sequence);
  inpacket->range = ntohl(inpacket->range);
  inpacket->x1 = ntohs(inpacket->x1);
  inpacket->x2 = ntohs(inpacket->x2);
  inpacket->time = ntohl(inpacket->time);
}
/*
open_sockets.c \/
*/

int open_clientside_socket(int *sockfd, struct sockaddr_in *serv_addr)

{
  struct sockaddr_in	cli_addr;   /* socket for the client to listen on */
  int flag_on  = 1;
  int flag_off = 0;

  /*
   * Fill in the structure "serv_addr" with the address of the
   * server that we want to send to.
   */

  memset((char *) serv_addr, 0, sizeof(*serv_addr));
  serv_addr->sin_family      = AF_INET;
  serv_addr->sin_addr.s_addr = inet_addr(SERV_HOST_ADDR);
  serv_addr->sin_port        = htons(SERV_UDP_PORT);

  /*
   * Open a UDP socket (an Internet datagram socket).
   */

  if ( (*sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
    fprintf(stderr,"client: can't open datagram socket\n");
    return(-1);
  }

  /*
   * Bind a local address for the client to listen on.
   */

  memset((char *) &cli_addr, 0, sizeof(cli_addr));	/* zero out */
  cli_addr.sin_family      = AF_INET;
  cli_addr.sin_addr.s_addr = htonl(INADDR_ANY);
  cli_addr.sin_port        = htons(CLI_UDP_PORT);
  if (bind(*sockfd, (struct sockaddr *) &cli_addr, sizeof(cli_addr)) < 0) {
    fprintf(stderr,"client: can't bind local address\n");
    return(-1);
  }

/* make this socket non-blocking, so we can retry if packets get lost */
  if (ioctl(*sockfd,FIONBIO,&flag_on) < 0)
    fprintf(stderr,"FIONBIO error - can't set non-blocking flag on socket\n");
  return(0);
}


/* This routine opens up the socket needed to listen for the client */

int open_serverside_socket(int *sockfd)
{
  struct sockaddr_in sock_addr;      /* the socket address */

  /*
   * Open a UDP socket (an Internet datagram socket).
   */

  if ( (*sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
    fprintf(stderr,"server: can't open datagram socket");
    return(-1);
  }

  /*
   * Bind our local address so that the client can send to us.
   */

  memset((char *) &sock_addr, 0, sizeof(sock_addr));
  sock_addr.sin_family      = AF_INET;
  sock_addr.sin_addr.s_addr = htonl(INADDR_ANY);
  sock_addr.sin_port        = htons(SERV_UDP_PORT);

  if (bind(*sockfd,
	   (struct sockaddr *) &sock_addr, sizeof(sock_addr)) < 0) {
    fprintf(stderr,"server: can't bind local address\n");
    return(-1);
  }

  return(0);
}


START_ROBOT_CLASS(Robot)
