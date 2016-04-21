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

////////////////////////
// SETTING UP BUTTONS //
////////////////////////
int[] upButtonFrame = {150,40,100,50};
String upButtonString = "^";

int[] downButtonFrame = {150,160,100,50};
String downButtonString = "˅";

int[] leftButtonFrame = {80,100,100,50};
String leftButtonString = "<";

int[] rightButtonFrame = {215,100,100,50};
String rightButtonString = ">";

///////////////////////
// SETTING UP OUTPUT //
///////////////////////
int[] outputFrame = {150,10,100,25};
String outputText = "";



void setup(){
  size(400,600);
  background(242, 196, 255);
  stroke(0);
}

void draw(){
  drawText(outputFrame,outputText);
  drawButton(upButtonFrame,upButtonString);
  drawButton(leftButtonFrame,leftButtonString);
  drawButton(rightButtonFrame,rightButtonString);
  drawButton(downButtonFrame,downButtonString);
}


//////////////////////////
// DRAWING BUTTON FIELD //
//////////////////////////
void drawButton(int[] xywh, String buttonText) {
    boolean mousePressedInButton = mouseInside(xywh) && mousePressed;
    stroke(0);
    if (mousePressedInButton) {
      fill(0); }
    else {
      fill(255);
    }
    rect(xywh[0],xywh[1],xywh[2],xywh[3],10);
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
    btWrite("FORWARD");
  }else if (mouseInside(downButtonFrame)) {
    btWrite("BACK");
  }else if (mouseInside(leftButtonFrame)) {
    btWrite("LEFT");
  }else if (mouseInside(rightButtonFrame)) {
    btWrite("RIGHT");
  }
}
