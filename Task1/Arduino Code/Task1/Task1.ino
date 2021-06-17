#include <Zumo32U4.h>

Zumo32U4Motors motors;

void setup() {
  Serial1.begin(9600); 
}

void loop() {
  int input = Serial1.read(); 
  switch (input){
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



  
