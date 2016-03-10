/*
Drive forward and turn left or right when border is detected
  - Only reflectionsensor 0 and 5 are used.
*/
#include <stdlib.h>
#include <stdbool.h>  
#include <ZumoMotors.h>
#include <Pushbutton.h>
#include <QTRSensors.h>
#include <ZumoReflectanceSensorArray.h>
#include <NewPing.h>
 
 
// this might need to be tuned for different lighting conditions, surfaces, etc.
#define QTR_THRESHOLD  300 // 
  
// these might need to be tuned for different motor types
#define reverse_speed     200 // 0 is stopped, 400 is full speed
#define turn_speed        200
#define full_speed        200
#define reverse_duration  200 // ms
#define turn_duration     300 // ms
 

// setting up Sonarsensor
const int echoPin = 3;
const int triggerPin = 6;
const int maxDistance = 100;    // max necessary distance

NewPing robotSonar(triggerPin, echoPin, maxDistance);


// setting up motors
ZumoMotors motors;
Pushbutton button(ZUMO_BUTTON); // pushbutton on pin 12

 
#define NUM_SENSORS 6
unsigned int sensor_values[NUM_SENSORS];
 
ZumoReflectanceSensorArray robotSensors;   // ''


 
void setup()
{
   robotSensors.init();
   button.waitForButton();

   startMove();
   

  Serial.begin(9600);
}

void startMove(){
  spinInRandomDirection();
  delay(300);                   //not sure about delay time here
  driveBack(full_speed);
  delay(reverse_duration);
}

void loop()
{
  robotSensors.read(sensor_values);

  double leftSensor = sensor_values[0];
  double rightSensor = sensor_values[5];

  if (sensor_values[0] < QTR_THRESHOLD || sensor_values[5] < QTR_THRESHOLD)
  {
    // if leftmost sensor detects line, reverse and turn to the right
    driveBack(reverse_speed);
    delay(reverse_duration);
    searchAndCharge();
    //motors.setSpeeds(full_speed, full_speed);
  }else{
    // otherwise, go search for tager
    searchAndCharge();
  }
}



void searchAndCharge(){
  if (detectedEnemy(5))
  {
    driveForward(full_speed);
  }else{
    spinInRandomDirection();
  }
}

void driveBack(int speed){
  motors.setSpeeds(-speed, -speed);
}

void driveForward(int speed){
  motors.setSpeeds(speed, speed);
}

void turnLeft(int speed){
  motors.setSpeeds(-speed,speed);
}

void turnRight(int speed){
  motors.setSpeeds(speed,-speed);
}

bool detectedEnemy(float maxSearchDistance){
  unsigned int time = robotSonar.ping();
  float sonarDistance = robotSonar.convert_cm(time);
  Serial.print(sonarDistance);
  delay(1000);
  if (sonarDistance <= maxSearchDistance && sonarDistance > 0.1) {
    
    return true;
  }else{
    return false;
  }

}


void spinInRandomDirection(){
  int right = 1;
  int left = 2;
  if (randomSide() == right){                   //randomSide is right, turning to righ 
    turnRight(turn_speed);
  }else{                                      //then randomSide is left
    turnLeft(turn_speed);
  }
}

int randomSide (){
  return (rand()%(1))+1;
}
  
