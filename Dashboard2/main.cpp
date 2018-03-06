/*
 * main.cpp
 *
 *  Created on: Jan 21, 2018
 *      Author: root
 */

#include "Subsystem.h"
#include "Display.h"


int main0()
{
    // initialize Subsystems
    Display& Screen = Display::StaticClass();

    Core.InitSubsystems();

    return 0;
}


