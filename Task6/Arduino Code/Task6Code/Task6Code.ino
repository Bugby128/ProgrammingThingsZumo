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
const uint16_t maxSpeed = 200;    //Max speed the robot can reach while automatically navigating

bool zumoAuto = false;  //A boolean variable to be used as a flag for whether the zumo is currentley automatically navigating
bool zumoRoom = false;  //A boolean variable to be used as a flag for whether the zumo is currentley in room checking mode

int roomValues[4][3]; //This will be used to store room number [0], room direction (0 = L / 1 = R)[1] and room empty? (0 = empty / 1 Occupied)[2]
int roomsVisited = 0; //This will be used as a counter for the number of rooms we have entered

int leftEncoders; //Creates variables to store the wheel encoders
int rightEncoders;
int reverseEncoders;  //A variable to hold the current wheel encoder distance for backtracking past the T-Junction

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
  for (uint16_t i = 0; i < 120; i++)
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

void avoidWalls() //Starting from task 6 i use this code in two different areas so i created a function for it to prevent repeating the code
{
  int16_t position = lineSensors.readLine(lineSensorValues, QTR_EMITTERS_ON, 1); //The zumo defaults to running along a black line avoiding white boundaries, this has been inversed for the map we recieved (White line, black boundaries)
  int16_t error = position - 2000;
  int16_t speedDifference = error / 4 + 6 * (error - lastError);
  lastError = error;
  int16_t leftSpeed = (int16_t)maxSpeed + speedDifference;
  int16_t rightSpeed = (int16_t)maxSpeed - speedDifference;
  leftSpeed = constrain(leftSpeed, 0, (int16_t)maxSpeed); //These lines of code will stop a track on one side of the zumo if a wall is detected to make a small turn away from it, keeping it within the corridor
  rightSpeed = constrain(rightSpeed, 0, (int16_t)maxSpeed);
  motors.setSpeeds(leftSpeed, rightSpeed);
}


void loop() {


  int input = Serial1.read();   //Read in the value that has been sent over the XBee and store it as a variable
  turnSensorReset();            //Resets the turn sensors at the start of each loop, makes sure it still turns the same amount even after turning multiple times on one route

  //When inside this IF statement we are in room navigation mode
  if (zumoRoom == true && zumoAuto == false) {
    switch (input) {
      case'L':
        motors.setSpeeds(-150, 150);
        while ((int32_t)turnAngle < turnAngle90) //I use the zumos gyro scope to make sure every turn is exactley 90 degrees clockwise/anti-clockwise
        {
          turnSensorUpdate();
        }
        leftEncoders = encoders.getCountsAndResetLeft();  //Here i reset the encoders so that they can be used to measure how far the zumo enters into the room
        rightEncoders = encoders.getCountsAndResetRight();
        motors.setSpeeds(100, 100);
        while (lineSensorValues[2] < 150 && proxSensors.countsFrontWithLeftLeds() < 6 && proxSensors.countsFrontWithRightLeds() < 6) { //This while loop continuously checks both the line and proximity sensors and stops the zumo once it hits a wall or detects an object inside the room
          lineSensors.readLine(lineSensorValues, QTR_EMITTERS_ON, 1);
          proxSensors.read();
        }
        motors.setSpeeds(0, 0);

        if (lineSensorValues[2] > 150 ) { //We go into this if statement if the zumo was stopped by its line sensors
          motors.setSpeeds(-100, -100);
          do {  //This do while loop reverses the zumo until it's encoders return to zero (it returns to its starting position)
            leftEncoders = encoders.getCountsLeft();
            rightEncoders = encoders.getCountsRight();
          }  while (leftEncoders > 0 && rightEncoders > 0);
          motors.setSpeeds(0, 0);
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
        else { //This else statement means that the zumo was stopped because the proximity sensors detected an item
          motors.setSpeeds(-100, -100);
          do {
            leftEncoders = encoders.getCountsLeft();
            rightEncoders = encoders.getCountsRight();
          }  while (leftEncoders > 0 && rightEncoders > 0);
          motors.setSpeeds(0, 0);
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
        while ((int32_t)turnAngle > -turnAngle90) //I use the zumos gyro scope to make sure every turn is exactley 90 degrees clockwise/anti-clockwise
        {
          turnSensorUpdate();
        }
        leftEncoders = encoders.getCountsAndResetLeft();
        rightEncoders = encoders.getCountsAndResetRight();
        motors.setSpeeds(100, 100);
        proxSensors.read();
        lineSensors.readLine(lineSensorValues, QTR_EMITTERS_ON, 1);
        while (lineSensorValues[2] < 150 && proxSensors.countsFrontWithLeftLeds() < 6 && proxSensors.countsFrontWithRightLeds() < 6) {
          lineSensors.readLine(lineSensorValues, QTR_EMITTERS_ON, 1);
          proxSensors.read();
        }
        motors.setSpeeds(0, 0);
        if (lineSensorValues[2] > 150 ) {
          motors.setSpeeds(-100, -100);
          do {
            leftEncoders = encoders.getCountsLeft();
            rightEncoders = encoders.getCountsRight();
          }  while (leftEncoders > 0 && rightEncoders > 0);
          motors.setSpeeds(0, 0);
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
        else {
          motors.setSpeeds(-100, -100);
          do {
            leftEncoders = encoders.getCountsLeft();
            rightEncoders = encoders.getCountsRight();
          }  while (leftEncoders > 0 && rightEncoders > 0);
          motors.setSpeeds(0, 0);
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

  //When inside this IF statement we are in manual control mode and the zumo is awaiting input from the GUI
  if (zumoAuto == false && zumoRoom == false) {

    switch (input) {
      case'G':
        zumoAuto = true;  //Sets the Zumo's Automatic navigation flag to true
        motors.setSpeeds(200, 200);  //Sets the Zumo into motion
        break;

      case'B':  //This case is for the backtracking segment
        Serial1.println("Zumo is currentley backtracking to the T-Junction, controls are locked for the duration!");  //Informs the user that they will be unable to control the zumo until it reaches
        Serial1.end();  //This closes the serial connection between the XBee devices to lock the user out of the controls while the zumo backtracks
        motors.setSpeeds(-150, 150);
        while ((int32_t)turnAngle < turnAngle90) //I repeat the 90 degree turn code twice in order to rotate the zumo 180 degrees
        {
          turnSensorUpdate();
        }
        turnSensorReset();
        while ((int32_t)turnAngle < turnAngle90)
        {
          turnSensorUpdate();
        }
        leftEncoders = encoders.getCountsAndResetLeft();  //Resets the encoders after a turn to make sure they are not recorded in the encoder count
        rightEncoders = encoders.getCountsAndResetRight();
        do {
          avoidWalls(); //While backtracking i will need to avoid the walls again
          leftEncoders = encoders.getCountsLeft();
          rightEncoders = encoders.getCountsRight();
        }  while (leftEncoders < reverseEncoders && rightEncoders < reverseEncoders); //Returns the Zumo to the T-Junction using the stored wheel encoder distance from the previous turn
        Serial1.begin(9600);  //Reopens the XBee connection
        delay(1000);  //This delay makes sure the serial connection is open again before continueing with the program (makes sure no GUI inputs or outputs are lost)
        Serial1.println("Zumo passed T-Junction, control returned!"); //Informs the user that they may control the robot again
        zumoAuto = true;  //Sets the Zumo's Automatic navigation flag to true
        break;

      case'E':
        zumoRoom = true;  //Sets the Zumo's room navigation flag to true
        Serial1.println("Robot is now in room checking mode, please indicate where the room is using the L and R buttons!");
        break;

      case'L':
        motors.setSpeeds(-150, 150);
        while ((int32_t)turnAngle < turnAngle90) //I use the zumos gyro scope to make sure every turn is exactley 90 degrees clockwise/anti-clockwise
        {
          turnSensorUpdate();
        }
        leftEncoders = encoders.getCountsAndResetLeft();  //Resets the encoders after a turn to make sure they are not recorded in the encoder count
        rightEncoders = encoders.getCountsAndResetRight();
        zumoAuto = true;  //Sets the Zumo's Automatic navigation flag to true
        motors.setSpeeds(200, 200);  //Sets the Zumo into motion
        break;

      case'R':
        motors.setSpeeds(150, -150);
        while ((int32_t)turnAngle > -turnAngle90) //I use the zumos gyro scope to make sure every turn is exactley 90 degrees clockwise/anti-clockwise
        {
          turnSensorUpdate();
        }
        leftEncoders = encoders.getCountsAndResetLeft();  //Resets the encoders after a turn to make sure they are not recorded in the encoder count
        rightEncoders = encoders.getCountsAndResetRight();
        zumoAuto = true;  //Sets the Zumo's Automatic navigation flag to true
        motors.setSpeeds(200, 200);  //Sets the Zumo into motion
        break;
    }
  }

  //When inside this IF statement we are currentley in auto navigating mode, certain checks need to run on loop, Code below is used from "LineFollower" Zumo32U4 Example
  if (zumoAuto == true && zumoRoom == false) {
    avoidWalls();

    if (input == 'S') {
      motors.setSpeeds(0, 0);  //The zumo stops
      zumoAuto = false; // Automatic navigation exits and the zumo awaits its next response from the GUI
    }

    if (lineSensorValues[2] > 150) { //This if detects whether the front central sensor on the zumo detects a line, in which case we have hit the end of a corridor
      motors.setSpeeds(0, 0);  //The zumo stops
      zumoAuto = false; // Automatic navigation exits and the zumo awaits its next response from the GUI
      Serial1.println("Robot reached the end of corridor, choose which way you would like to turn using the L and R buttons!"); //This sends a message to the text box on the GUI, instructing the user on how to continue
      reverseEncoders = encoders.getCountsRight();  //This records the encoders distance from the last turn the zumo made to its current position (used when backtacking to the T-Junction)
    }
  }
}
