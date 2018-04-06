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

struct SubsystemControl::TC
{
    TC( const delegate& f, uint16_t Interval ) :
            func (f), Interval(Interval), ready(false), count(0){
    }

    inline void INT_Call_Tick() {
        if(count == 0) {
            count = Interval;
            ready = true;
        }else
            --count;
    }

    inline void Update()
    {
        if(ready){
            ready = false;
            func(0);
        }
    }

    delegate func;
    uint16_t Interval;

    volatile bool ready = false;
    volatile uint16_t count;
};

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

void SubsystemControl::MainLoop()
{
    for(uint8_t i = 0; i < CONFIG::RSCMAXJOBS; ++i)
    {
        if (TimeControl[i] != nullptr)
        {
            TimeControl[i]->Update ();
        }
    }
}

void SubsystemControl::INT_CALL_TickAllTC()
{
    for (uint8_t i = 0; i < CONFIG::RSCMAXJOBS; ++i)
    {
        if (TimeControl[i] != nullptr)
        {
            TimeControl[i]->INT_Call_Tick();
        }
    }
}

int8_t SubsystemControl::RegisterEvent(delegate const& Event, uint16_t Interval)
{
    int8_t slot = getNextFreeTC();
    if(slot >= 0)
    {
        TimeControl[slot] = new TC(Event, Interval);
        return slot;
    }else
        return -1;
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
    static SubsystemControl SC {};
    return SC;
}

// private

SubsystemControl::SubsystemControl()
{

}

int8_t SubsystemControl::getNextFreeTC()
{
    for (uint8_t i = 0; i < CONFIG::RSCMAXJOBS; ++i)
    {
        if (TimeControl[i] == nullptr)
        {
            return i;
        }
    }
    return -1;
}

///////////////////////////////
///
//////////////////////////////


