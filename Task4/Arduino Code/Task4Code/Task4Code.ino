#include <Wire.h>
#include <Zumo32U4.h>
#include "TurnSensor.h"

Zumo32U4Motors motors;
Zumo32U4IMU imu;
Zumo32U4LineSensors lineSensors;

unsigned int lineSensorValues[5]; //Creates an empty array to hold the multiple line sensor values
int16_t lastError = 0;
const uint16_t maxSpeed = 200;  //Max speed the robot can reach
bool zumoAuto = false;  //A boolean variable to be used as a flag for whether the zumo is currentley automatically navigating

void setup() {
  Serial1.begin(9600);  //Open serial connection for the XBee
  lineSensors.initFiveSensors();  //Sets up the array for use with the zumos line sensor values
  calibrateSensors(); //Calls the function calibrateSensors, used for the setup of the Zumos line sensors
  turnSensorSetup(); //Calls the function turnSensorSetup, used to setup the gyro for turning
}


//Function to calibrate the line sensors before navigating through the maze, The zumo will rotate so the line sensors will be able to detect what is floor and what is a wall of the maze, Code below is used from "LineFollower" Zumo32U4 Example
void calibrateSensors()
{
  delay(1000);
  for(uint16_t i = 0; i < 120; i++)
  {
    if (i > 30 && i <= 90)
    {
      motors.setSpeeds(-150, 150);
    }
    else
    {
      motors.setSpeeds(150, -150);
    }

    lineSensors.calibrate();
  }
  motors.setSpeeds(0, 0);
}


void loop() {
  int input = Serial1.read();   //Read in the value that has been sent over the XBee and store it as a variable
  turnSensorReset();  //Resets the turn sensors at the start of each loop, makes sure it still turns the same amount even after turning multiple times on one route
  if (zumoAuto == false){
    
    switch (input){
      case'G':
      zumoAuto = true;  //Sets the Zumo's Automatic navigation flag to true
      motors.setSpeeds(200, 200);  //Sets the Zumo into motion
      break;
      
      case'L':
      motors.setSpeeds(-150, 150);
      while((int32_t)turnAngle < turnAngle90)//I use the zumos gyro scope to make sure every turn is exactley 90 degrees clockwise/anti-clockwise
      {
        turnSensorUpdate();
      }
      zumoAuto = true;  //Sets the Zumo's Automatic navigation flag to true
      motors.setSpeeds(200, 200);  //Sets the Zumo into motion
      break;
    
      case'R':
      motors.setSpeeds(150, -150);
      while((int32_t)turnAngle > -turnAngle90)//I use the zumos gyro scope to make sure every turn is exactley 90 degrees clockwise/anti-clockwise
      {
        turnSensorUpdate();
      }
      zumoAuto = true;  //Sets the Zumo's Automatic navigation flag to true
      motors.setSpeeds(200, 200);  //Sets the Zumo into motion
      break;
    }
  }
 
  //If the Zumo is currentley auto navigating then certain checks need to run on loop, Code below is used from "LineFollower" Zumo32U4 Example
  if (zumoAuto == true){
    int16_t position = lineSensors.readLine(lineSensorValues,QTR_EMITTERS_ON,1);  //The zumo defaults to running along a black line avoiding white boundaries, this has been inversed for the map we recieved (White line, black boundaries)
    int16_t error = position - 2000;
    int16_t speedDifference = error / 4 + 6 * (error - lastError);
    lastError = error;
    int16_t leftSpeed = (int16_t)maxSpeed + speedDifference;
    int16_t rightSpeed = (int16_t)maxSpeed - speedDifference;
    leftSpeed = constrain(leftSpeed, 0, (int16_t)maxSpeed); //These lines of code will stop a track on one side of the zumo if a wall is detected to make a small turn away from it, keeping it within the corridor
    rightSpeed = constrain(rightSpeed, 0, (int16_t)maxSpeed);
    motors.setSpeeds(leftSpeed, rightSpeed);

    if (lineSensorValues[2] > 150){ //This if detects whether the front central sensor on the zumo detects a line, in which case we have hit the end of a corridor
      motors.setSpeeds(0, 0);  //The zumo stops
      zumoAuto = false; // Automatic navigation exits and the zumo awaits its next response from the GUI
      Serial1.println("Robot reached the end of corridor, choose which way you would like to turn using the L and R buttons!"); //This sends a message to the text box on the GUI, instructing the user on how to continue
    }
  }
}
