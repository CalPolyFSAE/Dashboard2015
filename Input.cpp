/*
 * Input.cpp
 *
 *  Created on: Apr 3, 2018
 *      Author: root
 */

#include "Input.h"
#include "avr/interrupt.h"

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
    // MUST start the ADC on first channel in array
    static uint8_t indexLastChannel = 0;

    rotaryADCValues[indexLastChannel] = value;

    ++indexLastChannel;

    // figure out if we need to go back to the beginning
    if(!(indexLastChannel >= ADCINPUTS_SIZE))
    {
        // start read for next channel
        Subsystem::StaticClass<ADCManager> ().StartRead (
                this, ACDINPUTS[indexLastChannel]);
    }else
    {
        // reset counter
        indexLastChannel = 0;
        // do nothing, next read cycle will be started by Update
    }
}

void Input::Update(uint8_t)
{
    Subsystem::Update(0);
    // update positions while checking for change since last update
    for(uint8_t i = 0; i < ADCINPUTS_SIZE; ++i)
    {
        uint8_t newPos = positionFromADCResult(rotaryADCValues[i]);
        if(rotaryPositions[i] != newPos)
        {
            if(delegate::isValid(rotaryOnChange[i]))
            {
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
        uint16_t val = INPUTS[i];
        // Ex: val looks like this
        // port* pinMask appended in an uint16
        // 0000 0002  0000 0001
        // ((uint8_t*)(&val))[0] is 0000 0001, pinMask
        // ((uint8_t*)(&val))[1] is 0000 0002, port*
        volatile uint8_t* port = (volatile uint8_t *)(((uint8_t*)(&val))[1]);
        uint8_t mask = (((uint8_t*)(&val))[0]);
        volatile uint8_t reg = *port;

        // check for bit at pos
        bool state = reg & mask;

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

    // start next read cycle
    Subsystem::StaticClass<ADCManager>().StartRead(this, ACDINPUTS[0]);
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
