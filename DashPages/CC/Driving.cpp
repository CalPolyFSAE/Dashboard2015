/*
 * Driving.cpp
 *
 *  Created on: May 14, 2018
 *      Author: root
 */

#include "Driving.h"

Driving::Driving(FT801IMPL_SPI& LCD, const CCDashConfig::DashInfo& info) :
    LCD(LCD)
{
    DashInfo = info;
}

void Driving::Begin()
{

}

void Driving::Draw()
{
    float EngineTemp, RPM, Gear, Speed, OilTemp, OilPressure, BatteryVoltage;

    EngineTemp = motecToFloat(DashInfo.c0.EngineTemp);
    RPM = motecToFloat(DashInfo.c0.RPM, 1.0);
    Gear = motecToFloat(DashInfo.c0.Gear);
    Speed = motecToFloat(DashInfo.c0.Speed);
    OilTemp = motecToFloat(DashInfo.c1.OilTemp);
    OilPressure = motecToFloat(DashInfo.c2.OilPressure);
    BatteryVoltage = motecToFloat(DashInfo.c2.BatteryVoltage);

    LCD.DLStart();
    LCD.ClearColorRGB(0, 0, 0);

    LCD.Cmd_Text(240, 34, 0, FT_OPT_CENTERX, "");// GEAR

    LCD.Cmd_Number(364, 90, 31, FT_OPT_CENTER, Speed);// MPH
    LCD.Cmd_Text(364, 114, 26, FT_OPT_CENTER, "MPH");


    LCD.Cmd_Text(10, 255, 27, FT_OPT_CENTERY, "OIL");
    LCD.Cmd_Text(4, 237, 27, FT_OPT_CENTERY, "WTR");

    LCD.PrintText(265, 255, 26, FT_OPT_CENTERY, "%.1f F", OilTemp);
    LCD.PrintText(265, 237, 26, FT_OPT_CENTERY, "%.1f F", EngineTemp);

    LCD.ColorRGB(255, 0, 0);// oil color
    LCD.Cmd_Progress(50, 249, 200, 12, FT_OPT_FLAT, 92, 100);// last two (value, range)

    LCD.ColorRGB(0, 255, 0);// water color
    LCD.Cmd_Progress(50, 229, 200, 12, FT_OPT_FLAT, 66, 100);// last two (value, range)



    LCD.ColorRGB(0, 255, 255);
    LCD.Cmd_Number(140, 90, 25, FT_OPT_CENTERY | FT_OPT_RIGHTX, RPM);// RPM



    LCD.ColorRGB(255, 0, 0);// OP Box color
    LCD.Begin(FT_RECTS);
    LCD.Vertex2ii(325, 222, 0, 0);
    LCD.Vertex2ii(375, 247, 0, 0);
    LCD.End();

    LCD.ColorRGB(251, 255, 0);// OP Color, select based on temp
    LCD.Cmd_Text(350, 235, 27, FT_OPT_CENTER, "OP");


    LCD.ColorRGB(38, 255, 0);// BV Box Color
    LCD.Begin(FT_RECTS);
    LCD.Vertex2ii(325, 247, 0, 0);
    LCD.Vertex2ii(375, 272, 0, 0);
    LCD.End();

    LCD.ColorRGB(0, 0, 0);// BV COLOR
    LCD.Cmd_Text(350, 260, 27, FT_OPT_CENTER, "BV");


    LCD.ColorRGB(255, 255, 255);
    LCD.PrintText(390, 235, 26, FT_OPT_CENTERY, "%.1f PSI", OILPRESS);
    LCD.PrintText(390, 260, 26, FT_OPT_CENTERY, "%.1f V", BATVOLT);


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
    LCD.Cmd_Text(125, 143, 29, FT_OPT_CENTER, "ERROR LEVEL");
    LCD.Cmd_Text(125, 170, 27, FT_OPT_CENTER, "ERROR MESSAGE");

    // NOCAN
    LCD.ColorRGB(255, 0, 0);
    LCD.Begin(FT_RECTS);
    LCD.Vertex2ii(0, 0, 0, 0);
    LCD.Vertex2ii(200, 50, 0, 0);
    LCD.End();
    LCD.ColorRGB(0, 0, 0);
    LCD.Begin(FT_RECTS);
    LCD.Vertex2ii(5, 5, 0, 0);
    LCD.Vertex2ii(195, 45, 0, 0);
    LCD.End();
    LCD.ColorRGB(255, 255, 0);
    LCD.Cmd_Text(100, 25, 30, FT_OPT_CENTER, "NO DATA");

    LCD.ColorRGB(156, 156, 156);// grey line
    LCD.Begin(FT_RECTS);
    LCD.Vertex2ii(0, 220, 0, 0);
    LCD.Vertex2ii(480, 220, 0, 0);
    LCD.End();
}
