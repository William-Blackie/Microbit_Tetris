# Microbit_Tetris

a basic implementation of Tetris: the aim of the game is to sort falling blocks of different sizes(1*1, 2*1, 2*2) using the A and B buttons to move the blocks left or so they lay flat on the floor, shaking the Microbit will rotate the blocks clockwise,  once the bottom layer is covered then the layer will disapear allowing the player more space/ time to continue playing.

## Getting Started

Clone the repository.

### Prerequisites

You will need a command line tool called yotta: https://github.com/ARMmbed/yotta follow the instalation instructions and then open the repository you cloned.

## Instatlation

```
yt target bbc-microbit-classic-gcc
```
This selects the microbit for yotta to build for.

```
yt build
```
this will build all the files needed to run Tetris on the Microbit.

```
 cp build/bbc-microbit-classic-gcc/source/iot-example-combined.hex/media/YOUR_USER_NAME/MICROBIT
```
this command will copy the hex file built by yotta containing tetris onto your microbit allowing you to play.

### Controlls 

left button (A): moves block one pixel left.
Right button (B): moves block one pixel right.
Shake Microbit(accelerometer): roates block clockwise one quater turn.

## Authors

* **William Blackie**


