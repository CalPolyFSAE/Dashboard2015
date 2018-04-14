/*
 * ADCManager.h
 *
 *  Created on: Mar 29, 2017
 *      Author: root
 *
 *      This is another version of ADCManager that works with Subsystem
 *      TODO Make this work with delegates
 *      TODO variadic delegates
 */

#ifndef ADCMANAGER_H_
#define ADCMANAGER_H_

#include "Subsystem.h"

#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/atomic.h>


//this allows ADCManager to have a common interface for callbacks
//implement INT_Call_ADC_Finished_Implementation whenever you want to use ADCManager
class ADCManagerCallbackInterface
{
public:
    virtual ~ADCManagerCallbackInterface() {}

    virtual void INT_Call_ADC_Finished(const uint16_t& value, uint8_t channel) = 0;
protected:
    ADCManagerCallbackInterface() {}
};

//
class ADCManager : public Subsystem<ADCManager>
{
public:
    friend class Subsystem<ADCManager>;

    //start an ADC read
    bool StartRead(ADCManagerCallbackInterface* resultHandler, uint8_t channel);

    //if the ADC is available for a read request
    bool ADCAvailable();

    //do not call. used to handle the ADC read result
    void INT_ADCFinished();

protected:
    ADCManager() :
        Subsystem(0)// no update event
    {}

    //setup registers for ADC
    virtual void Init() override;
    void Update(uint8_t) {}

    //pointer to the current adc read Callback function
    static ADCManagerCallbackInterface* currentReadCallback;
    //channel of the current read
    static uint8_t channel;
};

#endif /* ADCMANAGER_H_ */
