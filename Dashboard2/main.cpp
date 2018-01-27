/*
 * main.cpp
 *
 *  Created on: Jan 21, 2018
 *      Author: root
 */

#include "Subsystem.h"
#include "CANData.h"
#include "Display.h"

// Global variables
// TODO: don't like global variables
SubsystemControl& Core = SubsystemControl::StaticClass();

int main0()
{
    // initialize Subsystems
    // there are no global subsystem variables, this allows initialization order control
    // Core needs to exist before these
    CANData& CAN = CANData::StaticClass();
    Display& Screen = Display::StaticClass();

    Core.InitSubsystems();

    return 0;
}


