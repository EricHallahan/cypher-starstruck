#pragma config(UART_Usage, UART2, uartNotUsed, baudRate4800, IOPins, None, None)
#pragma config(I2C_Usage, I2C1, i2cSensors)
#pragma config(Sensor, in1,    AP,             sensorPotentiometer)
#pragma config(Sensor, in2,    CP,             sensorPotentiometer)
#pragma config(Sensor, in3,    SP,             sensorPotentiometer)
#pragma config(Sensor, in4,    OP,             sensorNone)
#pragma config(Sensor, in5,    G,              sensorGyro)
#pragma config(Sensor, I2C_1,  FLE,            sensorQuadEncoderOnI2CPort,    , AutoAssign )
#pragma config(Sensor, I2C_2,  RLE,            sensorQuadEncoderOnI2CPort,    , AutoAssign )
#pragma config(Sensor, I2C_3,  RRE,            sensorQuadEncoderOnI2CPort,    , AutoAssign )
#pragma config(Sensor, I2C_4,  FRE,            sensorQuadEncoderOnI2CPort,    , AutoAssign )
#pragma config(Sensor, I2C_5,  HE,             sensorQuadEncoderOnI2CPort,    , AutoAssign )
#pragma config(Motor,  port1,           FL,            tmotorVex393HighSpeed_HBridge, openLoop, driveLeft, encoderPort, I2C_1)
#pragma config(Motor,  port2,           FR,            tmotorVex393HighSpeed_MC29, openLoop, reversed, driveRight, encoderPort, I2C_2)
#pragma config(Motor,  port3,           RL,            tmotorVex393HighSpeed_MC29, openLoop, driveLeft, encoderPort, I2C_4)
#pragma config(Motor,  port4,           RR,            tmotorVex393HighSpeed_MC29, openLoop, reversed, driveRight, encoderPort, I2C_3)
#pragma config(Motor,  port5,           H,             tmotorVex393HighSpeed_MC29, openLoop, reversed, encoderPort, I2C_5)
#pragma config(Motor,  port6,           C,             tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port7,           L,             tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port8,           R,             tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port9,           E,             tmotorVex393_MC29, openLoop)
#pragma config(DatalogSeries, 1, "", Sensors, Sensor, in1, 50)
#pragma config(DatalogSeries, 2, "", Sensors, Sensor, in2, 50)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*        Description: Competition template for VEX EDR                      */
/*                                                                           */
/*---------------------------------------------------------------------------*/

// This code is for the VEX cortex platform
#pragma platform(VEX2)

// Select Download method as "competition"
#pragma competitionControl(Competition)

//Main competition background code...do not modify!
#include "Vex_Competition_Includes.c"

//Create holonomic drive registry
int motorRegistery[3][4];

//Define lift variables
float KpLift = 0.11;
int liftTarget = 3500;
int liftDown = 3500;
int liftUp = 2800;
int liftOver = 850;
bool liftEnable = false;

float KpClaw = 0.30;
int clawTarget = 2800;
int clawOpen = 1000;
int clawClose = 1850;
bool clawEnable = false;

float E_in = 3.25*PI/392;

///////////////////////////////////////////////////////////////////////////////////////////////////

void encoderReset()
{
	SensorValue[FLE] = 0;
	SensorValue[RLE] = 0;
	SensorValue[RRE] = 0;
	SensorValue[FRE] = 0;
	SensorValue[HE] = 0;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

void leftDrive(int power) // Set Left Drivetrain motors
{
	motor[FL] = power;
	motor[RL] = power;
}
void rightDrive(int power) //Set Right Drivetrain Motors
{
	motor[FR] = power;
	motor[RR] = power;
}
void midDrive(int power) //Set Center Drivetrain Motor
{
	motor[H] = power;
}
void liftDrive(int power) //Set Lift Power
{
	motor[L] = power;
	motor[R] = power;
	motor[E] = power;
}
void clawDrive(int power) //Set Claw Power
{
	motor[C] = power;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

void liftSet(int location) //Set Lift Location
{
	liftTarget = location;
}
void clawSet(int location) //Set Claw Location
{
	clawTarget = location;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

void leftDriveRegistry(byte motorSpeed) //Set Left Drivetrain Planar Component
{
	motorRegistery[1][0] = motorSpeed;
}

void rightDriveRegistry(byte motorSpeed) //Set Right Drivetrain Planar Component
{
	motorRegistery[1][1] = motorSpeed;
}

void midDriveRegistry(byte motorSpeed) //Set Center Drivetrain Planar Component
{
	motorRegistery[1][2] = motorSpeed;
}

void rotateDrive(byte motorSpeed) //Set Drivetrain Angular Component
{
	motorRegistery[2][0] = motorSpeed;
	motorRegistery[2][1] = -motorSpeed;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

void forwardDrive(byte motorSpeed) //Set both Left and Right Registry
{
	leftDriveRegistry(motorSpeed);
	rightDriveRegistry(motorSpeed);
}

void sideDrive(byte motorSpeed) //Set Middle Registry
{
	midDriveRegistry(motorSpeed);
}

///////////////////////////////////////////////////////////////////////////////////////////////////

void yDriveDistance(float distance, float speed) //
{
	encoderReset();
	if(SensorValue[FLE] < distance)
	{
		while(SensorValue[FLE] < distance)
		{
			motorRegistery[1][0] = speed;
			motorRegistery[1][1] = speed;
			wait1Msec(20);
		}
	}
	else
	{
		while(SensorValue[FLE] > distance)
		{
			motorRegistery[1][0] = -speed;
			motorRegistery[1][1] = -speed;
			wait1Msec(20);
		}
	}
	motorRegistery[1][0] = 0;
	motorRegistery[1][1] = 0;
}

void xDriveDistance(float distance, float speed)
{
	while(SensorValue[HE] < (392*(distance/(3.25*PI))))
	{
		motorRegistery[1][2] = speed;
	}
	while(SensorValue[HE] > (392*(distance/(3.25*PI))))
	{
		motorRegistery[1][2] = -speed;
	}
	motorRegistery[1][2] = 0;
}

void turnLeftDegrees(int degrees)
{
	int motorSpeed = 127;

	//While the absolute value of the gyro is less than the desired rotation...
	while(abs(SensorValue[G]) > degrees)
	{
		//...continue turning
		rotateDrive(-motorSpeed);
	}

	//Brief brake to stop some drift
	rotateDrive(-motorSpeed);
	wait1Msec(200);
	rotateDrive(0);
	wait1Msec(100);
}

void turnRightDegrees(int degrees)
{
	int motorSpeed = 127;

	//While the absolute value of the gyro is less than the desired rotation...
	while(abs(SensorValue[G]) < degrees)
	{
		//...continue turning
		rotateDrive(motorSpeed);
	}

	//Brief brake to stop some drift
	rotateDrive(-motorSpeed);
	wait1Msec(200);
	rotateDrive(0);
	wait1Msec(100);
}

///////////////////////////////////////////////////////////////////////////////////////////////////

task setMotors //Set motor powers from motorMath
{
	while(true)
	{
		leftDrive(motorRegistery[0][0]);
		rightDrive(motorRegistery[0][1]);
		midDrive(motorRegistery[0][2]);
		wait1Msec(20);
	}
}

task motorMath //Combine the linear and angular components
{
	while(true)
	{
		motorRegistery[0][0] = (motorRegistery[1][0] + motorRegistery[2][0]);//
		motorRegistery[0][1] = (motorRegistery[1][1] + motorRegistery[2][1]);//
		motorRegistery[0][2] = (motorRegistery[1][2] + motorRegistery[2][2]);//
		motorRegistery[0][3] = (motorRegistery[1][3] + motorRegistery[2][3]);//
		wait1Msec(20);
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////

task clawPID()
{
	int clawError = 0;
	int clawOutput = 0;

	while( true )
	{
		if(clawEnable)
		{
			// Read the sensor value and scale
			clawError = clawTarget - SensorValue[CP];

			// calculate drive
			clawOutput = (KpClaw * clawError);

			// limit drive
			if (clawOutput > 127)
			{
				clawOutput = 127;
			}

			if (clawOutput < -127)
			{
				clawOutput = -127;
			}

			clawOutput = abs(clawOutput) > 20 ? clawOutput : 0;
			clawDrive(clawOutput);
		}
		else
		{
			clawDrive(0);
			clawError = 0;
			clawOutput = 0;
		}

		// Run at 50Hz
		wait1Msec( 25 );
	}
}


task liftPID()
{
	int liftError = 0;
	int liftOutput = 0;

	while(true)
	{
		if(liftEnable)
		{

			// Read the sensor value and scale
			liftError = liftTarget - SensorValue[AP];

			// calculate drive
			liftOutput = (KpLift * liftError);

			// limit drive
			if (liftOutput > 127)
			{
				liftOutput = 127;
			}

			if (liftOutput < -127)
			{
				liftOutput = -127;
			}

			liftOutput = abs(liftOutput) > 20 ? liftOutput : 0;
			liftDrive(liftOutput);
		}
		else
		{
			liftDrive(0);
			liftError = 0;
			liftOutput = 0;
		}

		// Run at 50Hz
		wait1Msec( 25 );
	}
}

void dump()
{
	while(SensorValue(AP) > 1950)
	{
		wait1Msec(1);
	}
	clawSet(clawOpen);
}


task dumpTask()
{
	dump();
}
///////////////////////////////////////////////////////////////////////////////////////////////////


/*---------------------------------------------------------------------------*/
/*                          Pre-Autonomous Functions                         */
/*                                                                           */
/*  You may want to perform some actions before the competition starts.      */
/*  Do them in the following function.  You must return from this function   */
/*  or the autonomous and usercontrol tasks will not be started.  This       */
/*  function is only called once after the cortex has been powered on and    */
/*  not every time that the robot is disabled.                               */
/*---------------------------------------------------------------------------*/

void pre_auton()
{
	encoderReset();
	wait1Msec(1000);
	SensorType[in5] = sensorNone;
	wait1Msec(100);
	SensorType[in5] = sensorGyro;
	wait1Msec(1000);
}

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                              Autonomous Task                              */
/*                                                                           */
/*  This task is used to control your robot during the autonomous phase of   */
/*  a VEX Competition.                                                       */
/*                                                                           */
/*  You must modify the code to add your own robot specific commands here.   */
/*---------------------------------------------------------------------------*/

task autonomous()
{
	startTask(motorMath);
	startTask(setMotors);
	startTask(liftPID);
	startTask(clawPID);

	if(SensorValue[SP] < 900) //Left
	{
		if(SensorValue[OP] < 1350) //Cube
		{
			liftEnable = true;
			clawEnable = true;
			liftSet(liftUp);
			clawSet(clawOpen);
			wait1Msec(100);
			yDriveDistance(800, 127);
			turnRightDegrees(900);
			liftSet(liftDown);
			yDriveDistance(1000, 127);
			clawSet(clawClose);
			wait1Msec(300);
			liftSet(liftUp);
			turnRightDegrees(1800);
			yDriveDistance(-100,127);
			liftSet(liftOver);
			wait1Msec(1000);
			clawSet(clawOpen);
			wait1Msec(1000);
			liftSet(liftUp);
		}
	}

	if(SensorValue[SP] >= 900 && SensorValue[SP] <= 1800) //Skills
	{
		liftEnable = true;                  //Activate arm
		clawEnable = true;                  //Activate claw
		yDriveDistance(-800, 127);          //Back up
		liftSet(liftUp);                    //Lift the arm to protect the claw
		clawSet(clawOpen);                  //Open the claw for claw deployment
		wait1Msec(100);                     //Pause for claw deployment
		liftSet(liftDown);                  //Lower the arm for "Phase I - Driver Loads"

		/*"Phase I - Driver Loads"*/
		for(int n	 = 0; n<3; n++)           //Run 3 times
		{
			wait1Msec(1000);									//Pause for Load placement
			clawSet(clawClose);								//Grab driver load
			wait1Msec(300);										//Pause
			liftSet(liftUp);									//Lift driver load
			yDriveDistance(round(-800), 127);	//Back up
			liftSet(liftOver);								//Start lifting motion
			dump(); 													//Dump
			wait1Msec(500);										//Pause for Dump Completion
			liftSet(liftDown);								//Lower the arm
			wait1Msec(300);										//Pause for arm reset
			yDriveDistance(800, 127);					//Drive forward
			clawSet(clawOpen);								//Open claw for next load
		}
	}

	if(SensorValue[SP] > 1800) //Right
	{
		if(SensorValue[OP] < 1350) //Cube
		{
			liftEnable = true;
			clawEnable = true;
			liftSet(liftUp);
			clawSet(clawOpen);
			wait1Msec(100);
			yDriveDistance(800, 127);
			turnLeftDegrees(900);
			liftSet(liftDown);
			yDriveDistance(1000, 127);
			clawSet(clawClose);
			wait1Msec(300);
			liftSet(liftUp);
			turnLeftDegrees(1800);
			yDriveDistance(-100,127);
			liftSet(liftOver);
			wait1Msec(1000);
			clawSet(clawOpen);
			wait1Msec(1000);
			liftSet(liftUp);
		}
	}
}


/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                              User Control Task                            */
/*                                                                           */
/*  This task is used to control your robot during the user control phase of */
/*  a VEX Competition.                                                       */
/*                                                                           */
/*  You must modify the code to add your own robot specific commands here.   */
/*---------------------------------------------------------------------------*/

task usercontrol()
{
startTask(motorMath);
startTask(setMotors);
startTask(liftPID);
startTask(clawPID);

while(true)
{
	rotateDrive(abs(vexRT[Ch1]) > 20 ? vexRT[Ch1] : 0); //Right Joystick Horizontal - Rotation
	forwardDrive(abs(vexRT[Ch3]) > 20 ? vexRT[Ch3] : 0); //Left Joystick Vertical - Forward
	sideDrive(abs(vexRT[Ch4]) > 20 ? vexRT[Ch4] : 0); //Left Joystick Horizontal - Sideways

	/////////////////////////////////////////////////////////////////////////////////////

	if(abs(vexRT[Ch2Xmtr2]) > 10) //Controller 2 Vertical - Manual Lift
	{
		int x;
		x = liftTarget + round(-vexRT[Ch2Xmtr2] * 0.04);
		if(liftDown < x < liftOver)
		{
			liftTarget = x;
		}
		if(x <= liftOver)
		{
			liftTarget = liftOver;
		}
		if(x >= liftDown)
		{
			liftTarget = liftDown;
		}
	}

	if(vexRT(Btn5DXmtr2) == true) //Manual Claw Close
	{
		clawSet(clawClose);
	}
	if(vexRT(Btn5UXmtr2) == true) //Manual Claw Open
	{
		clawSet(clawOpen);
	}

	/////////////////////////////////////////////////////////////////////////////////

	if(vexRT(Btn7D) == true || vexRT(Btn7DXmtr2) == true)
	{
		liftEnable = false;
		clawEnable = false;
	}

	if(vexRT(Btn8DXmtr2) == true) //Macro - Down
	{
		liftEnable = true;
		clawEnable = true;
		liftSet(liftDown);
		clawSet(clawOpen);
	}

	if(vexRT(Btn8RXmtr2) == true) //Macro - Lift
	{
		liftEnable = true;
		clawEnable = true;
		clawSet(clawClose);
		wait1Msec(300);
		liftSet(liftUp);
	}

	if(vexRT(Btn8UXmtr2) == true && vexRT(Btn6UXmtr2) == true || vexRT(Btn8UXmtr2) == true && vexRT(Btn6DXmtr2) == true) //Macro - Dump
	{
		liftEnable = true;
		clawEnable = true;
		liftSet(liftOver);
		startTask(dumpTask);
	}
}
}
