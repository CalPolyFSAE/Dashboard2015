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
class Input : public Subsystem, ADCManagerCallbackInterface
{
public:
    friend class Subsystem;// this is necessary for StaticClass function

    // input channels
    // active low
    // Note, these ports have to be configured in the pinConfig function in main.cpp
    static constexpr uint16_t BUTTON0 = BIT(PINC, PC0);// specify which bits to read for input
    static constexpr uint16_t BUTTON1 = BIT(PINC, PC1);
    static constexpr uint16_t INPUTS[] =
    {
        BUTTON0,
        BUTTON1
    };
    static constexpr uint8_t INPUTS_SIZE = 2;


    // rotary switch configs
    static constexpr uint8_t RED_ADC = 1;
    static constexpr uint8_t YELLOW_ADC = 0;
    static constexpr uint8_t BLACK_ADC = 2;

    // this also dictates the indices to be used with Input::getRotaryPos function
    static constexpr uint8_t ACDINPUTS[] =
    {
        RED_ADC,
        YELLOW_ADC,
        BLACK_ADC
    };
    static constexpr uint8_t ADCINPUTS_SIZE = 3;

    // positions in array of different inputs
    enum class ROTARY : uint8_t
    {
        RED = 0,
        YELLOW = 1,
        BLACK = 2
    };
    enum class BUTTON : uint8_t
    {
        BTNL = 0,
        BTNR = 1
    };

    static constexpr uint8_t ROTARYPOSITIONS = 12;

    // attach callback to button
    int8_t BindOnChangeButton(const delegate&, BUTTON);

    // attach callback to rotary
    int8_t BindOnChangeRotary(const delegate&, ROTARY);

    virtual void INT_Call_ADC_Finished(const uint16_t& value, uint8_t channel) override;// start next ADC read on complete

    // position get functions
    inline bool getButtonPos( uint8_t index ) {
        if (index < INPUTS_SIZE)
            return buttonPositions[index];
        return false;
    }

    inline uint8_t getRotaryPos(uint8_t index)
    {
        if (index < ADCINPUTS_SIZE)
            return rotaryPositions[index];
        return 0;
    }

protected:

    // ADC states
    volatile uint16_t rotaryADCValues[ADCINPUTS_SIZE];// raw ADC values
    uint8_t rotaryPositions[ADCINPUTS_SIZE];// calculated positions

    // last button states
    bool buttonPositions[INPUTS_SIZE];

    // callbacks for bound input functions
    delegate rotaryOnChange[ADCINPUTS_SIZE];
    delegate buttonOnChange[INPUTS_SIZE];



    Input() :
        Subsystem(CONFIG::INPUTINTERVAL),
        rotaryADCValues{},
        rotaryPositions{},
        buttonPositions{},
        rotaryOnChange{},
        buttonOnChange{}
    {}

    // TODO add list of ADC channels to read and corresponding input channel

    void Update(uint8_t);// start ADC read
    virtual void Init() override;

    // gets position from rawADC value
    uint8_t positionFromADCResult(volatile uint16_t& result);

    // gets for ADC result array based on channel
    uint8_t getIndexOfChannel(uint8_t channel);
};


#endif /* DASHBOARD2_INPUT_H_ */
