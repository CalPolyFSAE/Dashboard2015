/*
 * CCDash.cpp
 *
 *  Created on: May 12, 2018
 *      Author: root
 */


#include "CCDash.h"
#include "../../Input.h"
#include "../../Delegate.h"
#include "../DashPage.h"
#include "CCDashConfig.h"
#include "CCDashStr.h"
#include "../DashPage.h"
#include "Driving.h"
#include "ErrorConditions.h"
#include "AVRLibrary/arduino/Arduino.h"

const uint8_t PROGMEM CCDash::MSFont[] = {
#include "../../GraphicsAssets/MSFont"
};

CCDash::CCDash() :
    DashInfoR{},
    DashInfoSafe{},
    DisplayInfo{},
    pages(),
    rot0pos(),
    rot1pos(),
    rot2pos()
{
    ErrorManager = nullptr;
    DrivingPage = nullptr;
}

void CCDash::Init()
{
    Screen::Init();

    uploadFontToController();

    CANRaw& can = CANRaw::StaticClass();

    // set up CAN Mobs for incoming messages
    CANRaw::CAN_MOB CanRxMobHandle = can.GetNextFreeMob();
    can.ConfigRx(CCDashConfig::DashCANFrame0, CCDashConfig::DashCANMsgMsk, CanRxMobHandle);
    can.BindListener(this, CanRxMobHandle, true);
    Serial.println((uint8_t)CanRxMobHandle);

    CanRxMobHandle = can.GetNextFreeMob();
    can.ConfigRx(CCDashConfig::DashCANFrame1, CCDashConfig::DashCANMsgMsk, CanRxMobHandle);
    can.BindListener(this, CanRxMobHandle, true);
    Serial.println((uint8_t)CanRxMobHandle);

    CanRxMobHandle = can.GetNextFreeMob();
    can.ConfigRx(CCDashConfig::DashCANFrame2, CCDashConfig::DashCANMsgMsk, CanRxMobHandle);
    can.BindListener(this, CanRxMobHandle, true);
    Serial.println((uint8_t)CanRxMobHandle);


    //ErrorManager = new ErrorConditions(DisplayInfo);

    // create display screens
    // TODO
    DrivingPage = new Driving(LCD, DisplayInfo);
    AddNextPage(DrivingPage);

    ForceStateRunning();
}
//
void CCDash::Update(uint8_t)
{
    Screen::Update(0);


    bool needUpdate = false;
    // copy data out of volatile buffer
    ATOMIC_BLOCK(ATOMIC_FORCEON)
    {
        needUpdate = bNewData;
        if(needUpdate)
        {
            bNewData = false;
            DashInfoSafe = *(const_cast<CCDashConfig::DashInfo*>(&DashInfoR));// get rid of cv qualifier
        }
    }

    if (needUpdate)// update values
    {
        DisplayInfo.EngineTemp = DashPage::motecToFloat(DashInfoSafe.c0.EngineTemp);
        DisplayInfo.RPM = DashPage::motecToFloat(DashInfoSafe.c0.RPM, 1.0);
        DisplayInfo.Gear = DashPage::motecToFloat(DashInfoSafe.c0.Gear);
        DisplayInfo.Speed = DashPage::motecToFloat(DashInfoSafe.c0.Speed);
        DisplayInfo.Lambda = DashPage::motecToFloat(DashInfoSafe.c1.Lambda);
        DisplayInfo.OilTemp = DashPage::motecToFloat(DashInfoSafe.c1.OilTemp);
        DisplayInfo.MAP = DashPage::motecToFloat(DashInfoSafe.c1.MAP);
        DisplayInfo.ThrottlePOS = DashPage::motecToFloat(DashInfoSafe.c1.ThrottlePOS);
        DisplayInfo.BatteryVoltage = DashPage::motecToFloat(DashInfoSafe.c2.BatteryVoltage);
        DisplayInfo.OilPressure = DashPage::motecToFloat(DashInfoSafe.c2.OilPressure);
        DisplayInfo.IAT = DashPage::motecToFloat(DashInfoSafe.c2.IAT);
    }


}

// called on Can rx for Mob. Get received data with GetCANData()
// dlc is the data length code of the received message. It may be different
// than the one set in FrameData
void CCDash::INT_Call_GotFrame( const CANRaw::CAN_FRAME_HEADER& FrameData,
                                const CANRaw::CAN_DATA& Data )
{
    Screen::INT_Call_GotFrame(FrameData, Data);// tracks Rx occurrence
    bNewData = true;
    // copy out data
    for(uint8_t i = 0; i < CCDashConfig::NUM_CAN_MESSAGES; ++i)
    {
        if(FrameData.id == CCDashConfig::DashInfoLayout[i].id)
        {
            DashInfoR.msg[i] = Data;
        }
    }
}

// on no can data rx for interval
void CCDash::OnNoCANData()
{
    Screen::OnNoCANData();
    //Serial.println(FSTR("FEDash::OnNoCANData"));
    DrivingPage->SetDisplayNoCAN(true);
}

void CCDash::OnCANData()
{
    Screen::OnCANData();
    DrivingPage->SetDisplayNoCAN(false);
}

void CCDash::RunningDraw()
{
    uint8_t pageS = currentPage % CCDashConfig::NUMPAGES;
    if(pages[pageS] != nullptr)
    {
        pages[pageS]->Draw();
    }else
    {
        DrawInvalidPage();
    }
}

void CCDash::DrawInvalidPage()
{
    LCD.DLStart();
    LCD.Clear(0, 0, 0);

    LCD.Cmd_Scale(32768, 32768);
    LCD.Cmd_Translate(0, -1048576);
    LCD.Cmd_SetMatrix();
    LCD.Begin(FT_BITMAPS);
    LCD.Vertex2ii(328, 0, 1, 0);
    LCD.End();
    LCD.Cmd_LoadIdentity();
    LCD.Cmd_SetMatrix();

    LCD.DLEnd();
    LCD.Finish();
}

void CCDash::AddNextPage(DashPage* page)
{
    for(uint8_t i = 0; i < CCDashConfig::NUMPAGES; ++i)
    {
        if(pages[i] == nullptr)
        {
            pages[i] = page;
            return;
        }
    }
}

void CCDash::uploadFontToController()
{
    LCD.Cmd_Inflate(0);
    LCD.WriteCmdfromflash(MSFont, sizeof(MSFont));
    LCD.Finish();

    LCD.DLStart();
    LCD.BitmapHandle(0);
    LCD.BitmapSource(-115660);
    LCD.BitmapLayout(FT_L4, 28, 88);
    LCD.BitmapSize(FT_NEAREST, FT_BORDER, FT_BORDER, 56, 88);
    LCD.Cmd_SetFont(0, 0);
    LCD.DLEnd ();
    LCD.Finish ();
}

// used for input Change callback
// rotary sw
void CCDash::OnRotaryInputChange0( uint8_t pos )
{
    Screen::OnRotaryInputChange0(pos);
    Serial.println("CCDash::OnRotaryInputChange0");
    rot0pos = pos;
}
void CCDash::OnRotaryInputChange1( uint8_t pos )
{
    Screen::OnRotaryInputChange1(pos);
    Serial.println("CCDash::OnRotaryInputChange1");
    rot1pos = pos;
    currentPage = pos;
}
void CCDash::OnRotaryInputChange2( uint8_t pos )
{
    Screen::OnRotaryInputChange2(pos);
    Serial.println("CCDash::OnRotaryInputChange2");
    rot2pos = pos;
}
// buttons
void CCDash::OnButtonInputChange0( uint8_t pos )
{
    Screen::OnButtonInputChange0(pos);
    Serial.println(FSTR("FEDash::OnButtonInputChange0"));
}
void CCDash::OnButtonInputChange1( uint8_t pos )
{
    Screen::OnButtonInputChange1(pos);
    Serial.println(FSTR("FEDash::OnButtonInputChange1"));
}


