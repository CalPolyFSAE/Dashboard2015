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

    struct BUTTONINFO
    {
        volatile uint8_t* addr;
        uint8_t mask;
    };

    // input channels
    // active low
    // Note, these ports have to be configured in the pinConfig function in main.cpp
    static constexpr BUTTONINFO BUTTON0 = {
            &PINC,
            _BV(PC0)
    };

    static constexpr BUTTONINFO BUTTON1 = {
            &PINC,
            _BV(PC1)
    };

    static constexpr BUTTONINFO INPUTS[] =
    {
        BUTTON0,
        BUTTON1
    };
    static constexpr uint8_t INPUTS_SIZE = 2;

    enum class BUTTON : uint8_t
    {
        BTN0 = 0,
        BTN1 = 1
    };


    // rotary switch configs
    static constexpr uint8_t ROT0_ADC = 0;//channel 0 right
    static constexpr uint8_t ROT1_ADC = 1;//channel 1 left
    static constexpr uint8_t ROT2_ADC = 2;//channel 2 middle

    // this also dictates the index to be used with Input::getRotaryPos function
    static constexpr uint8_t ACDINPUTS[] =
    {
        ROT0_ADC,
        ROT1_ADC,
        ROT2_ADC
    };
    static constexpr uint8_t ADCINPUTS_SIZE = 3;

    // positions in array of different inputs
    enum class ROTARY : uint8_t
    {
        ROT0 = 0,// right
        ROT1 = 1,// left
        ROT2 = 2// middle
    };

    static constexpr uint8_t ROTARYPOSITIONS = 12;

    // attach callback to button
    int8_t BindOnChangeButton(const delegate&, BUTTON);

    // attach callback to rotary
    int8_t BindOnChangeRotary(const delegate&, ROTARY);

    virtual void INT_Call_ADC_Finished(const uint16_t& value, uint8_t channel) override;// start next ADC read on complete

    // position get functions
    inline bool getButtonPos( BUTTON button ) {
        uint8_t buti = (uint8_t)button;
        if (buti < INPUTS_SIZE)
            return buttonPositions[buti];
        return false;
    }

    inline uint8_t getRotaryPos(ROTARY rot)
    {
        uint8_t roti = (uint8_t)rot;
        if (roti < ADCINPUTS_SIZE)
            return rotaryPositions[roti];
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

    volatile bool bDidCompleteRead = false;


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
