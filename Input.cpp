/*
 * Input.cpp
 *
 *  Created on: Apr 3, 2018
 *      Author: root
 */

#include "Input.h"
#include "avr/interrupt.h"

constexpr Input::BUTTONINFO Input::BUTTON0;
constexpr Input::BUTTONINFO Input::BUTTON1;
constexpr Input::BUTTONINFO Input::INPUTS[];
constexpr uint8_t Input::INPUTS_SIZE;

constexpr uint8_t Input::ROT0_ADC;
constexpr uint8_t Input::ROT1_ADC;
constexpr uint8_t Input::ROT2_ADC;

constexpr uint8_t Input::ACDINPUTS[];
constexpr uint8_t Input::ADCINPUTS_SIZE;

constexpr uint8_t Input::ROTARYPOSITIONS;

// attach callback to button
int8_t Input::BindOnChangeButton( const delegate& func, BUTTON btn )
{
    uint8_t pos = (uint8_t) btn;
    if (!delegate::isValid (buttonOnChange[pos]))
    {
        buttonOnChange[pos] = func;
        return 0;
    }
    else
    {
        return -1;
    }
}

// attach callback to rotary
int8_t Input::BindOnChangeRotary( const delegate& func, ROTARY rot )
{
    uint8_t pos = (uint8_t) rot;
    if (!delegate::isValid (rotaryOnChange[pos]))
    {
        rotaryOnChange[pos] = func;
        return 0;
    }
    else
    {
        return -1;
    }
}

void Input::INT_Call_ADC_Finished(const uint16_t& value, uint8_t channel)
{
    bDidCompleteRead = true;
    rotaryADCValues[channel] = value;
}

void Input::Update(uint8_t)
{
    Subsystem::Update(0);

    for(uint8_t indexLastChannel = 0; indexLastChannel < ADCINPUTS_SIZE; ++indexLastChannel)
    {
        bDidCompleteRead = false;
        // start read for next channel
        Subsystem::StaticClass<ADCManager> ().StartRead (
                this, ACDINPUTS[indexLastChannel]);
        // wait for complete
        while(!bDidCompleteRead);
    }


    // update positions while checking for change since last update
    for(uint8_t i = 0; i < ADCINPUTS_SIZE; ++i)
    {
        uint8_t newPos = positionFromADCResult(rotaryADCValues[i]);
        if(rotaryPositions[i] != newPos)
        {
            if(delegate::isValid(rotaryOnChange[i]))
            {
                //Serial.println("FIRING");
                rotaryOnChange[i](newPos);// report change along with new position
            }
            rotaryPositions[i] = newPos;// update the rotary positions array

#ifdef DEBUG_PRINT
            Serial.print(FSTR("[INFO]: "));
            Serial.print(__FILE__);
            Serial.print(FSTR(" at "));
            Serial.print(__LINE__);
            Serial.print(FSTR(": Rotary Position Change, newPos: "));
            Serial.println(newPos);
#endif
        }
    }

    // check for button changes
    for(uint8_t i = 0; i < INPUTS_SIZE; ++i)
    {
        volatile uint8_t* port = INPUTS[i].addr;
        uint8_t mask = INPUTS[i].mask;

        // check for bit at pos
        bool state = *port & mask;

        // check for button change
        if(state != buttonPositions[i])
        {
            if(delegate::isValid(buttonOnChange[i]))
            {
                buttonOnChange[i](state);
            }
            buttonPositions[i] = state;
#ifdef DEBUG_PRINT
            Serial.print(FSTR("[INFO]: "));
            Serial.print(__FILE__);
            Serial.print(FSTR(" at "));
            Serial.print(__LINE__);
            Serial.print(FSTR(": Button Position Change, state: "));
            Serial.println(state);
#endif
        }
    }
}

void Input::Init()
{
    Subsystem::Init();// important

#ifdef DEBUG_PRINT
    Serial.print(FSTR("[INFO]: "));
    Serial.print(__FILE__);
    Serial.print(FSTR(" at "));
    Serial.print(__LINE__);
    Serial.print(FSTR(" INPUT: "));
    Serial.print(ADCINPUTS_SIZE);
    Serial.print(FSTR(" ADCINPUTS, "));
    Serial.print(ROTARYPOSITIONS);
    Serial.print(FSTR(" Positions, "));
    Serial.print(INPUTS_SIZE);
    Serial.println(FSTR(" BUTTONS"));
#endif //DEBUG_PRINT

    // TODO testing
    /*for(uint8_t i = 0; i < CONFIG::INPUTS_SIZE; ++i)
    {
        uint16_t v = CONFIG::INPUTS[i];
        Serial.println (v);
        Serial.println ((((uint8_t*) (&v))[0]));
        Serial.println ((((uint8_t*) (&v))[1]));
    }*/
    ///////////////

    while(!Subsystem::StaticClass<ADCManager>().ADCAvailable())
        ;// wait for ADCManager to be available
}

uint8_t Input::positionFromADCResult(volatile uint16_t& result)
{
    uint16_t cpy;
    ATOMIC_BLOCK(ATOMIC_FORCEON)
    {
        cpy = result;
    }

    // from CPFERotarySwitch.h
    uint8_t position = (uint8_t) (ROTARYPOSITIONS
                    - (float) cpy / (1024.0 / (float) ROTARYPOSITIONS));
    if(position >= ROTARYPOSITIONS)// clamp
        position = ROTARYPOSITIONS - 1;

    return position;
}

uint8_t Input::getIndexOfChannel(uint8_t channel)
{
    for(uint8_t i = 0; i < ADCINPUTS_SIZE; ++i)
    {
        if(channel == ACDINPUTS[i])
            return i;
    }
    return ADCINPUTS_SIZE;
}
