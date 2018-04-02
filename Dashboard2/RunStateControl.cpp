/*
 * RunStateControl.cpp
 *
 *  Created on: Mar 20, 2018
 *      Author: root
 */

#include "RunStateControl.h"

RunStateControl::RunStateControl() :
    TimeControl{}
{

}

void RunStateControl::MainLoop()
{

}

void RunStateControl::INT_CALL_TickAllTC()
{
    for (uint8_t i = 0; i < CONFIG::RSCMAXJOBS; ++i)
    {
        if (TimeControl[i] != nullptr)
        {
            TimeControl[i]->INT_Call_Tick();
        }
    }
}

int8_t RunStateControl::RegisterEvent(delegate& Event, uint16_t Interval)
{
    int8_t slot = getNextFreeTC();
    if(slot >= 0)
    {
        TimeControl[slot] = new TC(Event, Interval);
        return slot;
    }else
        return -1;
}

int8_t RunStateControl::getNextFreeTC()
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

