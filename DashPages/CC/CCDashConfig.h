/*
 * CCDashConfig.h
 *
 *  Created on: May 12, 2018
 *      Author: root
 */

#ifndef DASHPAGES_CC_CCDASHCONFIG_H_
#define DASHPAGES_CC_CCDASHCONFIG_H_

#include "../../CANLib.h"

namespace CCDashConfig
{
    // CAN messages for incoming data
    constexpr CANRaw::CAN_FRAME_HEADER DashCANFrame0 =
    {
        0xF0,             // id
        0,                // rtr
        0,                // ide
        8                 // dlc
    };

    constexpr CANRaw::CAN_FRAME_HEADER DashCANFrame1 =
    {
        0xF2,             // id
        0,                // rtr
        0,                // ide
        8                 // dlc
    };

    constexpr CANRaw::CAN_FRAME_HEADER DashCANFrame2 =
    {
        0xF3,             // id
        0,                // rtr
        0,                // ide
        8                 // dlc
    };

    // used as info for TX message containing INPUT data
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

    struct DashCAN0
    { // 0x0E0
        uint16_t EngineTemp;
        uint16_t RPM;
        uint16_t Gear;
        uint16_t Speed;
    };

    struct DashCAN1
    { // 0x0E1
        uint16_t Lambda;
        uint16_t OilTemp;
        uint16_t MAP;
        uint16_t ThrottlePOS;
    };

    struct DashCAN2
    { // 0x0E2
        uint16_t BatteryVoltage;
        uint16_t OilPressure;
        uint16_t IAT;
        uint8_t Brightness;
        uint8_t unused;
    };

    struct ConvertedInfo
    {
        float EngineTemp;
        float RPM;
        float Gear;
        float Speed;
        float Lambda;
        float OilTemp;
        float MAP;
        float ThrottlePOS;
        float BatteryVoltage;
        float OilPressure;
        float IAT;
        float Brightness;
    };

    constexpr uint8_t NUM_CAN_MESSAGES = 3;

    struct DashInfo
    {
        union
        {
            struct
            {
                DashCAN0 c0;
                DashCAN1 c1;
                DashCAN2 c2;
            };
            struct
            {
                CANRaw::CAN_DATA msg[NUM_CAN_MESSAGES];
            };
        };
    };

    constexpr CANRaw::CAN_FRAME_HEADER DashInfoLayout[] =
    {
        DashCANFrame0,
        DashCANFrame1,
        DashCANFrame2
    };

    // Errors
    constexpr uint8_t ERROR_ARRAY_SIZE = 10;
    constexpr uint8_t ERROR_TICKS_PERIOD_MS = 1000;
    constexpr uint8_t ERROR_TICKS_BEFORE_ACK_ERROR_RESET = 100;

    constexpr uint8_t EngineTempColdThresholdL = 100;
    constexpr uint8_t EngineTempErrThresholdH = 212;
    constexpr uint8_t EngineTempWarnThresholdH = 205;

    constexpr float BatteryErrThresholdL = 11.8;
    constexpr uint8_t BatteryWarnThresholdL = 12;

    constexpr uint8_t OilPressureErrThresholdL = 5;
    constexpr uint8_t OilPressureWarnThresholdL = 15;

    constexpr uint8_t OilTempColdThresholdL = 100;
    constexpr uint8_t OilTempErrThresholdH = 250;
    constexpr uint8_t OilTempWarnThresholdH = 210;

}


#endif /* DASHPAGES_CC_CCDASHCONFIG_H_ */
