/*
 * FEDashStr.h
 *
 *  Created on: Apr 15, 2018
 *      Author: root
 */

#ifndef DASHPAGES_FEDASHSTR_H_
#define DASHPAGES_FEDASHSTR_H_

#include "stdint.h"
#include <avr/pgmspace.h>

class FEDashStr
{
public:

    static constexpr uint8_t BMS_CHARGING_ERROR_MAX_LENGTH = 50;
    static constexpr uint8_t BMS_CHARGING_STATE_MAX_LENGTH = 15;
    static constexpr uint8_t STATE_MAX_DESC_LENGTH = 30;

    struct BMS_CHARGING_ERROR
    {
        const char BMS_CHARGING_ERROR_STR[BMS_CHARGING_ERROR_MAX_LENGTH];
    };

    struct BMS_CHARGING_STATE
    {
        const char BMS_CHARGING_STATE_STR[BMS_CHARGING_STATE_MAX_LENGTH];
    };

    struct STATE
    {
        const char STATE_STR[STATE_MAX_DESC_LENGTH];
    };

    enum class MCControlState
        : uint8_t
        {
            SC_Initial,
        SC_History,
        SC_ShallowHistory,
        SC_Terminal,
        BroadcastDiag,
        CurrentDrop,
        Enabled,
        Disabled
    };

    enum class ShutdownState
        : uint8_t
        {
            SC_Initial,
        SC_History,
        SC_ShallowHistory,
        SC_Terminal,
        AIR_Coolant_On,
        AIR_On,
        Precharge,
        TSMS_Off
    };

    enum class WarningSeverity
        : uint8_t
        {
            Okay, ShortWarning, LongWarning, ReturnToPits, Error, Danger
    };

    enum class eBMSChargingState
        : uint8_t
        {
            Disconnected,
        PreCharging,
        MainCharging,
        Balancing,
        ChargeFinishedOK,
        ChargingError
    };

    enum class eBMSLastChargeError
        : uint8_t
        {
            NoError,
        NoCellCommunication,
        NoCellCommunicationNoCAN,
        ChargeDurationExceeded,
        CellCommunicationLostDuringCharge,
        CannotSetBalancingVoltage,
        CellTempsTooHigh,
        NoCellCommsSoNoTemp,
        CellNumberMismatch,
        CellOvervoltage,
        CellProtectionEvent
    };

    enum class WarningMessage
        : uint8_t
        {
            ControllerTemperature,
        MotorTemperature,
        BatteryTemperature,
        BatteryLowVoltage,
        LVBattery,
        sbRIOTemperature,
        Precharge,
        BSPD,
        IMD,
        RemoteReturnToPits,
        EStopInterrupted,
        NoCellComms,
        MCOverSpeed,
        MCOverCurrent,
        MCOverVoltage,
        MCOverTemp,
        MCDirectionCommand,
        MCInverterResponseTimeout,
        MCDesatFault,
        MCHardwareOverCurrentFault,
        MCUnderVoltage,
        MCCommandMessageLost,
        MCMotorOverTemp,
        MCModAOverTemp,
        MCModBOverTem,
        MCModCOverTemp,
        MCPCBOverTemp,
        MCGateDrv1OverTemp,
        MCGateDrv2OverTemp,
        MCDateDrv3OverTemp,
        MCCurrentSensorFault,
        MCResolverNotConnected,
        ShutdownLatchTripped,
        UnknownBMS,
        RemoteEmergency
    };

    FEDashStr();

    const char * PROGMEM warningMessageToString(WarningMessage warning);
};


#endif /* DASHPAGES_FEDASHSTR_H_ */
