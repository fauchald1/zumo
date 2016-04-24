private PLabBridge pBridge;

private String received = null;

void bindPLabBridge (PLabBridge bridge) {

  pBridge = bridge;

  bridge.subscribeMessages (new PLabRecv() {
    public void receive (String string) {
      // code on BT receive goes here.
    }
  });
  size(bridge.getWidth(), bridge.getHeight());
}

void btWrite(String string) {
  if (pBridge != null) {
    pBridge.send(string);
  }
}

import processing.serial.*;
Serial myPort;
String val;

////////////////////////
// SETTING UP BUTTONS //
////////////////////////
int[] upButtonFrame = {150,60,100,50};
String upButtonString = "↑";

int[] downButtonFrame = {150,180,100,50};
String downButtonString = "↓";

int[] leftButtonFrame = {80,120,100,50};
String leftButtonString = "←";

int[] rightButtonFrame = {215,120,100,50};
String rightButtonString = "→";

///////////////////////
// SETTING UP OUTPUT //
///////////////////////
int[] outputFrame = {150,10,100,30};
String outputText = "";



void setup(){
  size(400,400);
  background(137,189,211);
  stroke(0);
//////////
// PORT //
//////////
  println(Serial.list());
  String portName = Serial.list()[1];
  myPort = new Serial(this, portName, 9600);

}

int tempVal = 0;
void draw(){
  if (myPort.available() > 0) {
    tempVal = myPort.read();
    println(tempVal);
    //val = myPort.read();
    //val = myPort.readStringUntill(n);
    drawText(outputFrame,val);
  }
  //drawText(outputFrame,val);
  drawButton(upButtonFrame,upButtonString);
  drawButton(leftButtonFrame,leftButtonString);
  drawButton(rightButtonFrame,rightButtonString);
  drawButton(downButtonFrame,downButtonString);
  mousePressed();
}


//////////////////////////
// DRAWING BUTTON FIELD //
//////////////////////////
void drawButton(int[] xywh, String buttonText) {
    boolean mousePressedInButton = mouseInside(xywh) && mousePressed;
    stroke(201,201,201);
    if (mousePressedInButton) {
      fill(201,201,201); }
    else {
      fill(227,227,227);
    }
    rect(xywh[0],xywh[1],xywh[2],xywh[3],30);
    if (mousePressedInButton) {
      fill(255); }
    else {
      fill(0);
    }
    int tSize = xywh[3] * 3 / 5;
    textSize(tSize);
    textAlign(CENTER, CENTER);
    text(buttonText,xywh[0]+xywh[2]/2,xywh[1]+xywh[3]/2-2);
}
////////////////////////
// DRAWING TEXT FIELD //
////////////////////////
void drawText(int[] xywh, String buttonText) {
    stroke(0);
    fill(255);
    rect(xywh[0],xywh[1],xywh[2],xywh[3]);
    fill(0);
    int tSize = xywh[3] * 3 / 5;
    textSize(tSize);
    textAlign(LEFT, CENTER);
    text(buttonText,xywh[0]+2,xywh[1]+xywh[3]/2-2);
}

boolean inside(int x,int y, int x0,int y0,int w,int h) {
  return (((x >= x0) && (x < (x0+w))) &&
      ((y >= y0) && (y < (y0+h))));
}

boolean mouseInside(int[] rect) {
  return inside(mouseX, mouseY, rect[0],rect[1],rect[2],rect[3]);
}

void mousePressed(){
  if (mouseInside(upButtonFrame)) {
    myPort.write("F"); //FORWARD
  }else if (mouseInside(downButtonFrame)) {
    myPort.write("B"); //BACK
  }else if (mouseInside(leftButtonFrame)) {
    myPort.write("L"); //LEFT
  }else if (mouseInside(rightButtonFrame)) {
    myPort.write("R"); // RIGHT
  }
}
