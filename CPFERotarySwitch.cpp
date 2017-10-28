/*
 * CPFERotarySwitch.cpp
 *
 *  Created on: Jun 1, 2015
 *      Author: thomaswillson
 */

#include <stdint.h>
#include "CPFERotarySwitch.h"

uint8_t CPFERotarySwitch::positionCount;
volatile int8_t CPFERotarySwitch::currentRotaryADCConversion;
volatile uint8_t CPFERotarySwitch::positions[NUM_ROTARYS];
CPFEAVRAnalog CPFERotarySwitch::analogPins[NUM_ROTARYS];

void rotarySwitchResultHandler( uint16_t result, void *info ) {
    CPFERotarySwitch::resultHandler (result, info);
}
