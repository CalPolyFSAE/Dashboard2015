/*
 * Driving.cpp
 *
 *  Created on: May 14, 2018
 *      Author: root
 */

#include "Driving.h"
#include "CCDashStr.h"
#include "AVRLibrary/arduino/Arduino.h"

constexpr char* Driving::GearText[];

Driving::Driving(FT801IMPL_SPI& LCD, const CCDashConfig::ConvertedInfo& info) :
    DashInfo(info),
    LCD(LCD),
    CurrentError()
{
    bDisplayNoCAN = false;
}

void Driving::Begin()
{

}

void Driving::Draw()
{
    Color oilBarColor, waterBarColor, BVBoxColor, BVTextColor, OPBoxColor, OPTextColor;
    // oil temp color
    if (DashInfo.OilTemp > CCDashConfig::OilTempErrThresholdH)
        oilBarColor = RED;
    else if (DashInfo.OilTemp > CCDashConfig::OilTempWarnThresholdH)
        oilBarColor = YELLOW;
    else if(DashInfo.OilTemp < CCDashConfig::OilTempColdThresholdL)
        oilBarColor = BLUE;
    else
        oilBarColor = GREEN;

    // water color
    if (DashInfo.EngineTemp > CCDashConfig::EngineTempErrThresholdH)
        waterBarColor = RED;
    else if (DashInfo.EngineTemp > CCDashConfig::EngineTempWarnThresholdH)
        waterBarColor = YELLOW;
    else if(DashInfo.EngineTemp < CCDashConfig::EngineTempColdThresholdL)
        waterBarColor = BLUE;
    else
        waterBarColor = GREEN;

    // Batt volt color
    if (DashInfo.BatteryVoltage < CCDashConfig::BatteryErrThresholdL){
        BVBoxColor = RED;
        BVTextColor = YELLOW;
    }else if (DashInfo.BatteryVoltage < CCDashConfig::BatteryWarnThresholdL){
        BVBoxColor = YELLOW;
        BVTextColor = BLACK;
    }else{
        BVBoxColor = GREEN;
        BVTextColor = BLACK;
    }

    // oil pressure color
    if (DashInfo.OilPressure < CCDashConfig::OilPressureErrThresholdL){
        OPBoxColor = RED;
        OPTextColor = YELLOW;
    }else if (DashInfo.BatteryVoltage < CCDashConfig::OilPressureWarnThresholdL){
        OPBoxColor = YELLOW;
        OPTextColor = BLACK;
    }else{
        OPBoxColor = GREEN;
        OPTextColor = BLACK;
    }

    char errorMsg[CCDashStr::ERROR_MAX_MSG_LENGTH];
    char errorType[CCDashStr::ERROR_MAX_TYPE_LENGTH];

    CCDashStr::getErrorMsgStr(CurrentError, errorMsg);
    CCDashStr::getErrorTypeStr(CurrentError, errorType);

    const char* gear = GearText[(uint8_t)DashInfo.Gear];

    LCD.DLStart();
    LCD.Clear(0, 0, 0);

    LCD.ColorRGB(255, 255, 255);

    // logo in corner
    LCD.Cmd_Scale(32768, 32768);
    LCD.Cmd_Translate(0, -1048576);
    LCD.Cmd_SetMatrix();
    LCD.Begin(FT_BITMAPS);
    LCD.Vertex2ii(328, 0, 1, 0);
    LCD.End();
    LCD.Cmd_LoadIdentity();
    LCD.Cmd_SetMatrix();

    LCD.Cmd_Text(240, 34, 0, FT_OPT_CENTERX, gear);// GEAR

    LCD.Cmd_Number(364, 90, 31, FT_OPT_CENTER, DashInfo.Speed);// MPH
    LCD.Cmd_Text(364, 114, 26, FT_OPT_CENTER, "MPH");


    LCD.Cmd_Text(10, 255, 27, FT_OPT_CENTERY, "OIL");
    LCD.Cmd_Text(4, 237, 27, FT_OPT_CENTERY, "WTR");

    LCD.PrintText(265, 255, 26, FT_OPT_CENTERY, "%.1f F", DashInfo.OilTemp);
    LCD.PrintText(265, 237, 26, FT_OPT_CENTERY, "%.1f F", DashInfo.EngineTemp);

    //oil temp
    LCD.ColorRGB (oilBarColor.r, oilBarColor.g, oilBarColor.b);
    LCD.Cmd_Progress (50, 249, 200, 12, FT_OPT_FLAT, DashInfo.OilTemp,
                      CCDashConfig::OilPressureErrThresholdL); // last two (value, range)

    // water temp
    LCD.ColorRGB (waterBarColor.r, waterBarColor.g, waterBarColor.b); // water tmp color
    LCD.Cmd_Progress (50, 229, 200, 12, FT_OPT_FLAT, DashInfo.EngineTemp,
                      CCDashConfig::EngineTempErrThresholdH); // last two (value, range)



    LCD.ColorRGB(0, 255, 255);
    LCD.Cmd_Number(140, 90, 25, FT_OPT_CENTERY | FT_OPT_RIGHTX, DashInfo.RPM);// RPM


    // OP Box color
    LCD.ColorRGB(OPBoxColor.r, OPBoxColor.g, OPBoxColor.b);
    LCD.Begin(FT_RECTS);
    LCD.Vertex2ii(325, 222, 0, 0);
    LCD.Vertex2ii(375, 247, 0, 0);
    LCD.End();

    LCD.ColorRGB(OPTextColor.r, OPTextColor.g, OPTextColor.b);// OP Color, select based on temp
    LCD.Cmd_Text(350, 235, 27, FT_OPT_CENTER, "OP");

    // BV Box Color
    LCD.ColorRGB(BVBoxColor.r, BVBoxColor.g, BVBoxColor.b);
    LCD.Begin(FT_RECTS);
    LCD.Vertex2ii(325, 247, 0, 0);
    LCD.Vertex2ii(375, 272, 0, 0);
    LCD.End();

    LCD.ColorRGB(BVTextColor.r, BVTextColor.g, BVTextColor.b);// BV COLOR
    LCD.Cmd_Text(350, 260, 27, FT_OPT_CENTER, "BV");


    LCD.ColorRGB(255, 255, 255);
    LCD.PrintText(390, 235, 26, FT_OPT_CENTERY, "%.1f PSI", DashInfo.OilPressure);
    LCD.PrintText(390, 260, 26, FT_OPT_CENTERY, "%.1f V", DashInfo.BatteryVoltage);


    if(CurrentError.GetErrorType() != Error::ErrorType::Invalid)
    {
        // warnings
        LCD.ColorRGB(255, 0, 0);
        LCD.Begin(FT_RECTS);
        LCD.Vertex2ii(0, 120, 0, 0);
        LCD.Vertex2ii(250, 200, 0, 0);
        LCD.End();
        LCD.ColorRGB(0, 0, 0);
        LCD.Begin(FT_RECTS);
        LCD.Vertex2ii(10, 130, 0, 0);
        LCD.Vertex2ii(240, 190, 0, 0);
        LCD.End();
        LCD.ColorRGB(255, 255, 0);
        LCD.Cmd_Text(125, 143, 29, FT_OPT_CENTER, errorType);
        LCD.Cmd_Text(125, 170, 27, FT_OPT_CENTER, errorMsg);
    }

    // NOCAN
    if (bDisplayNoCAN)
    {
        LCD.ColorRGB (255, 0, 0);
        LCD.Begin (FT_RECTS);
        LCD.Vertex2ii (0, 0, 0, 0);
        LCD.Vertex2ii (200, 50, 0, 0);
        LCD.End ();
        LCD.ColorRGB (0, 0, 0);
        LCD.Begin (FT_RECTS);
        LCD.Vertex2ii (5, 5, 0, 0);
        LCD.Vertex2ii (195, 45, 0, 0);
        LCD.End ();
        LCD.ColorRGB (255, 255, 0);
        LCD.Cmd_Text (100, 25, 30, FT_OPT_CENTER, "NO DATA");
    }

    LCD.ColorRGB(156, 156, 156);// grey line
    LCD.Begin(FT_RECTS);
    LCD.Vertex2ii(0, 220, 0, 0);
    LCD.Vertex2ii(480, 220, 0, 0);
    LCD.End();

    LCD.DLEnd ();
    LCD.Finish ();
}

void Driving::SetDisplayError(Error e)
{
    CurrentError = e;
}

void Driving::SetDisplayNoCAN(bool displayNOCAN)
{
    bDisplayNoCAN = displayNOCAN;
}
