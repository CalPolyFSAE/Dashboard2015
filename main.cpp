/*
 * main.cpp
 *
 *  Created on: Apr 5, 2015
 *      Author: thomaswillson
 */
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include <AVRLibrary/arduino/Arduino.h>

#define TEAM FSAE //Other: FSAE

#if TEAM==FE
#include "FEDashLCD.h"
FEDashLCD dashLCD;
#else
#include "FSAEDashLCD.h"
FSAEDashLCD dashLCD;
#endif

SIGNAL(TIMER2_OVF_vect) {
	dashLCD.TIMER2_OVF_INT();
}

void canRxIntFunc(CPFECANLib::MSG *msg, uint8_t mobNum) {
	dashLCD.CAN_RX_Int(msg, mobNum);
}

int main() {
	init(); //Arduino Timer Initialization, uses Timer 0 for millis() and other timing functions.
	dashLCD.init(&canRxIntFunc);

	while (1) {
		dashLCD.updateDisplay();
	}
}
