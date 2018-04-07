/*
 * main.cpp
 *
 *  Created on: Jan 21, 2018
 *      Author: root
 */

#include "AVRLibrary/arduino/Arduino.h"

#include "Screen.h"
#include "Subsystem.h"
#include "Input.h"

#include "Delegate.h"

int main0()
{
    Serial.begin(9600);
    Serial.println("START");

    // initialize Subsystems
    //Screen& ScreenS = Screen::StaticClass();

    Input& input = Input::StaticClass();

    //Core.InitSubsystems();

    return 0;
}


