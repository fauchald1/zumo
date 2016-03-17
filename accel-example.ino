/*-------------------------------------------\
| Uses the accelerometer to detect collision  \
| and turn to face the direction it came from. \
| Works poorly, serves as a quick code example. \
|----------------------------------------------*/

#include <ZumoMotors.h>
#include <LSM303.h>
#include <Wire.h>

ZumoMotors motor;
LSM303 accel;

void enableLSM303D(LSM303 device){
  /* This is a small part of code for accel.enableDefault(),
   * which enables both the accelerometer and magnetometer.
   * Perhaps it gives different results. */
  device.writeReg(LSM303::CTRL2, 0x00);
  device.writeReg(LSM303::CTRL1, 0x57);
  device.writeReg(LSM303::CTRL5, 0x64);
  device.writeReg(LSM303::CTRL6, 0x20);
  device.writeReg(LSM303::CTRL7, 0x00);
}
void fastSpinRadians(double rad){
  if(rad >= 0)
    motor.setSpeeds(400, -400); //turn right
  else{
    rad = -rad;
    motor.setSpeeds(-400, 400); //turn left
  }
  delay(rad * 125); //adjusted magic number

  motor.setSpeeds(0, 0);
  delay(300); //overly safe delay to prevent reading stop reaction
}

/*---------------*/
void setup(){
  Serial.begin(9600);
  Wire.begin(); //needed to Serial.print output from accel

  accel.init();
  enableLSM303D(accel);
}
void loop(){
  accel.readAcc(); //readings are stored in vector a
  long x = -accel.a.x;
  long y =  accel.a.y;
  //z is ignored

  //detect collision using magnitude of 2D acceleration
  if(sqrt(x*x + y*y) > 2500) //arbitrary threshold
    fastSpinRadians(atan2(y,x));
}
