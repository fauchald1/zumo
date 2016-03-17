#include <stdlib.h>
#include <stdbool.h>
#include <ZumoMotors.h>
#include <Pushbutton.h>
#include <QTRSensors.h>
#include <ZumoReflectanceSensorArray.h>
#include <NewPing.h>

#define QTR_THRESHOLD  300 //

// these might need to be tuned for different motor types
#define reverse_speed     200 // 0 is stopped, 400 is full speed
#define turn_speed        250
#define full_speed        400
#define reverse_duration  200 // ms
#define turn_duration     300 // ms


/*-----SONAR-----*/
const int triggerPin = 3;
const int echoPinFront = 6;
const int echoPinBack = A1;


const int maxDistance = 70;

NewPing sonarFront(triggerPin, echoPinFront, maxDistance);
NewPing sonarBack(triggerPin,echoPinBack,maxDistance);

// setting up motors
ZumoMotors motors;
Pushbutton button(ZUMO_BUTTON); // pushbutton on pin 12


#define NUM_SENSORS 6
unsigned int sensor_values[NUM_SENSORS];

ZumoReflectanceSensorArray robotSensors;   // ''



void setup() {
  robotSensors.init();
  button.waitForButton();

  startMove();

  Serial.begin(9600);
}

void startMove(){
  motors.setSpeeds(-400,400);
  delay(175);                   //not sure about delay time here
  driveBack(full_speed);
  delay(300);
}

void loop() {
  robotSensors.read(sensor_values);

  double leftSensor = sensor_values[0];
  double rightSensor = sensor_values[5];

  if (sensor_values[0] < QTR_THRESHOLD || sensor_values[5] < QTR_THRESHOLD)
  {
    // if leftmost sensor detects line, reverse and turn to the right
    driveBack(reverse_speed);
    delay(reverse_duration);
    searchAndCharge();
  }else{
    // otherwise, go search for tager
    searchAndCharge();
  }
}

bool detectedEnemyFront(int maxDetectDistance){
  unsigned int timeFront = sonarFront.ping();
  float sonarDistanceFront = sonarFront.convert_cm(timeFront);
  Serial.print("FRONT: ");
  Serial.println(sonarDistanceFront);
  if (sonarDistanceFront != 0 && sonarDistanceFront < maxDetectDistance) {
    return true;
  }else{
    return false;
  }
}

bool detectedEnemyBack(int maxDetectDistance){
  unsigned int timeBack = sonarBack.ping();
  float sonarDistanceBack = sonarBack.convert_cm(timeBack);
  Serial.print("BACK: ");
  Serial.println(sonarDistanceBack);
  if (sonarDistanceBack != 0 && sonarDistanceBack < maxDetectDistance) {
    return true;
  }else{
    return false;
  }
}

void searchAndCharge(){
  if (detectedEnemyFront(40)){
    driveForward(full_speed);
  }else if(detectedEnemyBack(40)){
    fastBackTurn(400);
  }else{
    spinWhileSearching();
    //spinInRandomDirection(randomSide());
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

void fastBackTurn(int speed){
  motors.setSpeeds(speed, -speed);
  delay(327);
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
