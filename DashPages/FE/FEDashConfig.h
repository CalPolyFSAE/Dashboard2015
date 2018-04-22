/*
 * FEDashCAN.h
 *
 *  Created on: Apr 15, 2018
 *      Author: root
 */

#ifndef DASHPAGES_FE_FEDASHCONFIG_H_
#define DASHPAGES_FE_FEDASHCONFIG_H_

namespace FEDashConfig
{
    // CAN messages for incoming data
    constexpr CANRaw::CAN_FRAME_HEADER DashCAN0 =
        {
            0xF0,             // id
            0,                // rtr
            0,                // ide
            8                 // dlc
        };

    constexpr CANRaw::CAN_FRAME_HEADER DashCAN1 =
        {
            0xF2,             // id
            0,                // rtr
            0,                // ide
            8                 // dlc
        };

    constexpr CANRaw::CAN_FRAME_HEADER DashCAN2 =
        {
            0xF3,             // id
            0,                // rtr
            0,                // ide
            8                 // dlc
        };

    constexpr CANRaw::CAN_FRAME_HEADER DashCAN3 =
        {
            0xF4,             // id
            0,                // rtr
            0,                // ide
            8                 // dlc
        };

    constexpr CANRaw::CAN_FRAME_HEADER WarningCAN =
        {
            0xF1,             // id
            0,                // rtr
            0,                // ide
            8                 // dlc
        };


    constexpr CANRaw::CAN_FRAME_HEADER DashCANMsgHeader =
        {
            0x15,             // id
            0,                // rtr
            0,                // ide
            8                 // dlc
        };

    constexpr CANRaw::CAN_FRAME_MASK DashCANMsgMsk =
        {
            0xFFFF,           // id
            true,             // rtr
            true              // ide
        };

    // id mapped to position in DashDataPack
    constexpr uint8_t FEDashDataPackLayout[] =
        {
            DashCAN0.id,
            DashCAN1.id,
            DashCAN2.id,
            WarningCAN.id
        };
}


#endif /* DASHPAGES_FE_FEDASHCONFIG_H_ */
