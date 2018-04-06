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
    if(!indexLastChannel >= CONFIG::ADCINPUTS_SIZE)
    {
        // start read for next channel
        ADCManager::StaticClass ().StartRead (
                this, CONFIG::ACDINPUTS[indexLastChannel]);
    }
        indexLastChannel = 0;
}

void Input::Update()
{
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
}

void Input::Init()
{
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
                    - (float) result / (1024.0 / (float) CONFIG::ROTARYPOSITIONS));
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
