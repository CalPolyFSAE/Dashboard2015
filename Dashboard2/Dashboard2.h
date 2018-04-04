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

    // input channels
    constexpr uint8_t INPUT_BUTTON0 = 0;
    constexpr uint8_t INPUTS[] =
        {
            INPUT_BUTTON0
        };

    constexpr uint8_t ADCINPUT0 = 0;
    constexpr uint8_t ACDINPUTS[] =
        {
            ADCINPUT0
        };
}


#endif /* DASHBOARD2_DASHBOARD2_H_ */
