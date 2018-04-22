/*
 * DashTypes.h
 *
 *  Created on: Apr 21, 2018
 *      Author: root
 */

#ifndef DASHPAGES_DASHTYPES_H_
#define DASHPAGES_DASHTYPES_H_

#include "stdint.h"
#include "../CANLib.h"

namespace DashTypes
{
    constexpr uint8_t DashDataPackLayoutSize = 4;

    struct CANDataPack
    {
        CANRaw::CAN_DATA data[DashDataPackLayoutSize];
    };

    struct DashDataPack
    {
        uint8_t data0[8];
        uint8_t data1[8];
        uint8_t data2[8];
        uint8_t data3[8];
    };

    union DashData
    {
        DashDataPack Data;
        CANDataPack Msg;
    };
}


#endif /* DASHPAGES_DASHTYPES_H_ */
