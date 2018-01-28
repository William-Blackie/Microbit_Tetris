/**
 * File: main.cpp
 * Author: Benedict R. Gaster
 * Date: 21-01-2018
 * Desc: Basic Micro:bit example taken from BBC examples.
 * Copyright: University of West of England 2017
 */
#include "MicroBit.h"

MicroBit uBit;

int main()
{
    // Initialise the micro:bit runtime.
    uBit.init();

    // Insert your code here!
    uBit.display.scroll("HELLO WORLD! :)");

    // If main exits, there may still be other fibers running or 
    // registered event handlers etc.
    // Simply release this fiber, which will mean we enter the 
    // scheduler. Worse case, we then
    // sit in the idle task forever, in a power efficient sleep.
    release_fiber();
}

