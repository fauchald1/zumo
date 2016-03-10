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
#define turn_speed        250
#define full_speed        400
#define reverse_duration  200 // ms
#define turn_duration     300 // ms
 

// setting up Sonarsensor
const int echoPin = 3;
const int triggerPin = 6;
const int maxDistance = 70;    // max necessary distance



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
  spinInRandomDirection(randomSide());                   //not sure about delay time here
  driveBack(full_speed);
  delay(reverse_duration/2);
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
  if (detectedEnemy(40))
  {
    driveForward(full_speed);
  }else{
    spinWhileSearching();
    //spinInRandomDirection(randomSide());
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

bool detectedEnemy(int maxDetectDistance){
  unsigned int time = robotSonar.ping();
  float sonarDistance = robotSonar.convert_cm(time);
  if (sonarDistance != 0 && sonarDistance < maxDetectDistance) {
    return true;
  }else{
    return false;
  }

}

bool seekDirection = true;
int seekSteps = 0;

void spinWhileSearching(){
  if (seekDirection){ 
    turnRight(turn_speed);
  }else{
    turnLeft(turn_speed);
  }

  
  if(seekSteps == 0){
    seekDirection = !seekDirection;
    seekSteps = random(70, 150);
  }

  seekSteps--;
  /* search for enemy */

}


void spinInRandomDirection(int randSide){
  int right = 1;
  if (randSide == right){                   //randomSide is right, turning to righ 
    turnRight(turn_speed);
  }else{                                      //then randomSide is left
    turnLeft(turn_speed);
  }
}

int randomSide(){
  return random(1, 3);
}
  