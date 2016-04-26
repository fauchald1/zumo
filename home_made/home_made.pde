import processing.serial.*;
Serial port;

Button upButton;
Button leftButton;
Button rightButton;
Button downButton;

SensorData mySensorData;
////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//   /////////     /////////     //////////////    ///     ///     /////////  //
//  ///            ///                ///          ///     ///     ///   ///  //
//   ////////      /////////          ///          ///     ///     /////////  //
//         ///     ///                ///          ///     ///     ///        //
//   ////////      /////////          ///           /////////      ///        //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////
/////         /////         /////              ////   /////   /////         ////
////   ////////////   ////////////////   //////////   /////   /////   ///   ////
/////        //////         //////////   //////////   /////   /////         ////
///////////   /////   ////////////////   //////////   /////   /////   //////////
/////        //////         //////////   ///////////         //////   //////////
////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//   /////////     /////////     //////////////    ///     ///     /////////  //
//  ///            ///                ///          ///     ///     ///   ///  //
//   ////////      /////////          ///          ///     ///     /////////  //
//         ///     ///                ///          ///     ///     ///        //
//   ////////      /////////          ///           /////////      ///        //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////
/////         /////         /////              ////   /////   /////         ////
////   ////////////   ////////////////   //////////   /////   /////   ///   ////
/////        //////         //////////   //////////   /////   /////         ////
///////////   /////   ////////////////   //////////   /////   /////   //////////
/////        //////         //////////   ///////////         //////   //////////
////////////////////////////////////////////////////////////////////////////////
color fillValue = color(150);
void setup(){

  size(400,400);
  smooth();
  //-------------------------- 'Kobler til' porten -----------------------------
  println("Available serail ports: ");
  // Printer ut tilgjengelige porter
  println(Serial.list());
  // Finn ønsket port i listen og erstatt [0] hvis det trenges
  //port = new Serial(this, Serial.list()[0], 9600);
  //-------------------------- Lager nye knapper -------------------------------
  upButton    = new Button("↑", 150,  60, 100, 50);
  leftButton  = new Button("←",  80, 120, 100, 50);
  rightButton = new Button("→", 215, 120, 100, 50);
  downButton  = new Button("↓", 150, 180, 100, 50);

  //------------------------ Lager sensorData felt  ----------------------------
  mySensorData = new SensorData("42", 150, 10, 100, 30);
}
void draw(){
  //--------------------------- Tegner knapper ---------------------------------
  upButton.Draw();
  leftButton.Draw();
  rightButton.Draw();
  downButton.Draw();

  //------------------------ Tegner sensorData felt ----------------------------
  mySensorData.Draw();
}



////////////////////////////////////////////////////////////////////////////////
/////////////////////////////// BUTTON CLASS ///////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

class Button {
  String label, pv;
  float x, y ,w ,h;
  color fillValue;
  boolean MouseIsOver() {
    if (mouseX > x && mouseX < (x + w) && mouseY > y && mouseY < (y + h)) {
      return true;
    }
    return false;
  }

  // Constructor
  Button(String labelB, float xpos, float ypos, float widthB, float heightB) {
    label = labelB;
    x = xpos;
    y = ypos;
    w = widthB;
    h = heightB;
    pv = printValue(labelB);
    fillValue = 218;
  }

  String printValue(String labelB) {
    String value = "";
    if (labelB == "↑") {
      value = "up";
    }else if (labelB == "←") {
      value = "left";
    }else if (labelB == "→") {
      value = "right";
    }else if (labelB == "↓") {
      value = "down";
    }
    return value;
  }

  void Draw() {
    stroke(141);
    if (!keyPressed) {
      if (MouseIsOver() && mousePressed) {
        sendToPort(pv);
        fillValue = 150;
      }else{
        fillValue = 218;
      }
    }
    fill(fillValue);
    rect(x,y,w,h,10);
    textAlign(CENTER, CENTER);
    textSize(32);
    fill(0);
    text(label, x + (w/2), y + (h/2.5));
  }

}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////  KEYPRESSED  //////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

void keyPressed() {
  if (key == CODED) {
    if (keyCode == 38) {
      upButton.fillValue = 150;
      sendToPort("up");
    }else if (keyCode == 37) {
      leftButton.fillValue = 150;
      sendToPort("left");
    }else if (keyCode == 39) {
      rightButton.fillValue = 150;
      sendToPort("right");
    }else if (keyCode == 40){
      downButton.fillValue = 150;
      sendToPort("down");
    }
  }
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////  DISPLAY SENSOR DATA //////////////////////////////
////////////////////////////////////////////////////////////////////////////////
class SensorData {
  String label;
  float x,y,w,h;

  SensorData(String labelS, float xpos, float ypos, float widthS, float heightS){
    label = labelS;
    x = xpos;
    y = ypos;
    w = widthS;
    h = heightS;
  }

  void Draw(){
    stroke(0);
    fill(255);
    rect(x,y,w,h);
    fill(0);
    textAlign(LEFT, CENTER);
    textSize(12);
    text(label, x + 5, y + (h/2-2));
  }
}



////////////////////////////////////////////////////////////////////////////////
//////////////////////////// INNCOMMING SERIAL DATA ////////////////////////////
////////////////////////////////////////////////////////////////////////////////

String valueFromPort = "";
void serialEvent(Serial serialPort) {
  String portData = new String(serialPort.readBytesUntil('\n'));
  if (portData != null) {
    valueFromPort = portData;
    println("Port data: " + valueFromPort);
  }

}

////////////////////////////////////////////////////////////////////////////////
/////////////////////////////// OUTGOING DATA //////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

void sendToPort(String string) {
  // port.write(string + "\n");
  println("Sent: " + string);
}
