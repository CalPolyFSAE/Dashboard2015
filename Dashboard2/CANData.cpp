/*
 * CANData.cpp
 *
 *  Created on: Jan 15, 2018
 *      Author: root
 */

#include "CANData.h"

CANData::CANData()
{
    // setup data buffers
    // are there data buffers?
    // How many CAN Mobs should be saved? (uint8_t data[])
}

// CAN rx interrupt call
void CANData::INT_CANRx()
{
    // need to read data
    bCANNeedRead = true;
}

// startup
void CANData::Init()
{
    // setup CAN Controller
}

void CANData::Update()
{
    //check for data that needs to get copied from CAN MOB
    if(bCANNeedRead)
    {
        //do stuff
        bCANNeedRead = false;
    }
}
