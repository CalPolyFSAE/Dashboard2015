/*
 * CCDashStr.cpp
 *
 *  Created on: May 13, 2018
 *      Author: root
 */

#include "CCDashStr.h"
#include "avr/pgmspace.h"

static const char PROGMEM RotaryRed1[] = "LC - Default";
static const char PROGMEM RotaryRed2[] = "LC - -100rpm";
static const char PROGMEM RotaryRed3[] = "LC - +100rpm";
static const char PROGMEM RotaryRed4[] = "LC - -250rpm";
static const char PROGMEM RotaryRed5[] = "LC - +250rpm";
static const char PROGMEM RotaryRed6[] = "Invalid";
static const char PROGMEM RotaryRed7[] = "Invalid";
static const char PROGMEM RotaryRed8[] = "Invalid";
static const char PROGMEM RotaryRedUnused[] = "Invalid";

PGM_P const RotaryRedStringTable[] PROGMEM =
{
        RotaryRed1, RotaryRed2, RotaryRed3, RotaryRed4, RotaryRed5, RotaryRed6, RotaryRed7, RotaryRed8, RotaryRedUnused, RotaryRedUnused, RotaryRedUnused, RotaryRedUnused
};

static const char PROGMEM RotaryYellow1[] = "Brightness: 25%";
static const char PROGMEM RotaryYellow2[] = "Brightness: 50%";
static const char PROGMEM RotaryYellow3[] = "Brightness: 75%";
static const char PROGMEM RotaryYellow4[] = "Brightness: 100%";
static const char PROGMEM RotaryYellow5[] = "Invalid";
static const char PROGMEM RotaryYellow6[] = "Invalid";
static const char PROGMEM RotaryYellow7[] = "Invalid";
static const char PROGMEM RotaryYellow8[] = "Invalid";
static const char PROGMEM RotaryYellowUnused[] = "Invalid";

PGM_P const RotaryYellowStringTable[] PROGMEM = {RotaryYellow1, RotaryYellow2,
        RotaryYellow3, RotaryYellow4, RotaryYellow5, RotaryYellow6, RotaryYellow7,
        RotaryYellow8, RotaryYellowUnused, RotaryYellowUnused, RotaryYellowUnused,
        RotaryYellowUnused};

static const char PROGMEM RotaryBlack1[] = "Page - Auto";
static const char PROGMEM RotaryBlack2[] = "Page - Driving";
static const char PROGMEM RotaryBlack3[] = "Page - Sensors";
static const char PROGMEM RotaryBlack4[] = "Invalid";
static const char PROGMEM RotaryBlack5[] = "Invalid";
static const char PROGMEM RotaryBlack6[] = "Invalid";
static const char PROGMEM RotaryBlack7[] = "Invalid";
static const char PROGMEM RotaryBlack8[] = "Invalid";
static const char PROGMEM RotaryBlackUnused[] = "Invalid";

PGM_P const RotaryBlackStringTable[] PROGMEM =
{
        RotaryBlack1, RotaryBlack2, RotaryBlack3, RotaryBlack4, RotaryBlack5, RotaryBlack6, RotaryBlack7, RotaryBlack8, RotaryBlackUnused, RotaryBlackUnused,RotaryBlackUnused,RotaryBlackUnused
};

static const size_t CCDashStr::STATE_MAX_DESC_LENGTH = 30;

void CCDashStr::getDialStr(ROTARY rot, uint8_t pos, char cpyout[])
{
    if(sizeof(cpyout) < STATE_MAX_DESC_LENGTH)
    {
        return;
    }
    const char* PROGMEM str = nullptr;

    switch(rot)
    {
        case ROTARY::LEFT:
            str = RotaryBlackStringTable[pos];
            break;
        case ROTARY::CENTER:
            str = RotaryRedStringTable[pos];
            break;
        case ROTARY::RIGHT:
            str = RotaryYellowStringTable[pos];
            break;
    }
    strncpy_P(cpyout, str, STATE_MAX_DESC_LENGTH);
}


static const char PROGMEM WarningMessage_EngineTemperatureError[] = "Engine Temperature Error";
static const char PROGMEM WarningMessage_EngineTemperatureHigh[] = "Engine Temp High Warn";
static const char PROGMEM WarningMessage_OilTemperatureHigh[] = "Oil Temp High Warn";
static const char PROGMEM WarningMessage_Invalid[] = "Invalid";
static const char PROGMEM WarningMessage_InvalidData[] = "Invalid Data";
static const char PROGMEM WarningMessage_BatteryLow[] = "Battery Voltage Low";
static const char PROGMEM WarningMessage_OilPressure[] = "Oil Pressure Low";

static const size_t CCDashStr::ERROR_MAX_MSG_LENGTH = 25;


void CCDashStr::getErrorTypeStr(Error e, char out[])
{
    const char* PROGMEM str = nullptr;
    switch(e.GetErrorType())
    {
        case Error::ErrorType::EngineOverheat:
            str = WarningMessage_EngineTemperatureError;
            break;
        case Error::ErrorType::HighEngineTemp:
            str = WarningMessage_EngineTemperatureHigh;
            break;
        case Error::ErrorType::HighOilTemp:
            str = WarningMessage_OilTemperatureHigh;
            break;
        case Error::ErrorType::Invalid:
            str = WarningMessage_Invalid;
            break;
        case Error::ErrorType::InvalidValue:
            str = WarningMessage_InvalidData;
            break;
        case Error::ErrorType::LowBatteryVoltage:
            str = WarningMessage_BatteryLow;
            break;
        case Error::ErrorType::LowOilPressure:
            str = WarningMessage_OilPressure;
            break;
    }
    strncpy_P(out, str, ERROR_MAX_MSG_LENGTH);
}

static const char PROGMEM ErrorLevel_Danger[] = "DANGER";
static const char PROGMEM ErrorLevel_Error[] = "ERROR";
static const char PROGMEM ErrorLevel_Message[] = "Message";
static const char PROGMEM ErrorLevel_None[] = "Invalid";
static const char PROGMEM ErrorLevel_ReturnToPits[] = "RETURN";
static const char PROGMEM ErrorLevel_Warning[] = "Warning";

static const size_t CCDashStr::ERROR_MAX_TYPE_LENGTH = 15;

void CCDashStr::getErrorMsgStr(Error e, char out[])
{
    const char* PROGMEM str = nullptr;
    switch(e.GetErrorLevel())
    {
        case Error::ErrorLevel::Danger:
            str = ErrorLevel_Danger;
            break;
        case Error::ErrorLevel::Error:
            str = ErrorLevel_Error;
            break;
        case Error::ErrorLevel::Message:
            str = ErrorLevel_Message;
            break;
        case Error::ErrorLevel::None:
            str = ErrorLevel_None;
            break;
        case Error::ErrorLevel::ReturnToPits:
            str = ErrorLevel_ReturnToPits;
            break;
        case Error::ErrorLevel::Warning:
            str = ErrorLevel_Warning;
            break;
    }
    strncpy_P(out, str, ERROR_MAX_MSG_LENGTH);
}


