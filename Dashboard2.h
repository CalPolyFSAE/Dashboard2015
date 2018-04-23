/*
 * Dashboard2.h
 *
 *  Created on: Jan 22, 2018
 *      Author: root
 */

#ifndef DASHBOARD2_DASHBOARD2_H_
#define DASHBOARD2_DASHBOARD2_H_

#include "stdint.h"

#define TEAM FE //Other: FSAE


#define VERSION 002

// uncomment to print version info through serial on startup
#define PRINT_VERSION_INFO

// uncomment to print extra information while running
#define DEBUG_PRINT

// serial baudrate
#define SERIAL_BAUD 9600

// a string helper that replaces the Arduino F macro and maintains const
class __FlashStringHelper;
#define FSTR(string_literal) (reinterpret_cast<const __FlashStringHelper*>(PSTR(string_literal)))

// port and pin is the register and bit position that should be read for
// pin state
#define BIT(addr, pin) {(uint16_t)(((uint8_t)&addr)<<8 | pin)}

//TODO: config stuff
namespace CONFIG
{
    // SUBSYSTEMS:
    // max number of jobs the Timing system can have
    constexpr uint8_t RSCMAXJOBS = 15;

    // INPUT:
    // timing interval for Input (ms)
    constexpr uint8_t INPUTINTERVAL = 10;

    // SCREEN:
    // screen update Interval
    constexpr uint8_t SCREENINTERVAL = 16;
    // update cycles before OnNoCANData event
    // will be about SCREENINTERVAL*MAXNOCANUPDATES ms
    constexpr uint8_t MAXNOCANUPDATES = 3;

}


/* stuff for led outputs
 * if (PORTB & BIT7)
 PORTB &= ~(BIT7);
 else
 PORTB |= BIT7;
 }
 if (DashboardData.yellowLED == OutputState::Flashing) {
 if (PORTB & BIT6)
 PORTB &= ~(BIT6);
 else
 PORTB |= BIT6;
 }
 if (DashboardData.greenLED == OutputState::Flashing) {
 if (PORTB & BIT5)
 PORTB &= ~(BIT5);
 else
 PORTB |= BIT5;*/

#endif /* DASHBOARD2_DASHBOARD2_H_ */
