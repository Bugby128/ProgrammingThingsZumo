#include <Zumo32U4.h>

Zumo32U4Motors motors;

void setup() {
  Serial1.begin(9600);    //Open serial connection for the XBee
}

void loop() {
  int input = Serial1.read();   //Read in the value that has been sent over the XBee and store it as a variable
  
  switch (input){   //Switch statement that takes the value sent over the XBee and changes the wheel motors speed to move/turn it appropriately 
      case'U':
      motors.setSpeeds(175,175);
      break;
      
      case'L':
      motors.setSpeeds(-150, 150);
      break;
    
      case'R':
      motors.setSpeeds(150, -50);
      break;
      
      case 'D':
      motors.setSpeeds(-175,-175);
      break;

      case'S':
      motors.setSpeeds(0, 0);
      break;
    }

}



  
