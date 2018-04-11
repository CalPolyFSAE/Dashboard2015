/*
 * Input.h
 *
 *  Created on: Mar 29, 2018
 *      Author: root
 */

#ifndef DASHBOARD2_INPUT_H_
#define DASHBOARD2_INPUT_H_

#include "Subsystem.h"
#include "ADCManager.h"
#include "Dashboard2.h"
#include "Delegate.h"


// TODO make ADCManagerCallbackInterface use delegates
class Input : public Subsystem<Input>, ADCManagerCallbackInterface
{
public:
    friend class Subsystem<Input>;// this is necessary for StaticClass function

    // attach callback to button
    int8_t BindOnChangeButton(const delegate& func);

    // attach callback to rotary
    int8_t BindOnChangeRotary(const delegate& func);

    virtual void INT_Call_ADC_Finished(const uint16_t& value, uint8_t channel) override;// start next ADC read on complete

protected:

    // ADC states
    volatile uint16_t rotaryADCValues[CONFIG::ADCINPUTS_SIZE];// raw ADC values
    uint8_t rotaryPositions[CONFIG::ADCINPUTS_SIZE];// calculated positions

    // last button states
    bool buttonPositions[CONFIG::INPUTS_SIZE];

    // callbacks for bound input functions
    delegate rotaryOnChange[CONFIG::ADCINPUTS_SIZE];
    delegate buttonOnChange[CONFIG::INPUTS_SIZE];


    Input() :
        Subsystem(CONFIG::INPUTINTERVAL),
        rotaryADCValues{},
        rotaryPositions{},
        buttonPositions{},
        rotaryOnChange{},
        buttonOnChange{}
    {}

    // TODO add list of ADC channels to read and corresponding input channel

    virtual void Update(uint8_t) override;// start ADC read
    virtual void Init() override;

    // gets position from rawADC value
    uint8_t positionFromADCResult(volatile uint16_t& result);

    // gets for ADC result array based on channel
    uint8_t getIndexOfChannel(uint8_t channel);
};


#endif /* DASHBOARD2_INPUT_H_ */
