/**
 * File: main.cpp
 * Author: William Blackie
 * Date: 21-01-2018
 * Desc: Basic Tetris game.
 */
#include "MicroBit.h"

MicroBit uBit;
MicroBitImage Stage("0, 0, 0, 0, 0\n 0, 0, 0, 0, 0\n 0, 0, 0, 0, 0\n 0, 0, 0, 0, 0\n 0, 0, 0, 0, 0\n ");
MicroBitImage PlayerSpace(Stage);
bool MoveLeft, MoveRight, canMove, canRotate = false;
int X, Y = 0;
int oldX = 1;
int oldY = 0;


void onButtonA(MicroBitEvent e)
{
        if (e.value == MICROBIT_BUTTON_EVT_CLICK)
                MoveLeft = true;
        // Move left
}

void onButtonB(MicroBitEvent e)
{
        MoveRight = true;
        // Move right
}

void onAccellerometer(MicroBitEvent e){
        if(e.value == 11) {
                canRotate = true;
        }
}

bool moveBlock(int X, int Y){
        if(Y >4) { // Prevent blocks going offscreen
                Y = 4;
        }

        if(Y == 0) { // Stop old blocks getting overwritten
                oldX = X - 1;
                oldY = Y - 1;
        }

        if(X > 4) {// Prevent blocks going offscreen
                X = 4;
        }
        else if(X < 0) {
                X = 0;
        }

        if((PlayerSpace.getPixelValue(X, Y) > 1) || (PlayerSpace.getPixelValue(X+1, Y) > 1) || (Y > 4)) {
                return false;
                oldX = 1;
                oldY = 0;
        }
        else{
                PlayerSpace.setPixelValue(X, Y, 255);//set new pixel 2 BLOCK
                PlayerSpace.setPixelValue(oldX, oldY, 0); //whipe old pixel

                PlayerSpace.setPixelValue(X+1, Y, 255);
                PlayerSpace.setPixelValue(oldX+1, oldY, 0);

                oldX = X;
                oldY = Y;
                return true;
        }
}


void rotateBlock(int X, int Y){
        int tempX = X;
        int tempY = Y;
        bool bLeft, bRight, tLeft, tRight = false;

        if((PlayerSpace.getPixelValue(X, Y)) > 0) {
                PlayerSpace.setPixelValue(X, Y+1, 255);
                PlayerSpace.setPixelValue(X, Y, 0);
                tempY = Y+1;
        }
        if((PlayerSpace.getPixelValue(X, Y+1)) > 0) {
                PlayerSpace.setPixelValue(X+1, Y+1, 255);
                PlayerSpace.setPixelValue(X, Y+1, 0);

                tempY = Y + 1;
                tempX = X + 1;
        }
        if((PlayerSpace.getPixelValue(X+1, Y+1)) > 0) {
                PlayerSpace.setPixelValue(X+1, Y, 255);
                PlayerSpace.setPixelValue(X+1, Y+1, 0);
                tempX = +1;
        }
        if((PlayerSpace.getPixelValue(X+1, Y)) > 0) {
                PlayerSpace.setPixelValue(X, Y, 255);
                PlayerSpace.setPixelValue(X+1, Y, 0);
        }

        Y = tempY;
        X = tempX;
        canRotate = false;
}

bool checkBottomLine(){
        int counter = 0;

        for (int j =0; j <= 4; j++) {
                counter +=  PlayerSpace.getPixelValue(j,4);
        }
        if(counter >= 1275) {
                //moveBlock(X, Y+1);
                PlayerSpace.shiftDown(1); //remove bottom row if full
                return true;
        }
        else{
                return false;
        }
}

bool checkEndGame(){
        int counter = 0;

        for (int k = 0; k<= 4; k++) {
                counter +=  PlayerSpace.getPixelValue(k,0);//top row
        }
        if (counter >= 1) {
                uBit.display.print("game over");
                return false;
        }
        else{
                return true;
        }
}

int main()
{
        // Initialise the micro:bit runtime.
        uBit.init();
        uBit.messageBus.listen(MICROBIT_ID_BUTTON_A, MICROBIT_EVT_ANY, onButtonA);
        uBit.messageBus.listen(MICROBIT_ID_BUTTON_B, MICROBIT_BUTTON_EVT_CLICK, onButtonB);
        uBit.messageBus.listen(MICROBIT_ID_GESTURE, MICROBIT_ACCELEROMETER_EVT_SHAKE, onAccellerometer);
        //uBit.messageBus.listen(MICROBIT_ID_ACCELEROMETER, MICROBIT_EVT_ANY, onAccellerometer);

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

        bool canMove = true; //can player still place a block
        while(canMove) {
                int X = 1;
                for(int Y = 0; Y <= 4; Y++) {
                        uBit.display.print(PlayerSpace);
                        uBit.sleep(1000);
                        if(canRotate) {
                                rotateBlock(X,Y);
                                MoveLeft = false;
                                MoveRight = false;
                        }
                        else if(MoveLeft == true) {
                                X--;
                                MoveLeft = false;
                        }
                        else if(MoveRight == true) {
                                X++;
                                MoveRight = false;
                        }
                        /*else if(accelerometer.getGesture() == 11) { //SHAKE
                                for(int i = 0; i < 4; i++) {
                                        PlayerSpace.setPixelValue(0, i, 255); //make colour bottom row to be cleaned up
                                }
                        }*/
                        if(checkBottomLine()) { // places a new block
                                X = 1; //reset xy
                                Y = 0;
                        }
                        else{
                                if((moveBlock(X, Y)) == false) {
                                        canMove = checkEndGame();
                                        if(!(canMove)) { // Stop trying to place
                                                break;
                                        }
                                }
                                else{
                                        checkBottomLine();
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
