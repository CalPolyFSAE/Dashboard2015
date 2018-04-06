/*
 * Dashboard2.h
 *
 *  Created on: Jan 22, 2018
 *      Author: root
 */

#ifndef DASHBOARD2_DASHBOARD2_H_
#define DASHBOARD2_DASHBOARD2_H_

#include "stdint.h"

//TODO: config stuff
namespace CONFIG
{
    // max number of jobs the Timing system can have
    constexpr uint8_t RSCMAXJOBS = 15;

    // timing interval for Input (ms)
    constexpr uint8_t INPUTINTERVAL = 10;

    // input channels
    constexpr uint8_t BUTTON0 = 0;
    constexpr uint8_t BUTTON1 = 0;
    constexpr uint8_t BUTTON2 = 0;
    constexpr uint8_t BUTTON3 = 0;
    constexpr uint8_t INPUTS[] =
    {
        BUTTON0,
        BUTTON1,
        BUTTON2,
        BUTTON3
    };
    constexpr uint8_t INPUTS_SIZE = 4;


    // rotary switch configs
    constexpr uint8_t RED_ADC = 1;
    constexpr uint8_t YELLOW_ADC = 0;
    constexpr uint8_t BLACK_ADC = 2;

    constexpr uint8_t ACDINPUTS[] =
    {
        RED_ADC,
        YELLOW_ADC,
        BLACK_ADC
    };
    constexpr uint8_t ADCINPUTS_SIZE = 3;

    constexpr uint8_t ROTARYPOSITIONS = 12;

}


#endif /* DASHBOARD2_DASHBOARD2_H_ */
