// ZUMO ROBOT PERIPHERAL CONNECTION
// Bluetooth unit: BLE-CC41-A (HM-10 clone)
// I/O: Ultrasound proximity sensor

#include <SoftwareSerial.h>
#include <ZumoMotors.h>
#include <NewPing.h>

SoftwareSerial bluetooth(A4, A5); //TX, RX
ZumoMotors motors;
NewPing sonar(2, 3, 400); //Trig, Echo, Max distance cm

char commBuf[80] = {0};

void setup(){
  bluetooth.begin(9600);

  //Quick configuration
  runCommand("AT+RENEW");
  runCommand("AT+NAMESemprini");
  runCommand("AT+PASS573000");
}
void loop(){
  //Listen for messages from master
  while(!bluetooth.available());

  //Copy message to char* commBuf
  getResponse();

  //Check message format, disconnect if invalid
  char header[7] = {0};
  memcpy(header, commBuf, 6);
  bool invalidFormat = false;

  if(strcmp(header, "SMove:"))
    invalidFormat = true;
  else switch(commBuf[6]){
    case 'N': motors.setSpeeds(   0,    0); break; //stand still
    case 'U': motors.setSpeeds( 200,  200); break; //forward
    case 'L': motors.setSpeeds(-200,  200); break; //turn left
    case 'D': motors.setSpeeds(-200, -200); break; //reverse
    case 'R': motors.setSpeeds( 200, -200); break; //turn right
    default:  invalidFormat = true;
  }

  if(invalidFormat){
    motors.setSpeeds(0, 0);
    runCommand("AT");
  }
  else{ //Provide response
    sprintf(commBuf, "%f\r\n", sonar.convert_cm(sonar.ping()));
    bluetooth.write(commBuf);
  }
}

/* ---------------------- */
void runCommand(char* cmd){
  //First transmit
  bluetooth.write(cmd);

  //Wait for response, resend on timeout
  unsigned long timestamp = millis();
  while(!bluetooth.available())
    if(millis() - timestamp >= 1000){
      bluetooth.write(cmd);
      timestamp = millis();
    }

  //Message is underway
  getResponse();
}
void getResponse(){
  //Saves response in external char* commBuf
  for(int index = 0; index < 79; index++){
    //Abort if no null-terminator in sight
    for(int abort = 0; !bluetooth.available(); abort++)
      if(abort >= 250){
        commBuf[index] = 0;
        return;
      }

    //Store characters until NUL is read
    commBuf[index] = bluetooth.read();
    if(!commBuf[index])
      break;
  }
  //Just to be sure...
  commBuf[79] = 0;
}
