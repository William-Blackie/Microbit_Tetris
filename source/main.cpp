/**
 * File: main.cpp
 * Author: William Blackie
 * Date: 21-01-2018
 * Desc: Basic Tetris game.
 */
#include "MicroBit.h"

MicroBit uBit;

MicroBitImage stage("0, 0, 0, 0, 0\n 0, 0, 0, 0, 0\n 0, 0, 0, 0, 0\n 0, 0, 0, 0, 0\n 0, 0, 0, 0, 0\n "); // Representation of the players stage
MicroBitImage playerSpace(stage); // Copy of stage so could be reset if chosen to
bool moveLeft, moveRight, canMove, canRotate = false; // Flags used in movement controll
int X, Y = 0; // Coordinates used for block movement
int newX = 1;
int newY = 0;


void onButtonA(MicroBitEvent e)
{
        if (e.value == MICROBIT_BUTTON_EVT_CLICK)
                moveLeft = true;
        // Move left
}

void onButtonB(MicroBitEvent e)
{
        moveRight = true;
        // Move right
}

void onAccellerometer(MicroBitEvent e){
        if(e.value == 11) {
                canRotate = true;
        }
}

int getPixelIntensity(bool pixel){
        if(pixel == true) {
                return 255; // Max pixel intensity
        }
        else{
                return 0; // Min pixel intensity
        }
}

bool moveBlock(int X, int Y, int newX, int newY, int blockCounter){
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

        if((playerSpace.getPixelValue(newX, newY) > 1) || (playerSpace.getPixelValue(newX+1, newY) > 1) || (newY > 4)) {
                return false;
        }
        else{
                bLeft = (playerSpace.getPixelValue(X, Y) > 1); // Find block placement
                bRight = (playerSpace.getPixelValue(X+1, Y) > 1);
                tLeft = (playerSpace.getPixelValue(X, Y-1) > 1);
                tRight = (playerSpace.getPixelValue(X+1,Y-1) > 1);

                if(Y == -1) { // Create first block
                        if(blockCounter == 0) {
                                bLeft = true;
                                bRight = true;

                        }
                        else if(blockCounter == 1) {
                                bLeft = true;
                                bRight = true;
                                tRight = true;
                        }
                        else{
                                bLeft = true;
                                bRight = true;
                                tRight = true;
                                tLeft = true;
                        }
                }

                playerSpace.setPixelValue(newX, newY, getPixelIntensity(bLeft)); // Move all the coloured pixel
                playerSpace.setPixelValue(newX+1, newY, getPixelIntensity(bRight));
                playerSpace.setPixelValue(newX, newY-1, getPixelIntensity(tLeft));
                playerSpace.setPixelValue(newX+1, newY-1, getPixelIntensity(tRight));

                playerSpace.setPixelValue(newX+2, newY-1, 0); // Remove old pixels
                playerSpace.setPixelValue(newX+2, newY-2, 0);
                playerSpace.setPixelValue(newX-1, newY-1, 0);
                playerSpace.setPixelValue(newX-1, newY-2, 0);
                playerSpace.setPixelValue(newX, newY-2, 0);
                playerSpace.setPixelValue(newX+1, newY-2, 0);


                X = newX;
                Y = newY;

                return true;
        }
}


void rotateBlock(int X, int Y){
        bool bLeft, bRight, tLeft, tRight = false;

        bLeft = (playerSpace.getPixelValue(X, Y) > 1); // Find pixel placement
        bRight = (playerSpace.getPixelValue(X+1, Y) > 1);
        tLeft = (playerSpace.getPixelValue(X, Y-1) > 1);
        tRight = (playerSpace.getPixelValue(X+1,Y-1) > 1);

        playerSpace.setPixelValue(X, Y, getPixelIntensity(bRight)); // Rotate pixels clockwise
        playerSpace.setPixelValue(X+1, Y, getPixelIntensity(tRight));
        playerSpace.setPixelValue(X, Y-1, getPixelIntensity(bLeft));
        playerSpace.setPixelValue(X+1, Y-1, getPixelIntensity(tLeft));

        uBit.display.print(playerSpace);
        canRotate = false;
}


bool checkBottomLine(){
        int counter = 0;

        for (int j =0; j <= 4; j++) { // Check entire bottom row for pixels
                counter +=  playerSpace.getPixelValue(j,4);
        }
        if(counter >= 1275) {
                playerSpace.shiftDown(1); //remove bottom row if full
                return true;
        }
        else{
                return false;
        }
}

bool checkEndGame(){
        int counter = 0;

        for (int k = 0; k<= 4; k++) {
                counter +=  playerSpace.getPixelValue(k,0);// Check entire top row for pixels
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

        bool canMove = true; // Used to
        int blockCounter = -1; // Used to select blocks
        uBit.messageBus.listen(MICROBIT_ID_BUTTON_A, MICROBIT_EVT_ANY, onButtonA); // Listener for button A
        uBit.messageBus.listen(MICROBIT_ID_BUTTON_B, MICROBIT_BUTTON_EVT_CLICK, onButtonB); // Listener for button B
        uBit.messageBus.listen(MICROBIT_ID_GESTURE, MICROBIT_ACCELEROMETER_EVT_SHAKE, onAccellerometer); // Listener for the acelerometer Shake gesture

        uBit.display.scroll("TETRIS");

        while(canMove) {
                newX = 1;
                X = 1;
                Y = -1;
                if(blockCounter <= 2) {
                        blockCounter++; // Changes block to be placed every time
                }
                else{
                        blockCounter = -1;
                }
                for(newY = 0; newY <= 4; newY++) {
                        uBit.display.print(playerSpace);
                        uBit.sleep(1000);
                        if(canRotate) {
                                rotateBlock(newX,newY);
                                moveLeft = false;
                                moveRight = false;
                                newY--;
                        }
                        else if(moveLeft == true) {
                                newX--;
                                moveLeft = false;
                        }
                        else if(moveRight == true) {
                                newX++;
                                moveRight = false;
                        }
                        if(checkBottomLine()) { // places a new block
                                newX = 1; //reset xy
                                newY = 0;
                        }
                        else{
                                if((moveBlock(X, Y, newX, newY, blockCounter)) == false) {
                                        canMove = checkEndGame();
                                        if(!(canMove)) { // Stop when game is done
                                                break;
                                        }
                                }
                        }
                }
        }

        uBit.display.clear();


        // If main exits, there may still be other fibers running or
        // registered event handlers etc.
        // Simply release this fiber, which will mean we enter the
        // scheduler. Worse case, we then
        // sit in the idle task forever, in a power efficient sleep.
        release_fiber();
}
