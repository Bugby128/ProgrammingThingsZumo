#include <Wire.h>
#include <Zumo32U4.h>
#include "TurnSensor.h"

Zumo32U4Encoders encoders;
Zumo32U4LCD lcd;
Zumo32U4Motors motors;
Zumo32U4IMU imu;
Zumo32U4LineSensors lineSensors;
Zumo32U4ProximitySensors proxSensors;
Zumo32U4Buzzer buzzer;

unsigned int lineSensorValues[5]; //Creates an empty array to hold the multiple line sensor values
int16_t lastError = 0;
const uint16_t maxSpeed = 200;    //Max speed the robot can reach while automatically navigating

bool zumoAuto = false;  //A boolean variable to be used as a flag for whether the zumo is currentley automatically navigating
bool zumoRoom = false;  //A boolean variable to be used as a flag for whether the zumo is currentley in room checking mode
bool followMeBuzzer = false; //A boolean variable to be used as a flag for wether the zumo should be continnually be playing a follow me note

int pathHome[10][3];    //This will be used to record the zumo's path home ([0] is encoder distance recorded for that step, [1] is the room number (if this step was entering a room, this should only be added if the room was populated otherwise default to zero), [2] will be turn direction (0 = No turn / 1 = L / 2 = R / 3 = B))
int currentStep = 0;  //This will be used as a counter for how many steps there are to return home
int currentTurn = 0;  //This will be used to keep track of the turn taken 
int roomValues[4][3]; //This will be used to store room number [0], room direction (1 = L / 2 = R)[1] and room empty? (0 = empty / 1 Occupied)[2]
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
  leftEncoders = encoders.getCountsAndResetLeft();  //Here i reset the encoders so that they can be used to measure how far the zumo enters into the room
  rightEncoders = encoders.getCountsAndResetRight();
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


void avoidWalls() //Starting from task 6 i use this code in two different areas so i created a function for it to prevent repeating the code
{
  int16_t position = lineSensors.readLine(lineSensorValues,QTR_EMITTERS_ON,1);  //The zumo defaults to running along a black line avoiding white boundaries, this has been inversed for the map we recieved (White line, black boundaries)
    int16_t error = position - 2000;
    int16_t speedDifference = error / 4 + 6 * (error - lastError);
    lastError = error;
    int16_t leftSpeed = (int16_t)maxSpeed + speedDifference;
    int16_t rightSpeed = (int16_t)maxSpeed - speedDifference;
    leftSpeed = constrain(leftSpeed, 0, (int16_t)maxSpeed); //These lines of code will stop a track on one side of the zumo if a wall is detected to make a small turn away from it, keeping it within the corridor
    rightSpeed = constrain(rightSpeed, 0, (int16_t)maxSpeed);
    motors.setSpeeds(leftSpeed, rightSpeed); 
  }


void goHome() //This function uses the "homePath" array to backtrack its steps through the maze and recheck rooms that were occupied.
{
  Serial1.println("Zumo is returning home!");
  motors.setSpeeds(-150, 150);
  while((int32_t)turnAngle < turnAngle90)//I repeat the 90 degree turn code twice in order to rotate the zumo 180 degrees
  {
    turnSensorUpdate();
  }
  turnSensorReset();
  while((int32_t)turnAngle < turnAngle90)
  {
    turnSensorUpdate();
  }
  leftEncoders = encoders.getCountsAndResetLeft();  
  rightEncoders = encoders.getCountsAndResetRight();
  motors.setSpeeds(200, 200);
  
  for (int i = currentStep; i > 0; i--) { //This for loop is used to go through each item in "homePath" array, the i is set to currentStep (how many steps the robot has taken through the maze) and decrements with each loop (this checks homePath backwards)
    int distance = pathHome[i - 1][0];  //Motor encoders for current step (how far to move)
    int room = pathHome[i - 1][1];  //Room number of occupied rooms 
    int turn = pathHome[i - 1][2];  //The direction of any turns made on this step (Reversed allow the robot to turn corners when going the opposite direction)
      
    if (followMeBuzzer == true){  //This turns on the LED and makes a tone every iteration of the loop as a signal to follow
      ledGreen(1);
      buzzer.playFrequency(440, 200, 15);
      }
  
    do {  //This do while loop drives the zumo until it has reached the encoders for this step
    avoidWalls();
    leftEncoders = encoders.getCountsLeft();
    rightEncoders = encoders.getCountsRight();
    }  while(leftEncoders < distance && rightEncoders < distance && lineSensorValues[2] < 150);
    motors.setSpeeds(0, 0);
    leftEncoders = encoders.getCountsAndResetLeft();
    rightEncoders = encoders.getCountsAndResetRight();
    delay(500);
           

    if(room > 0){ //If a room number is detected this segment of code runs
      int roomDirection = roomValues[roomsVisited - 1][1];  //Takes which side of the zumo the room was located on
      turnSensorReset();
      switch(roomDirection){
        case 1:
        motors.setSpeeds(150, -150);  //This turn is the opposite of the direction given to make sure it works going the opposite direction
        while((int32_t)turnAngle > -turnAngle90)  //I use the zumos gyro scope to make sure every turn is exactley 90 degrees clockwise/anti-clockwise
        {
          turnSensorUpdate();
        }
        leftEncoders = encoders.getCountsAndResetLeft();
        rightEncoders = encoders.getCountsAndResetRight();
        turnSensorReset();
        motors.setSpeeds(100, 100);
        proxSensors.read();
        lineSensors.readLine(lineSensorValues,QTR_EMITTERS_ON,1); //Scans the room a second time
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
          leftEncoders = encoders.getCountsAndResetLeft();
        rightEncoders = encoders.getCountsAndResetRight();
          delay(500);  
        }
        else{ //Item was found for the second time
          motors.setSpeeds(-100, -100);
          do {
          leftEncoders = encoders.getCountsLeft();
          rightEncoders = encoders.getCountsRight();
          }  while(leftEncoders > 0 && rightEncoders > 0);
          motors.setSpeeds(0,0);
          leftEncoders = encoders.getCountsAndResetLeft();
          rightEncoders = encoders.getCountsAndResetRight();
          delay(500);
          followMeBuzzer = true;  //The follow me flag is set to true so the robot will turn on its led and beep the rest of the way to the beginning
          Serial1.println("Object Detected"); //User is informed an object was found again
          }
        motors.setSpeeds(-150, 150); 
        while((int32_t)turnAngle < turnAngle90)//I use the zumos gyro scope to make sure every turn is exactley 90 degrees clockwise/anti-clockwise
        {
        turnSensorUpdate();
        }
        motors.setSpeeds(0, 0);
        leftEncoders = encoders.getCountsAndResetLeft();   
        rightEncoders = encoders.getCountsAndResetRight();
        roomsVisited -= 1;
        break;
        
        case 2:
        motors.setSpeeds(-150, 150);
        while((int32_t)turnAngle < turnAngle90) //I use the zumos gyro scope to make sure every turn is exactley 90 degrees clockwise/anti-clockwise
        {
          turnSensorUpdate();
        }
        leftEncoders = encoders.getCountsAndResetLeft();  //Here i reset the encoders so that they can be used to measure how far the zumo enters into the room
        rightEncoders = encoders.getCountsAndResetRight();
        turnSensorReset();
        motors.setSpeeds(100, 100);
        while (lineSensorValues[2] < 150 && proxSensors.countsFrontWithLeftLeds() < 6 && proxSensors.countsFrontWithRightLeds() < 6){ //This while loop continuously checks both the line and proximity sensors and stops the zumo once it hits a wall or detects an object inside the room
          lineSensors.readLine(lineSensorValues,QTR_EMITTERS_ON,1);
          proxSensors.read();
          }
        motors.setSpeeds(0, 0);
        leftEncoders = encoders.getCountsAndResetLeft();
        rightEncoders = encoders.getCountsAndResetRight();
                
        if (lineSensorValues[2] > 150 ){  //We go into this if statement if the zumo was stopped by its line sensors
          motors.setSpeeds(-100, -100);   
          do {  //This do while loop reverses the zumo until it's encoders return to zero (it returns to its starting position)
          leftEncoders = encoders.getCountsLeft();
          rightEncoders = encoders.getCountsRight();
          }  while(leftEncoders > 0 && rightEncoders > 0);
          motors.setSpeeds(0,0);
          leftEncoders = encoders.getCountsAndResetLeft();
          rightEncoders = encoders.getCountsAndResetRight();
          delay(500);
        }
        else{ //This else statement means that the zumo was stopped because the proximity sensors detected an item
          motors.setSpeeds(-100, -100);
          do {
          leftEncoders = encoders.getCountsLeft();
          rightEncoders = encoders.getCountsRight();
          }  while(leftEncoders > 0 && rightEncoders > 0);
          motors.setSpeeds(0,0);
          leftEncoders = encoders.getCountsAndResetLeft();
          rightEncoders = encoders.getCountsAndResetRight();
          delay(500);
          followMeBuzzer = true;
          Serial1.println("Object Detected");
          }
        motors.setSpeeds(150, -150);
        while((int32_t)turnAngle > -turnAngle90)//I use the zumos gyro scope to make sure every turn is exactley 90 degrees clockwise/anti-clockwise
        {
          turnSensorUpdate();
        }
        motors.setSpeeds(0, 0);
        leftEncoders = encoders.getCountsAndResetLeft();  //Resets the encoders after a turn to make sure they are not recorded in the encoder count  
        rightEncoders = encoders.getCountsAndResetRight();
        roomsVisited -= 1;
        break;
      }
    }
    
    turnSensorReset();
    switch(turn){ //Switch statement that takes the directional turn and reverses it for turning corners on the path home
      case 1:
      motors.setSpeeds(150, -150);
      while((int32_t)turnAngle > -turnAngle90)//I use the zumos gyro scope to make sure every turn is exactley 90 degrees clockwise/anti-clockwise
      {
        turnSensorUpdate();
      }
      motors.setSpeeds(0, 0);
      leftEncoders = encoders.getCountsAndResetLeft();  //Resets the encoders after a turn to make sure they are not recorded in the encoder count  
      rightEncoders = encoders.getCountsAndResetRight();
      break;
      
      case 2: 
      motors.setSpeeds(-150, 150); 
      while((int32_t)turnAngle < turnAngle90)//I use the zumos gyro scope to make sure every turn is exactley 90 degrees clockwise/anti-clockwise
      {
        turnSensorUpdate();
      }
      motors.setSpeeds(0, 0);
      leftEncoders = encoders.getCountsAndResetLeft();  //Resets the encoders after a turn to make sure they are not recorded in the encoder count  
      rightEncoders = encoders.getCountsAndResetRight();
      break;

      case 3: //3 is given when the backtrack button was pressed on a step
      motors.setSpeeds(-150, 150);
      while((int32_t)turnAngle < turnAngle90)//I repeat the 90 degree turn code twice in order to rotate the zumo 180 degrees
      {
        turnSensorUpdate();
      }
      turnSensorReset();
      while((int32_t)turnAngle < turnAngle90)
      {
        turnSensorUpdate();
      }
      motors.setSpeeds(0, 0);
      leftEncoders = encoders.getCountsAndResetLeft();  //Resets the encoders after a turn to make sure they are not recorded in the encoder count  
      rightEncoders = encoders.getCountsAndResetRight();
      break;
      
    }
  }
  ledGreen(0);  //Turns off the follow me LED when the Zumo reaches the end of the map, the buzzer was buzzing repeatedly inside the loop so it does not need to be manually turned off
}


void loop() {
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
      turnSensorReset();
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
        pathHome[currentStep + 1][1] = 0;
        pathHome[currentStep][2] = currentTurn;
        roomValues[roomsVisited][0] = roomsVisited + 1; //The following three lines of code are for storing the values of the room by adding them to an array (Room number, Directrion, room occupied)
        roomValues[roomsVisited][1] = 1;
        roomValues[roomsVisited][2] = 0;
        roomsVisited += 1;  //Increments the room counter to keep track of how many rooms we have visited
        currentStep += 1; //Increments current step by one to show that another step on the path home has been added
        currentTurn = 0;
        String message1 = "Checked room number: ";  //Due to the way concatonation works in arduino i have to add the strings together before printing the final message to prevent errors
        String message2 = message1 + roomsVisited;
        String messageFull = message2 + " to the left of the Zumo. This room was empty. Please use the GO! button to continue.";
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
        pathHome[currentStep + 1][1] = roomsVisited + 1;
        pathHome[currentStep][2] = currentTurn;
        roomValues[roomsVisited][0] = roomsVisited + 1;
        roomValues[roomsVisited][1] = 2;
        roomValues[roomsVisited][2] = 1;
        roomsVisited += 1;  
        currentStep += 1; //Increments current step by one to show that another step on the path home has been added 
        currentTurn = 0;       
        String message1 = "Checked room number: ";
        String message2 = message1 + roomsVisited;
        String messageFull = message2 + " to the left of the Zumo. This room was populated. Please use the GO! button to continue.";
        Serial1.println(messageFull);
        }
      motors.setSpeeds(150, -150);
      while((int32_t)turnAngle > -turnAngle90)  //I use the zumos gyro scope to make sure every turn is exactley 90 degrees clockwise/anti-clockwise
      {
        turnSensorUpdate();
      }
      leftEncoders = encoders.getCountsAndResetLeft();
      rightEncoders = encoders.getCountsAndResetRight();
      motors.setSpeeds(0, 0);
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
      turnSensorReset();
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
        pathHome[currentStep + 1][1] = 0;
        pathHome[currentStep][2] = currentTurn;
        roomValues[roomsVisited][0] = roomsVisited + 1;
        roomValues[roomsVisited][1] = 2;
        roomValues[roomsVisited][2] = 0;
        roomsVisited += 1;
        currentStep += 1; //Increments current step by one to show that another step on the path home has been added
        currentTurn = 0;
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
        pathHome[currentStep + 1][1] = roomsVisited + 1;
        pathHome[currentStep][2] = currentTurn;
        roomValues[roomsVisited][0] = roomsVisited + 1;
        roomValues[roomsVisited][1] = 1;
        roomValues[roomsVisited][2] = 1;
        roomsVisited += 1;  
        currentStep += 1; //Increments current step by one to show that another step on the path home has been added 
        currentTurn = 0;     
        String message1 = "Checked room number: ";
        String message2 = message1 + roomsVisited;
        String messageFull = message2 + " to the right of the Zumo. This room was populated. Please rotate the zumo and use the GO! button to continue.";
        Serial1.println(messageFull);
        }
        motors.setSpeeds(-150, 150);
      while((int32_t)turnAngle < turnAngle90) //I use the zumos gyro scope to make sure every turn is exactley 90 degrees clockwise/anti-clockwise
      {
        turnSensorUpdate();
      }
      leftEncoders = encoders.getCountsAndResetLeft();  //Here i reset the encoders so that they can be used to measure how far the zumo enters into the room
      rightEncoders = encoders.getCountsAndResetRight();
      motors.setSpeeds(0, 0);
      zumoRoom = false;
      input = "";
      break;
    }
    }

  //When inside this IF statement we are in manual control mode and the zumo is awaiting input from the GUI
  if (zumoAuto == false && zumoRoom == false){
    
    switch (input){
      case'H':
      goHome(); //Calls the function to begin the Zumos return trip home
      break;
     
      case'G':
      zumoAuto = true;  //Sets the Zumo's Automatic navigation flag to true
      motors.setSpeeds(200, 200);  //Sets the Zumo into motion
      break;

      case'B':  //This case is for the backtracking segment
      currentTurn = 3;  //Indicates the zumo will have to make a 180 turn on its way home
      Serial1.println("Zumo is currentley backtracking to the T-Junction, controls are locked for the duration!");  //Informs the user that they will be unable to control the zumo until it reaches 
      Serial1.end();  //This closes the serial connection between the XBee devices to lock the user out of the controls while the zumo backtracks
      motors.setSpeeds(-150, 150);
      while((int32_t)turnAngle < turnAngle90)//I repeat the 90 degree turn code twice in order to rotate the zumo 180 degrees
      {
        turnSensorUpdate();
      }
      turnSensorReset();
      while((int32_t)turnAngle < turnAngle90)
      {
        turnSensorUpdate();
      }
      leftEncoders = encoders.getCountsAndResetLeft();  //Resets the encoders after a turn to make sure they are not recorded in the encoder count  
      rightEncoders = encoders.getCountsAndResetRight();
      pathHome[currentStep][0] = reverseEncoders;
      pathHome[currentStep][1] = 0;
      pathHome[currentStep][2] = currentTurn; 
      currentStep += 1;
      currentTurn = 0;        
      do {
        avoidWalls(); //While backtracking i will need to avoid the walls again
        leftEncoders = encoders.getCountsLeft();
        rightEncoders = encoders.getCountsRight();
        }  while(leftEncoders < reverseEncoders && rightEncoders < reverseEncoders);  //Returns the Zumo to the T-Junction using the stored wheel encoder distance from the previous turn
        Serial1.begin(9600);  //Reopens the XBee connection
        delay(1000);  //This delay makes sure the serial connection is open again before continueing with the program (makes sure no GUI inputs or outputs are lost)
        Serial1.println("Zumo passed T-Junction, control returned!"); //Informs the user that they may control the robot again
        zumoAuto = true;  //Sets the Zumo's Automatic navigation flag to true  
      break;

      case'E':
      reverseEncoders = encoders.getCountsRight();
      pathHome[currentStep][0] = reverseEncoders;
      zumoRoom = true;  //Sets the Zumo's room navigation flag to true
      Serial1.println("Robot is now in room checking mode, please indicate where the room is using the L and R buttons!");
      break;
      
      case'L':
      currentTurn = 1;  //Sets the current turn for the pathHome to be left
      motors.setSpeeds(-150, 150);
      while((int32_t)turnAngle < turnAngle90)//I use the zumos gyro scope to make sure every turn is exactley 90 degrees clockwise/anti-clockwise
      {
        turnSensorUpdate();
      }
      leftEncoders = encoders.getCountsAndResetLeft();  //Resets the encoders after a turn to make sure they are not recorded in the encoder count  
      rightEncoders = encoders.getCountsAndResetRight();
      zumoAuto = true;  //Sets the Zumo's Automatic navigation flag to true
      motors.setSpeeds(200, 200);  //Sets the Zumo into motion
      break;
    
      case'R':
      currentTurn = 2;  //Sets the current turn for the pathHome to be left
      motors.setSpeeds(150, -150);
      while((int32_t)turnAngle > -turnAngle90)//I use the zumos gyro scope to make sure every turn is exactley 90 degrees clockwise/anti-clockwise
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
  if (zumoAuto == true && zumoRoom == false){
    avoidWalls();

    if (input == 'S'){
      motors.setSpeeds(0, 0);  //The zumo stops
      zumoAuto = false; // Automatic navigation exits and the zumo awaits its next response from the GUI
      }

    if (lineSensorValues[2] > 150){ //This if detects whether the front central sensor on the zumo detects a line, in which case we have hit the end of a corridor
      motors.setSpeeds(0, 0);  //The zumo stops
      zumoAuto = false; // Automatic navigation exits and the zumo awaits its next response from the GUI
      Serial1.println("Robot reached the end of corridor, choose which way you would like to turn using the L and R buttons!"); //This sends a message to the text box on the GUI, instructing the user on how to continue
      delay(1000); 
      reverseEncoders = encoders.getCountsRight();  //This records the encoders distance from the last turn the zumo made to its current position (used when backtacking to the T-Junction)   
      pathHome[currentStep][0] = reverseEncoders;
      pathHome[currentStep][1] = 0;
      pathHome[currentStep][2] = currentTurn;
      currentStep += 1; //Increments current step by one to show that another step on the path home has been added
      currentTurn = 0;  //Resets the currentTurn for the next step
    }
  }
}
