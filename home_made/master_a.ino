// REMOTE CONTROL CENTRAL CONNECTION
// Bluetooth unit: CZ-HC-05 (HC-05 clone)
// I/O: IR remote and 6-LED indicator

#include <SoftwareSerial.h>
#include <PLab_IRremote.h>

#define PIN_LED_400  2
#define PIN_LED_200  3
#define PIN_LED_100  4
#define PIN_LED_50   5
#define PIN_LED_25   6
#define PIN_LED_10   7
#define PIN_IR_RECV  8
#define PIN_BLT_PWR  A0
#define PIN_BLT_TXD  A1
#define PIN_BLT_RXD  A2
#define PIN_BLT_KEY  A3

SoftwareSerial bluetooth(PIN_BLT_TXD, PIN_BLT_RXD);
IRrecv irRecv(PIN_IR_RECV);
decode_results irResults;

char commBuf[80] = {0};
char* lastCommand = "SMove:N";

void setup(){
  pinMode(PIN_LED_400 , OUTPUT);
  pinMode(PIN_LED_200 , OUTPUT);
  pinMode(PIN_LED_100 , OUTPUT);
  pinMode(PIN_LED_50  , OUTPUT);
  pinMode(PIN_LED_25  , OUTPUT);
  pinMode(PIN_LED_10  , OUTPUT);
  pinMode(PIN_BLT_KEY , OUTPUT);
  pinMode(PIN_BLT_PWR , OUTPUT);

  //Initialize Bluetooth unit in command mode
  digitalWrite(PIN_BLT_KEY, HIGH);
  digitalWrite(PIN_BLT_PWR, HIGH);
  bluetooth.begin(38400);

  //Quick configuration
  runCommand("AT+ORGL\r\n", true);
  runCommand("AT+RMAAD\r\n", true);
  runCommand("AT+PSWD=573000\r\n", true);
  runCommand("AT+ROLE=1\r\n", true);
  runCommand("AT+CMODE=1\r\n", true);
  runCommand("AT+RESET\r\n", true);
  runCommand("AT+INIT\r\n", true);

  do{ //Try pairing with robot (fixed address)
    runCommand("AT+PAIR=74DA,EA,AF83DB,20\r\n", false);
  } while(strcmp(commBuf, "OK"));

  //Pairing successful, bind address and reboot
  runCommand("AT+CMODE=0\r\n", true);
  runCommand("AT+BIND=74DA,EA,AF83DB\r\n", true);
  digitalWrite(PIN_BLT_PWR, LOW);
  digitalWrite(PIN_BLT_KEY, LOW);
  delay(1000);
  digitalWrite(PIN_BLT_PWR, HIGH);
  bluetooth.begin(38400);

  irRecv.enableIRIn();
}
void loop(){
  //IR input lag workaround
  unsigned long timeout = millis();
  bool cmdModified = false;
  while(millis() - timeout < 150)
    if(irRecv.decode(&irResults)){
      cmdModified = true;
      irRecv.resume();
      break;
    }

  //Modify command based on button press, or lack thereof
  if(!cmdModified)  lastCommand[6] = 'N';
  else switch(irResults.value){
    case IR_UP:     lastCommand[6] = 'U'; break;
    case IR_LEFT:   lastCommand[6] = 'L'; break;
    case IR_DOWN:   lastCommand[6] = 'D'; break;
    case IR_RIGHT:  lastCommand[6] = 'R'; break;
    case IR_REPEAT: break;
    default:        lastCommand[6] = 'N';
  }
  runCommand(lastCommand, true);

  //Once command is sent, response will land in char* commBuf
  float sonarDist = atof(commBuf);

  //Indicate result with a "distance bar" of consecutive LEDs
  digitalWrite(PIN_LED_400 , sonarDist && sonarDist <= 400);
  digitalWrite(PIN_LED_200 , sonarDist && sonarDist <= 200);
  digitalWrite(PIN_LED_100 , sonarDist && sonarDist <= 100);
  digitalWrite(PIN_LED_50  , sonarDist && sonarDist <= 50);
  digitalWrite(PIN_LED_25  , sonarDist && sonarDist <= 25);
  digitalWrite(PIN_LED_10  , sonarDist && sonarDist <= 10);
}

/* ---------------------- */
void runCommand(char* cmd, bool enableResend){
  //First transmit
  bluetooth.write(cmd);

  if(enableResend){ //Wait for response, resend on timeout
    unsigned long timestamp = millis();
    while(!bluetooth.available())
      if(millis() - timestamp >= 1000){
        bluetooth.write(cmd);
        timestamp = millis();
      }
  }
  else //Wait patiently
    while(!bluetooth.available());

  //Message is underway, store it in char* commBuf
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
