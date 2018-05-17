/*
 * FEDashStr.cpp
 *
 *  Created on: Apr 15, 2018
 *      Author: root
 */

#include "FEDashStr.h"
#include <avr/pgmspace.h>

static const char PROGMEM WarningMessage_ControllerTemperature[] = "CONTROLLER WARM: %.2fC";
static const char PROGMEM WarningMessage_MotorTemperature[] = "MOTOR WARM: %.2fC";
static const char PROGMEM WarningMessage_BatteryTemperature[] = "BATTERY WARM: %.2fC";
static const char PROGMEM WarningMessage_BatteryLowVoltage[] = "PACK LOW VOLTAGE: %.2fV";
static const char PROGMEM WarningMessage_LVBattery[] = "GLV LOW VOLTAGE: %.2fV";
static const char PROGMEM WarningMessage_sbRIOTemperature[] = "sbRIO WARM: %.0fC";
static const char PROGMEM WarningMessage_Precharge[] = "HV PRECHARGING: %.0fV";
static const char PROGMEM WarningMessage_Invalid[] = "INVALID MSG";
static const char PROGMEM WarningMessage_BSPD[] = "BSPD ERROR";
static const char PROGMEM WarningMessage_IMD[] = "IMD ERROR";
static const char PROGMEM WarningMessage_RemotePit[] = "Remote Request";
static const char PROGMEM WarningMessage_EStop[] = "An E-Stop is Pressed";
static const char PROGMEM WarningMessage_NoCellComms[] = "NO CELL COMMS";
static const char PROGMEM WarningMessage_MCOverSpeed[] = "MC Over Speed";
static const char PROGMEM WarningMessage_MCOverCurrent[] = "MC Over Current";
static const char PROGMEM WarningMessage_MCOverVoltage[] = "MC Over Voltage";
static const char PROGMEM WarningMessage_MCOverTemp[] = "MC Inverter Overtemp";
static const char PROGMEM WarningMessage_MCDirectionCommand[] = "MC Direction Error";
static const char PROGMEM WarningMessage_MCInverterResponseTimeout[] = "MC Inverter Timeout";
static const char PROGMEM WarningMessage_MCDesatFault[] = "MC Desat Fault";
static const char PROGMEM WarningMessage_MCHardwareOverCurrentFault[] = "MC HW Over-Current";
static const char PROGMEM WarningMessage_MCUnderVoltage[] = "MC Under Voltage";
static const char PROGMEM WarningMessage_MCCommandMessageLost[] = "MC Command Message Lost";
static const char PROGMEM WarningMessage_MCMotorOverTemp[] = "Motor Over Temp";
static const char PROGMEM WarningMessage_MCModAOverTemp[] = "Module A Overtemp";
static const char PROGMEM WarningMessage_MCModBOverTemp[] = "Module B Overtemp";
static const char PROGMEM WarningMessage_MCModCOverTemp[] = "Module C Overtemp";
static const char PROGMEM WarningMessage_MCPCBOverTemp[] = "PCB Overtemp";
static const char PROGMEM WarningMessage_MCGateDrv1OverTemp[] = "Gate DRV 1 Overtemp";
static const char PROGMEM WarningMessage_MCGateDrv2OverTemp[] = "Gate DRV 2 Overtemp";
static const char PROGMEM WarningMessage_MCGateDrv3OverTemp[] = "Gate DRV 3 Overtemp";
static const char PROGMEM WarningMessage_MCCurrentSensorFault[] = "MC Curr Sensor Fault";
static const char PROGMEM WarningMessage_MCResolverNotConnected[] = "Resolver Not Connected";
static const char PROGMEM WarningMessage_ShutdownLatchTripped[] = "Shutdown Latch Tripped";
static const char PROGMEM WarningMessage_UnknownBMS[] = "Unknown BMS Error";
static const char PROGMEM WarningMessage_RemoteEmergency[] = "REMOTE EMERGENCY SHUTDOWN";

static const char PROGMEM RotaryRed1[] = "MC Off";
static const char PROGMEM RotaryRed2[] = "120Nm";
static const char PROGMEM RotaryRed3[] = "120Nm";
static const char PROGMEM RotaryRed4[] = "120Nm";
static const char PROGMEM RotaryRed5[] = "120Nm";
static const char PROGMEM RotaryRed6[] = "Invalid";
static const char PROGMEM RotaryRed7[] = "Invalid";
static const char PROGMEM RotaryRed8[] = "Invalid";
static const char PROGMEM RotaryRedUnused[] = "Invalid";

PGM_P const RotaryRedStringTable[] PROGMEM =
{
        RotaryRed1,
        RotaryRed2,
        RotaryRed3,
        RotaryRed4,
        RotaryRed5,
        RotaryRed6,
        RotaryRed7,
        RotaryRed8,
        RotaryRedUnused,
        RotaryRedUnused,
        RotaryRedUnused,
        RotaryRedUnused
};

static const char PROGMEM RotaryYellow1[] = "Invalid";
static const char PROGMEM RotaryYellow2[] = "Invalid";
static const char PROGMEM RotaryYellow3[] = "Invalid";
static const char PROGMEM RotaryYellow4[] = "Invalid";
static const char PROGMEM RotaryYellow5[] = "Invalid";
static const char PROGMEM RotaryYellow6[] = "Invalid";
static const char PROGMEM RotaryYellow7[] = "Invalid";
static const char PROGMEM RotaryYellow8[] = "Invalid";
static const char PROGMEM RotaryYellowUnused[] = "Invalid";

PGM_P const RotaryYellowStringTable[] PROGMEM =
{
        RotaryYellow1,
        RotaryYellow2,
        RotaryYellow3,
        RotaryYellow4,
        RotaryYellow5,
        RotaryYellow6,
        RotaryYellow7,
        RotaryYellow8,
        RotaryYellowUnused,
        RotaryYellowUnused,
        RotaryYellowUnused,
        RotaryYellowUnused
};

static const char PROGMEM RotaryBlack1[] = "Auto";
static const char PROGMEM RotaryBlack2[] = "Charging";
static const char PROGMEM RotaryBlack3[] = "Invalid";
static const char PROGMEM RotaryBlack4[] = "Invalid";
static const char PROGMEM RotaryBlack5[] = "Invalid";
static const char PROGMEM RotaryBlack6[] = "Invalid";
static const char PROGMEM RotaryBlack7[] = "Invalid";
static const char PROGMEM RotaryBlack8[] = "Invalid";
static const char PROGMEM RotaryBlackUnused[] = "Invalid";

PGM_P const RotaryBlackStringTable[] PROGMEM =
{
        RotaryBlack1,
        RotaryBlack2,
        RotaryBlack3,
        RotaryBlack4,
        RotaryBlack5,
        RotaryBlack6,
        RotaryBlack7,
        RotaryBlack8,
        RotaryBlackUnused,
        RotaryBlackUnused,
        RotaryBlackUnused,
        RotaryBlackUnused
};

static const char PROGMEM MCState0[] = "SC.Initial";
static const char PROGMEM MCState1[] = "SC.History";
static const char PROGMEM MCState2[] = "SC.ShallowHistory";
static const char PROGMEM MCState3[] = "SC.Terminal";
static const char PROGMEM MCState4[] = "TransDiag";
static const char PROGMEM MCState5[] = "CurrentDrop";
static const char PROGMEM MCState6[] = "Enabled";
static const char PROGMEM MCState7[] = "Disabled";

PGM_P const MCStateStringTable[] PROGMEM =
{
        MCState0,
        MCState1,
        MCState2,
        MCState3,
        MCState4,
        MCState5,
        MCState6,
        MCState7
};

static const char PROGMEM ShutdownState0[] = "SC.Initial";
static const char PROGMEM ShutdownState1[] = "SC.History";
static const char PROGMEM ShutdownState2[] = "SC.ShallowHistory";
static const char PROGMEM ShutdownState3[] = "AIR's & Coolant On";
static const char PROGMEM ShutdownState4[] = "AIR's On";
static const char PROGMEM ShutdownState5[] = "Precharge";
static const char PROGMEM ShutdownState6[] = "TSMS Off";

PGM_P const ShutdownStateStringTable[] PROGMEM =
{
        ShutdownState0,
        ShutdownState1,
        ShutdownState2,
        ShutdownState3,
        ShutdownState4,
        ShutdownState5,
        ShutdownState6
};

static const char PROGMEM BMSChargingState0[] = "Disconnected";
static const char PROGMEM BMSChargingState1[] = "Pre-Charging";
static const char PROGMEM BMSChargingState2[] = "Main Charge";
static const char PROGMEM BMSChargingState3[] = "Balancing";
static const char PROGMEM BMSChargingState4[] = "Charge Finished";
static const char PROGMEM BMSChargingState5[] = "Charging Error";

PGM_P const BMSChargingStateStringTable[] PROGMEM =
{
        BMSChargingState0,
        BMSChargingState1,
        BMSChargingState2,
        BMSChargingState3,
        BMSChargingState4,
        BMSChargingState5
};

static const char PROGMEM BMSChargingError0[] = "No Error";
static const char PROGMEM BMSChargingError1[] = "No Cell Comms";
static const char PROGMEM BMSChargingError2[] = "No Cell Comms";
static const char PROGMEM BMSChargingError3[] = "Maximum Duration Exceeded";
static const char PROGMEM BMSChargingError4[] = "No Cell Comms";
static const char PROGMEM BMSChargingError5[] = "Cannot Set Balancing";
static const char PROGMEM BMSChargingError6[] = "Cell Temps Too High";
static const char PROGMEM BMSChargingError7[] = "No Cell Comms";
static const char PROGMEM BMSChargingError8[] = "Num Cells Mismatch";
static const char PROGMEM BMSChargingError9[] = "Cells Overvoltage";
static const char PROGMEM BMSChargingError10[] = "Cell Protection Event";

PGM_P const BMSChargingErrorStringTable[] PROGMEM =
{
    BMSChargingError0,
    BMSChargingError1,
    BMSChargingError2,
    BMSChargingError3,
    BMSChargingError4,
    BMSChargingError5,
    BMSChargingError6,
    BMSChargingError7,
    BMSChargingError8,
    BMSChargingError9,
    BMSChargingError10
};



void FEDashStr::WarningMessageToString(WarningMessage warning, STATE& out) {
    const char* PROGMEM str = nullptr;
    switch (warning) {
        case WarningMessage::BatteryLowVoltage:
            str = WarningMessage_BatteryLowVoltage;
            break;
        case WarningMessage::BatteryTemperature:
            str = WarningMessage_BatteryTemperature;
            break;
        case WarningMessage::ControllerTemperature:
            str = WarningMessage_ControllerTemperature;
            break;
        case WarningMessage::LVBattery:
            str = WarningMessage_LVBattery;
            break;
        case WarningMessage::MotorTemperature:
            str = WarningMessage_MotorTemperature;
            break;
        case WarningMessage::Precharge:
            str = WarningMessage_Precharge;
            break;
        case WarningMessage::sbRIOTemperature:
            str = WarningMessage_sbRIOTemperature;
            break;
        case WarningMessage::IMD:
            str = WarningMessage_IMD;
            break;
        case WarningMessage::BSPD:
            str = WarningMessage_BSPD;
            break;
        case WarningMessage::RemoteReturnToPits:
            str = WarningMessage_RemotePit;
            break;
        case WarningMessage::EStopInterrupted:
            str = WarningMessage_EStop;
            break;
        case WarningMessage::NoCellComms:
            str = WarningMessage_NoCellComms;
            break;
        case WarningMessage::MCOverSpeed:
            str = WarningMessage_MCOverSpeed;
            break;
        case WarningMessage::MCOverCurrent:
            str = WarningMessage_MCOverCurrent;
            break;
        case WarningMessage::MCOverVoltage:
            str = WarningMessage_MCOverVoltage;
            break;
        case WarningMessage::MCOverTemp:
            str = WarningMessage_MCOverTemp;
            break;
        case WarningMessage::MCDirectionCommand:
            str = WarningMessage_MCDirectionCommand;
            break;
        case WarningMessage::MCInverterResponseTimeout:
            str = WarningMessage_MCInverterResponseTimeout;
            break;
        case WarningMessage::MCDesatFault:
            str = WarningMessage_MCDesatFault;
            break;
        case WarningMessage::MCHardwareOverCurrentFault:
            str = WarningMessage_MCHardwareOverCurrentFault;
            break;
        case WarningMessage::MCUnderVoltage:
            str = WarningMessage_MCUnderVoltage;
            break;
        case WarningMessage::MCCommandMessageLost:
            str = WarningMessage_MCCommandMessageLost;
            break;
        case WarningMessage::MCMotorOverTemp:
            str = WarningMessage_MCMotorOverTemp;
            break;
        case WarningMessage::MCModAOverTemp:
            str = WarningMessage_MCModAOverTemp;
            break;
        case WarningMessage::MCModBOverTem:
            str = WarningMessage_MCModBOverTemp;
            break;
        case WarningMessage::MCModCOverTemp:
            str = WarningMessage_MCModCOverTemp;
            break;
        case WarningMessage::MCPCBOverTemp:
            str = WarningMessage_MCPCBOverTemp;
            break;
        case WarningMessage::MCGateDrv1OverTemp:
            str = WarningMessage_MCGateDrv1OverTemp;
            break;
        case WarningMessage::MCGateDrv2OverTemp:
            str = WarningMessage_MCGateDrv2OverTemp;
            break;
        case WarningMessage::MCDateDrv3OverTemp:
            str = WarningMessage_MCGateDrv3OverTemp;
            break;
        case WarningMessage::MCCurrentSensorFault:
            str = WarningMessage_MCCurrentSensorFault;
            break;
        case WarningMessage::MCResolverNotConnected:
            str = WarningMessage_MCResolverNotConnected;
            break;
        case WarningMessage::ShutdownLatchTripped:
            str = WarningMessage_ShutdownLatchTripped;
            break;
        case WarningMessage::UnknownBMS:
            str = WarningMessage_UnknownBMS;
            break;
        case WarningMessage::RemoteEmergency:
            str = WarningMessage_RemoteEmergency;
            break;
        default:
            str = WarningMessage_Invalid;
            break;
    }
    strncpy_P(out.STATE_STR, str, STATE_MAX_DESC_LENGTH);
}

void FEDashStr::BmsChargeErrorToString(eBMSLastChargeError error, BMS_CHARGING_ERROR& out)
{
    strncpy_P (
            out.BMS_CHARGING_ERROR_STR,
            (PGM_P) pgm_read_word(
                    &(BMSChargingErrorStringTable[(uint8_t) error])),
            BMS_CHARGING_STATE_MAX_LENGTH);
}

void FEDashStr::BmsChargeStateToString(eBMSChargingState state, BMS_CHARGING_STATE& out)
{
    strncpy_P (
            out.BMS_CHARGING_STATE_STR,
            (PGM_P) pgm_read_word(
                    &(BMSChargingStateStringTable[(uint8_t ) state])),
            BMS_CHARGING_STATE_MAX_LENGTH);
}


