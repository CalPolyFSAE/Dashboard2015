/*
 * Input.cpp
 *
 *  Created on: Apr 3, 2018
 *      Author: root
 */

#include "Input.h"
#include "avr/interrupt.h"

void Input::INT_Call_ADC_Finished(const uint16_t& value, uint8_t channel)
{
    // MUST start the ADC on first channel in array
    static uint8_t indexLastChannel = 0;

    rotaryADCValues[indexLastChannel] = value;

    ++indexLastChannel;

    // figure out if we need to go back to the beginning
    if(!(indexLastChannel >= CONFIG::ADCINPUTS_SIZE))
    {
        // start read for next channel
        ADCManager::StaticClass ().StartRead (
                this, CONFIG::ACDINPUTS[indexLastChannel]);
    }else
    {
        // reset counter
        indexLastChannel = 0;
    }
}

void Input::Update(uint8_t)
{
    Serial.println("STATE CHANGE");
    // update positions while checking for change since last update
    for(uint8_t i = 0; i < CONFIG::ADCINPUTS_SIZE; ++i)
    {
        uint8_t newPos = positionFromADCResult(rotaryADCValues[i]);
        if(rotaryPositions[i] != newPos)
        {
            if(delegate::isValid(rotaryOnChange[i]))
            {
                rotaryOnChange[i](newPos);// report change along with new position
            }
            rotaryPositions[i] = newPos;// update the rotary positions array
        }
    }

    // TODO buttons array onChange
    for(uint8_t i = 0; i < CONFIG::INPUTS_SIZE; ++i)
    {
        uint16_t val = CONFIG::INPUTS[i];
        // Ex: val looks like this
        // port* pinMask appended in an uint16
        // 0000 0002  0000 0001
        // ((uint8_t*)(&val))[0] is 0000 0001, pinMask
        // ((uint8_t*)(&val))[1] is 0000 0002, port*
        uint8_t* port = (((uint8_t*)(&val))[1]);
        uint8_t mask = (((uint8_t*)(&val))[0]);
        uint8_t reg = *port;

        int a = PORTA;

        bool state = reg & mask;

        // check for button change
        if(state != buttonPositions[i])
        {
            if(delegate::isValid(buttonOnChange[i]))
            {
                buttonOnChange[i](state);
            }
        }
    }

    // start next read cycle
    ADCManager::StaticClass().StartRead(this, CONFIG::ACDINPUTS[0]);
}

void Input::Init()
{
    Subsystem<Input>::Init();// important

    Serial.println(FSTR("INPUT: "));
    Serial.print(CONFIG::ADCINPUTS_SIZE);
    Serial.print(FSTR(" ADCINPUTS, "));
    Serial.print (CONFIG::ROTARYPOSITIONS);
    Serial.println (FSTR(" Positions"));
    Serial.print (CONFIG::INPUTS_SIZE);
    Serial.println (FSTR(" BUTTONS"));

    // TODO testing
    /*for(uint8_t i = 0; i < CONFIG::INPUTS_SIZE; ++i)
    {
        uint16_t v = CONFIG::INPUTS[i];
        Serial.println (v);
        Serial.println ((((uint8_t*) (&v))[0]));
        Serial.println ((((uint8_t*) (&v))[1]));
    }*/
    ///////////////

    while(!ADCManager::StaticClass().ADCAvailable())
        ;// wait for ADCManager to be available
    ADCManager::StaticClass().StartRead(this, CONFIG::ACDINPUTS[0]);
}

uint8_t Input::positionFromADCResult(volatile uint16_t& result)
{
    uint16_t cpy;
    ATOMIC_BLOCK(ATOMIC_FORCEON)
    {
        cpy = result;
    }

    // from CPFERotarySwitch.h
    uint8_t position = (uint8_t) (CONFIG::ROTARYPOSITIONS
                    - (float) cpy / (1024.0 / (float) CONFIG::ROTARYPOSITIONS));
    if(position >= CONFIG::ROTARYPOSITIONS)// clamp
        position = CONFIG::ROTARYPOSITIONS - 1;

    return position;
}

uint8_t Input::getIndexOfChannel(uint8_t channel)
{
    for(uint8_t i = 0; i < CONFIG::ADCINPUTS_SIZE; ++i)
    {
        if(channel == CONFIG::ACDINPUTS[i])
            return i;
    }
    return CONFIG::ADCINPUTS_SIZE;
}
