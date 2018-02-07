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
int newX, oldX = 1;
int newY, oldY = 0;


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

int pixelIntensity(bool pixel){
        if(pixel) {
                return 255; // Max pixel intensity
        }
        else{
                return 0; // Min pixel intensity
        }
}

bool moveBlock(int X, int Y, int newX, int newY){
        bool bLeft, bRight, tLeft, tRight = false;
        if(newY >4) { // Prevent blocks going offscreen
                newY = 4;
        }

        if(newX > 4) {// Prevent blocks going offscreen
                newX = 4;
        }
        else if(newX < 0) {
                newX = 0;

        }

        if((PlayerSpace.getPixelValue(newX, newY) > 1) || (PlayerSpace.getPixelValue(newX+1, newY) > 1) || (newY > 4)) {
                //newX = 1;
                //  newY = 0;
                return false;
        }
        else{
                bLeft = (PlayerSpace.getPixelValue(X, Y) > 1); // Find block placement
                bRight = (PlayerSpace.getPixelValue(X+1, Y) > 1);
                tLeft = (PlayerSpace.getPixelValue(X, Y-1) > 1);
                tRight = (PlayerSpace.getPixelValue(X+1,Y-1) > 1);

                if(Y == -1 && !(bLeft && bRight && tLeft && tRight)) { // Create first block
                        bLeft = true;
                        bRight = true;
                        tRight = true;
                        tLeft = true;
                }

                PlayerSpace.setPixelValue(newX, newY, pixelIntensity(bLeft)); // Move all the coloured pixel
                PlayerSpace.setPixelValue(newX+1, newY, pixelIntensity(bRight));
                PlayerSpace.setPixelValue(newX, newY-1, pixelIntensity(tLeft));
                PlayerSpace.setPixelValue(newX+1, newY-1, pixelIntensity(tRight));

                PlayerSpace.setPixelValue(newX+2, newY-1, 0); // Remove old pixels
                PlayerSpace.setPixelValue(newX+2, newY-2, 0);
                PlayerSpace.setPixelValue(newX-1, newY-1, 0);
                PlayerSpace.setPixelValue(newX-1, newY-2, 0);


                X = newX;
                Y = newY;

                return true;
        }
}


void rotateBlock(int X, int Y){
        bool bLeft, bRight, tLeft, tRight = false;

        bLeft = (PlayerSpace.getPixelValue(X, Y) > 1); // Find pixel placement
        bRight = (PlayerSpace.getPixelValue(X+1, Y) > 1);
        tLeft = (PlayerSpace.getPixelValue(X, Y-1) > 1);
        tRight = (PlayerSpace.getPixelValue(X+1,Y-1) > 1);

        PlayerSpace.setPixelValue(X, Y, pixelIntensity(bRight)); // Rotate pixels
        PlayerSpace.setPixelValue(X+1, Y, pixelIntensity(tRight));
        PlayerSpace.setPixelValue(X, Y-1, pixelIntensity(bLeft));
        PlayerSpace.setPixelValue(X+1, Y-1, pixelIntensity(tLeft));
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
                newX = 1;
                X = 1;
                Y = -1;
                for(newY = 0; newY <= 4; newY++) {
                        uBit.display.print(PlayerSpace);
                        uBit.sleep(1000);
                        if(canRotate) {
                                rotateBlock(newX,newY);
                                MoveLeft = false;
                                MoveRight = false;
                                newY--;
                        }
                        else if(MoveLeft == true) {
                                newX--;
                                MoveLeft = false;
                        }
                        else if(MoveRight == true) {
                                newX++;
                                MoveRight = false;
                        }
                        /*else if(accelerometer.getGesture() == 11) { //SHAKE
                                for(int i = 0; i < 4; i++) {
                                        PlayerSpace.setPixelValue(0, i, 255); //make colour bottom row to be cleaned up
                                }
                           }*/
                        if(checkBottomLine()) { // places a new block
                                newX = 1; //reset xy
                                newY = 0;
                        }
                        else{
                                if((moveBlock(X, Y, newX, newY)) == false) {
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
