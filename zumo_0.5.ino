#include <ZumoMotors.h>
#include <Pushbutton.h>
#include <QTRSensors.h>
#include <ZumoReflectanceSensorArray.h>
#include <NewPing.h>

#define QTR_THRESHOLD  300

// these might need to be tuned for different motor types
#define reverse_speed     200 // 0 is stopped, 400 is full speed
#define turn_speed        175
#define full_speed        400
#define reverse_duration  200 // ms

/*-----SONAR-----*/
const int triggerPin = 3;
const int echoPinFront = 6;
const int echoPinBack = A1;

const int maxDistance = 50;

NewPing sonarFront(triggerPin, echoPinFront, maxDistance);
NewPing sonarBack(triggerPin, echoPinBack, maxDistance);

// setting up motors
ZumoMotors motors;
Pushbutton button(ZUMO_BUTTON); // pushbutton on pin 12

ZumoReflectanceSensorArray robotSensors;
unsigned int sensor_values[6];

void setup(){
  robotSensors.init();
  button.waitForButton();

  startMove();
}
void loop(){
  robotSensors.read(sensor_values);

  // reverse based on edge detection
  if (sensor_values[0] < QTR_THRESHOLD && sensor_values[5] < QTR_THRESHOLD){
    driveBack(reverse_speed);
    delay(reverse_duration);
  }else if (sensor_values[0] < QTR_THRESHOLD){
    motors.setSpeeds(-full_speed, 0);
    delay(reverse_duration);
  }else if (sensor_values[5] < QTR_THRESHOLD){
    motors.setSpeeds(0, -full_speed);
    delay(reverse_duration);
  }
  searchAndCharge();
}

void startMove(){
  turnLeft(full_speed);
  delay(175);

  driveBack(full_speed);
  delay(300);
}
boolean detectedEnemyFront(){
  unsigned int timeFront = sonarFront.ping();
  float sonarDistanceFront = sonarFront.convert_cm(timeFront);
  if (sonarDistanceFront != 0) {
    return true;
  }else{
    return false;
  }
}
boolean detectedEnemyBack(){
  unsigned int timeBack = sonarBack.ping();
  float sonarDistanceBack = sonarBack.convert_cm(timeBack);
  if (sonarDistanceBack != 0) {
    return true;
  }else{
    return false;
  }
}
void searchAndCharge(){
  if (detectedEnemyFront()){
    driveForward(full_speed);
  }else if(detectedEnemyBack()){
    fastBackTurn(400);
  }else{
    spinWhileSearching();
  }
}
boolean seekDirection = true;
int seekSteps = 0;

void spinWhileSearching(){
  if (seekDirection){
    turnRight(turn_speed);
  }else{
    turnLeft(turn_speed);
  }
  if (seekSteps == 0){
    seekDirection = !seekDirection;
    seekSteps = random(70, 150);
  }
  seekSteps--;
}

void fastBackTurn(int speed){
  motors.setSpeeds(speed, -speed);
  delay(350);
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
