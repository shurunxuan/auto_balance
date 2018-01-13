/*
 * main.c
 *
 *  Created on: 2018/01/10
 *      Author: Shu Runxuan
 *
 * Description:
 *
 *         This is the entrance point of the program;
 *  the file includes the base of the MSP432 program.
 *  Should not be modified.
 *
 */


#include "main_prog.h"
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

int main(void)
 {
    // Disabling the Watchdog
    MAP_WDT_A_holdTimer();

    // Startup program which runs only once
    setup();

    // Enabling interrupts and starting the watchdog timer
    MAP_Interrupt_enableSleepOnIsrExit();
    MAP_Interrupt_enableMaster();

    while (1)
    {
        // Main program which runs continuously
        loop();
    }
}
