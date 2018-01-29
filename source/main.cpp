/**
 * File: main.cpp
 * Author: Benedict R. Gaster
 * Date: 21-01-2018
 * Desc: Basic Micro:bit example taken from BBC examples.
 * Copyright: University of West of England 2017
 */
#include "MicroBit.h"

MicroBit uBit;
MicroBitImage LBlock("255,0\n255,0\n255,255\n"); //L shaped block
MicroBitImage SqrBlock("255,255\n255,255\n"); // Square shaped block
MicroBitImage StrBlock("255,255,255,0\n"); // straight shaped block
MicroBitImage Stage("0, 0, 0, 0, 0\n 0, 0, 0, 0, 0\n 0, 0, 0, 0, 0\n 0, 0, 0, 0, 0\n 0, 0, 0, 0, 0\n ");
MicroBitImage PlayerSpace(Stage);
bool MoveLeft, MoveRight = false;
//int X, Y = 0;

void onButtonA(MicroBitEvent e)
{
  MoveLeft = true;
    //move left
}

void onButtonB(MicroBitEvent e)
{
  MoveRight = true;
  //move right
}

void moveBlock(int X, int Y){
  if(Y <= 3){ //prevents blocks going offscreen
    PlayerSpace.setPixelValue(X,Y,255);//set new pixel
    PlayerSpace.setPixelValue(X,Y-1,0); //whipe old pixel

    PlayerSpace.setPixelValue(X+1,Y,255);//set new pixel
    PlayerSpace.setPixelValue(X+1,Y-1,0); //whipe old pixel

    PlayerSpace.setPixelValue(X,Y+1,255);//set new pixel
    PlayerSpace.setPixelValue(X+1,Y-1,0); //whipe old pixel

    PlayerSpace.setPixelValue(X+1,Y+1,255);//set new pixel
    PlayerSpace.setPixelValue(X-1,Y-1,0); //whipe old pixel
  }
}

bool checkBottomLine(int X, int Y){
  int counter = 0;

  for (int j =0; j <= 4; j++){
    counter +=  PlayerSpace.getPixelValue(j,4);
  }
  if(counter >= 1275){
    //moveBlock(X, Y+1);
    PlayerSpace.shiftDown(1); //remove bottom row if full
    return true;
  }
  else{
    return false;
  }
}

int main()
{
    // Initialise the micro:bit runtime.
    uBit.init();
    uBit.messageBus.listen(MICROBIT_ID_BUTTON_A, MICROBIT_BUTTON_EVT_CLICK, onButtonA);
    uBit.messageBus.listen(MICROBIT_ID_BUTTON_B, MICROBIT_BUTTON_EVT_CLICK, onButtonB);

    //add gyro to tell user to hold micro bit right way up
    //shaking game to clear ground blocks
    // Insert your code here

    /*
        0 1 2 3 4 X
    0   x x x x x
    1   x x x x x
    2   x x x x x
    3   x x x x x
    4   x x x x x
    Y
    */


while(true){
  int X = 1;
  bool lineCheck = false;
for(int Y = 0; Y <= 4; Y++){
    uBit.sleep(750);
    uBit.display.print(PlayerSpace);
    if(MoveLeft == true){
      X--;
      moveBlock(X,Y);
      MoveLeft = false;
    }
    else if(MoveRight == true){
      X++;
      moveBlock(X,Y);
      MoveRight = false;
    }
    else{
      if(checkBottomLine(X, Y) == true){
        X,Y =0;
        break;
      }
      else{
          moveBlock(X,Y); //move downs
      }
    }
    }
  }
    uBit.display.clear(); //whipe display


    // If main exits, there may still be other fibers running or
    // registered event handlers etc.
    // Simply release this fiber, which will mean we enter the
    // scheduler. Worse case, we then
    // sit in the idle task forever, in a power efficient sleep.
    release_fiber();
}
