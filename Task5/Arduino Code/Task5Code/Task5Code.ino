#include <Wire.h>
#include <Zumo32U4.h>
#include "TurnSensor.h"

Zumo32U4Encoders encoders;
Zumo32U4LCD lcd;
Zumo32U4Motors motors;
Zumo32U4IMU imu;
Zumo32U4LineSensors lineSensors;
Zumo32U4ProximitySensors proxSensors;

unsigned int lineSensorValues[5]; //Creates an empty array to hold the multiple line sensor values
int16_t lastError = 0;
const uint16_t maxSpeed = 200;    //Max speed the robot can reach
bool zumoAuto = false;  //A boolean variable to be used as a flag for whether the zumo is currentley automatically navigating
bool zumoRoom = false;  //A boolean variable to be used as a flag for whether the zumo is currentley in room checking mode

int roomValues[4][3]; //This will be used to store room number [0], room direction (0 = L / 1 = R)[1] and room empty? (0 = empty / 1 Occupied)[2]
int roomsVisited = 0; //This will be used as a counter for the number of rooms we have entered

void setup() {
  Serial1.begin(9600);            //Open serial connection for the XBee
  lineSensors.initFiveSensors();  //Sets up the array for use with the zumos line sensor values
  calibrateSensors();             //Calls the function calibrateSensors, used for the setup of the Zumos line sensors
  proxSensors.initThreeSensors(); //Sets up the array for use with zumos proximity sensors
  turnSensorSetup();              //Calls the function turnSensorSetup, used to setup the gyro for turning
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
  int leftEncoders; //Creates variables to store the wheel encoders
  int rightEncoders;
  leftEncoders = encoders.getCountsAndResetLeft();  //Resets the encoders at the begininng of each loop  
  rightEncoders = encoders.getCountsAndResetRight(); 
  
  int input = Serial1.read();   //Read in the value that has been sent over the XBee and store it as a variable
  turnSensorReset();            //Resets the turn sensors at the start of each loop, makes sure it still turns the same amount even after turning multiple times on one route

  //When inside this IF statement we are in room navigation mode
  if (zumoRoom == true && zumoAuto == false){
    switch (input){
      case'L':
      motors.setSpeeds(-150, 150);
      while((int32_t)turnAngle < turnAngle90) //I use the zumos gyro scope to make sure every turn is exactley 90 degrees clockwise/anti-clockwise
      {
        turnSensorUpdate();
      }
      leftEncoders = encoders.getCountsAndResetLeft();  //Here i reset the encoders so that they can be used to measure how far the zumo enters into the room
      rightEncoders = encoders.getCountsAndResetRight();
      motors.setSpeeds(100, 100);
      while (lineSensorValues[2] < 150 && proxSensors.countsFrontWithLeftLeds() < 6 && proxSensors.countsFrontWithRightLeds() < 6){ //This while loop continuously checks both the line and proximity sensors and stops the zumo once it hits a wall or detects an object inside the room
        lineSensors.readLine(lineSensorValues,QTR_EMITTERS_ON,1);
        proxSensors.read();
        }
      motors.setSpeeds(0, 0);
      
      if (lineSensorValues[2] > 150 ){  //We go into this if statement if the zumo was stopped by its line sensors
        motors.setSpeeds(-100, -100);   
        do {  //This do while loop reverses the zumo until it's encoders return to zero (it returns to its starting position)
        leftEncoders = encoders.getCountsLeft();
        rightEncoders = encoders.getCountsRight();
        }  while(leftEncoders > 0 && rightEncoders > 0);
        motors.setSpeeds(0,0);
        delay(1000);  //This delay makes sure that the zumo has returned to its starting position before any more code is executed (prevents the zumo from being interrupted when returning to starting position)
        
        roomValues[roomsVisited][0] = roomsVisited + 1; //The following three lines of code are for storing the values of the room by adding them to an array (Room number, Directrion, room occupied)
        roomValues[roomsVisited][1] = 0;
        roomValues[roomsVisited][2] = 0;
        roomsVisited += 1;  //Increments the room counter to keep track of how many rooms we have visited
        String message1 = "Checked room number: ";  //Due to the way concatonation works in arduino i have to add the strings together before printing the final message to prevent errors
        String message2 = message1 + roomsVisited;
        String messageFull = message2 + " to the left of the Zumo. This room was empty. Please rotate the zumo and use the GO! button to continue.";
        Serial1.println(messageFull);
      }
      else{ //This else statement means that the zumo was stopped because the proximity sensors detected an item
        motors.setSpeeds(-100, -100);
        do {
        leftEncoders = encoders.getCountsLeft();
        rightEncoders = encoders.getCountsRight();
        }  while(leftEncoders > 0 && rightEncoders > 0);
        motors.setSpeeds(0,0);
        delay(1000);
        roomValues[roomsVisited][0] = roomsVisited + 1;
        roomValues[roomsVisited][1] = 0;
        roomValues[roomsVisited][2] = 1;
        roomsVisited += 1;          
        String message1 = "Checked room number: ";
        String message2 = message1 + roomsVisited;
        String messageFull = message2 + " to the left of the Zumo. This room was populated. Please rotate the zumo and use the GO! button to continue.";
        Serial1.println(messageFull);
        }
        
      zumoRoom = false;
      input = ""; //Since L and R are used in other areas of the code i make sure to reset the input variable after this segment of code has executed (prevents an automatic Left or Right turn taking place)
      break;
    
      case'R':  //Repeat of the above code but turned to the right
      motors.setSpeeds(150, -150);
      while((int32_t)turnAngle > -turnAngle90)  //I use the zumos gyro scope to make sure every turn is exactley 90 degrees clockwise/anti-clockwise
      {
        turnSensorUpdate();
      }
      leftEncoders = encoders.getCountsAndResetLeft();
      rightEncoders = encoders.getCountsAndResetRight();
      motors.setSpeeds(100, 100);
      proxSensors.read();
      lineSensors.readLine(lineSensorValues,QTR_EMITTERS_ON,1);
      while (lineSensorValues[2] < 150 && proxSensors.countsFrontWithLeftLeds() < 6 && proxSensors.countsFrontWithRightLeds() < 6){
        lineSensors.readLine(lineSensorValues,QTR_EMITTERS_ON,1);
        proxSensors.read();
        }
      motors.setSpeeds(0, 0);
      if (lineSensorValues[2] > 150 ){
        motors.setSpeeds(-100, -100);
        do {
        leftEncoders = encoders.getCountsLeft();
        rightEncoders = encoders.getCountsRight();
        }  while(leftEncoders > 0 && rightEncoders > 0);
        motors.setSpeeds(0,0);
        delay(1000);  
        roomValues[roomsVisited][0] = roomsVisited + 1;
        roomValues[roomsVisited][1] = 1;
        roomValues[roomsVisited][2] = 0;
        roomsVisited += 1;
        String message1 = "Checked room number: ";
        String message2 = message1 + roomsVisited;
        String messageFull = message2 + " to the right of the Zumo. This room was empty. Please rotate the zumo and use the GO! button to continue.";
        Serial1.println(messageFull);
      }
      else{
        motors.setSpeeds(-100, -100);
        do {
        leftEncoders = encoders.getCountsLeft();
        rightEncoders = encoders.getCountsRight();
        }  while(leftEncoders > 0 && rightEncoders > 0);
        motors.setSpeeds(0,0);
        delay(1000);
        roomValues[roomsVisited][0] = roomsVisited + 1;
        roomValues[roomsVisited][1] = 1;
        roomValues[roomsVisited][2] = 1;
        roomsVisited += 1;          
        String message1 = "Checked room number: ";
        String message2 = message1 + roomsVisited;
        String messageFull = message2 + " to the right of the Zumo. This room was populated. Please rotate the zumo and use the GO! button to continue.";
        Serial1.println(messageFull);
        }
      zumoRoom = false;
      input = "";
      break;
    }
    }
  
  if (zumoAuto == false && zumoRoom == false){
    
    switch (input){
      case'G':
      zumoAuto = true;  //Sets the Zumo's Automatic navigation flag to true
      motors.setSpeeds(200, 200);  //Sets the Zumo into motion
      break;

      case'E':
      zumoRoom = true;  //Sets the Zumo's room navigation flag to true
      Serial1.println("Robot is now in room checking mode, please indicate where the room is using the L and R buttons!");
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
  if (zumoAuto == true && zumoRoom == false){
    int16_t position = lineSensors.readLine(lineSensorValues,QTR_EMITTERS_ON,1);  //The zumo defaults to running along a black line avoiding white boundaries, this has been inversed for the map we recieved (White line, black boundaries)
    int16_t error = position - 2000;
    int16_t speedDifference = error / 4 + 6 * (error - lastError);
    lastError = error;
    int16_t leftSpeed = (int16_t)maxSpeed + speedDifference;
    int16_t rightSpeed = (int16_t)maxSpeed - speedDifference;
    leftSpeed = constrain(leftSpeed, 0, (int16_t)maxSpeed); //These lines of code will stop a track on one side of the zumo if a wall is detected to make a small turn away from it, keeping it within the corridor
    rightSpeed = constrain(rightSpeed, 0, (int16_t)maxSpeed);
    motors.setSpeeds(leftSpeed, rightSpeed);
    
    if (input == 'S'){
      motors.setSpeeds(0, 0);  //The zumo stops
      zumoAuto = false; // Automatic navigation exits and the zumo awaits its next response from the GUI
      }

    if (lineSensorValues[2] > 150){ //This if detects whether the front central sensor on the zumo detects a line, in which case we have hit the end of a corridor
      motors.setSpeeds(0, 0);  //The zumo stops
      zumoAuto = false; // Automatic navigation exits and the zumo awaits its next response from the GUI
      Serial1.println("Robot reached the end of corridor, choose which way you would like to turn using the L and R buttons!"); //This sends a message to the text box on the GUI, instructing the user on how to continue
    }
  }
}
