/*
 * RunStateControl.h
 *
 *  Created on: Jan 22, 2018
 *      Author: root
 */

#ifndef DASHBOARD2_RUNSTATECONTROL_H_
#define DASHBOARD2_RUNSTATECONTROL_H_

#include "Dashboard2.h"
#include "Delegate.h"
#include "stdint.h"

class RunStateControl
{
public:
    static RunStateControl& StaticClass() {
        static RunStateControl RSC
            { };
        return RSC;
    }

    void MainLoop();

    void INT_CALL_TickAllTC();

    int8_t RegisterEvent(delegate& Event, uint16_t Interval);

    //bool UnregisterEvent(int8_t Eventslot)

private:
    RunStateControl();

    struct TC
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

    TC* TimeControl[CONFIG::RSCMAXJOBS];

    int8_t getNextFreeTC();
};

#endif /* DASHBOARD2_RUNSTATECONTROL_H_ */
