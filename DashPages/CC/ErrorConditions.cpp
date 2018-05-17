/*
 * ErrorConditions.cpp
 *
 *  Created on: May 16, 2018
 *      Author: root
 */

#include "ErrorConditions.h"
#include "../../Subsystem.h"

ErrorConditions::ErrorConditions(const CCDashConfig::ConvertedInfo& info):
    DashInfo(info),
    monitors{},
    currentErrorIndex(0)
{
    SubsystemControl::StaticClass ().RegisterEvent (
            delegate::from_method<ErrorConditions, &ErrorConditions::Refresh> (
                    this),
            CCDashConfig::ERROR_TICKS_PERIOD_MS);
}


void ErrorConditions::Refresh(uint8_t)
{
    //constexpr uint8_t EngineTempErrThresholdH = 212;
    //constexpr uint8_t EngineTempWarnThresholdH = 205;

    //constexpr float BatteryErrThresholdL = 11.8;
    //constexpr uint8_t BatteryWarnThresholdL = 12;

    //constexpr uint8_t OilPressureErrThresholdL = 5;
    //constexpr uint8_t OilPressureWarnThresholdL = 15;

    //constexpr uint8_t OilTempErrThresholdH = 250;
    //constexpr uint8_t OilTempWarnThresholdH = 210;

    for(uint8_t i = 0; i < Error::NUM_ERROR_TYPES; ++i)
    {
        ErrorMonitor& current = monitors[i];
        if(current.bHasBeenAck && current.bIsSet)
        {
            current.ticksSinceAck++;
            if(current.ticksSinceAck >= CCDashConfig::ERROR_TICKS_BEFORE_ACK_ERROR_RESET)
            {
                current.bHasBeenAck = false;
                current.ticksSinceAck = 0;
            }
        }
    }

    // check for water overheat
    if(DashInfo.EngineTemp > CCDashConfig::EngineTempErrThresholdH){
        monitors[(uint8_t)Error::ErrorType::EngineOverheat].bIsSet = true;
    }else if(DashInfo.EngineTemp > CCDashConfig::BatteryWarnThresholdL){
        monitors[(uint8_t)Error::ErrorType::HighEngineTemp].bIsSet = true;
    }else{
        monitors[(uint8_t)Error::ErrorType::HighEngineTemp].bIsSet = false;
    }

    // check for oil overheat
    if(DashInfo.OilTemp > CCDashConfig::OilTempErrThresholdH){
        monitors[(uint8_t)Error::ErrorType::EngineOverheat].bIsSet = true;
    }else if(DashInfo.EngineTemp > CCDashConfig::BatteryWarnThresholdL){
        monitors[(uint8_t)Error::ErrorType::HighOilTemp].bIsSet = true;
    }else{
        monitors[(uint8_t)Error::ErrorType::HighOilTemp].bIsSet = false;
    }

    // if both oil and water have cooled enough, reset
    if (!monitors[(uint8_t)Error::ErrorType::HighEngineTemp].bIsSet
            && !monitors[(uint8_t)Error::ErrorType::HighOilTemp].bIsSet)
    {
        monitors[(uint8_t)Error::ErrorType::EngineOverheat].bIsSet = false;
    }

    // check battery voltage
    if(DashInfo.BatteryVoltage < CCDashConfig::BatteryErrThresholdL){
        monitors[(uint8_t)Error::ErrorType::LowBatteryVoltage].bIsSet = true;
    }else{
        monitors[(uint8_t)Error::ErrorType::LowBatteryVoltage].bIsSet = false;
    }

    // check oil pressure
    if(DashInfo.OilPressure < CCDashConfig::OilPressureErrThresholdL){
        monitors[(uint8_t)Error::ErrorType::LowOilPressure].bIsSet = true;
    }else{
        monitors[(uint8_t)Error::ErrorType::LowOilPressure].bIsSet = false;
    }

}

Error ErrorConditions::GetPriorityError()
{
    return Error();
}

Error ErrorConditions::CycleError()
{
    return Error();
}

Error ErrorConditions::GetCurrentError()
{
    return Error();
}


void ErrorConditions::SetError(Error e)
{
    uint8_t index = (uint8_t)(e.GetErrorType());
    monitors[index].bIsSet = true;
}


