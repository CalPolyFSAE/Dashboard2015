/*
 * Subsystem.cpp
 *
 *  Created on: Jan 15, 2018
 *      Author: root
 */

#include "Subsystem.h"


/////////////////////////////////////////////////
// SubsystemControl
/////////////////////////////////////////////////

uint8_t SubsystemControl::RegisterSubsystem(AbstractSubsystem& sys)
{
    for(uint8_t i = 0; i < NUMSUBSYSTEMS; ++i)
    {
        if(systems[i] == nullptr)
        {
            systems[i] = &sys;
            return 0;
        }
    }
    return 1;
}

void SubsystemControl::UpdateSubsystems()
{
    for (uint8_t i = 0; i < NUMSUBSYSTEMS; ++i)
    {
        systems[i]->Update ();
    }
}

void SubsystemControl::InitSubsystems()
{
    if(!bHaveSubInit)
    {
        for (uint8_t i = 0; i < NUMSUBSYSTEMS; ++i)
        {
            systems[i]->Init ();
        }
        bHaveSubInit = true;
    }
}

//get instance
SubsystemControl& SubsystemControl::StaticClass()
{
    // dynamic initialization
    static SubsystemControl* SC = new SubsystemControl{};
    return *SC;
}

// private

SubsystemControl::SubsystemControl()
{
    //don't do anything with global variables here
}


