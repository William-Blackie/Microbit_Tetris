/**
 * File: main.cpp
 * Author: Benedict R. Gaster
 * Date: 21-01-2018
 * Desc: Basic Micro:bit example taken from BBC examples.
 * Copyright: University of West of England 2017
 */
#include "MicroBit.h"

MicroBit uBit;

void onButtonA(MicroBitEvent e)
{
    uBit.display.print("A");
    //move left
}

void onButtonB(MicroBitEvent e)
{
    uBit.display.print("B");
    //move right
}

int main()
{
    // Initialise the micro:bit runtime.
    uBit.init();
    uBit.messageBus.listen(MICROBIT_ID_BUTTON_A, MICROBIT_BUTTON_EVT_CLICK, onButtonA);
    uBit.messageBus.listen(MICROBIT_ID_BUTTON_B, MICROBIT_BUTTON_EVT_CLICK, onButtonB);
    //add gyro to tell user to hold micro bit right way up
    //shaking game increase brightness?
    // Insert your code here
    MicroBitImage LBlock("255,0\n255,0\n255,255\n"); //L shaped block
    //MicroBitImage SqrBlock("255,255\n255,255\n"); // Square shaped block
    MicroBitImage StrBlock("255,255,255,0\n"); // straight shaped block
    MicroBitImage Stage("0,0,0,0,0\n0,0,0,0,0\n0,0,0,0,0\n0,0,0,0,0\n0,0,0,0,0\n");
         //paste(img,left most x cords, (highest y cord)

    /*
        0 1 2 3 4
    0   x x x x x
    1   x x x x x
    2   x x x x x
    3   x x x x x
    4   x x x x x
    */
      Stage.paste(StrBlock,1,0);
    for(int i = 0; i <=4; i++){
        uBit.display.print(Stage);
        uBit.sleep(500);
        Stage.shiftDown(1);
      }

    uBit.display.clear(); //whipe display


    // If main exits, there may still be other fibers running or
    // registered event handlers etc.
    // Simply release this fiber, which will mean we enter the
    // scheduler. Worse case, we then
    // sit in the idle task forever, in a power efficient sleep.
    release_fiber();
}
