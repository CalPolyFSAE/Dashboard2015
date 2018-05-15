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

CCDash::CCDash() :
    DashInfoR{},
    DashInfoSafe{},
    pages{}
{

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

    CanRxMobHandle = can.GetNextFreeMob();
    can.ConfigRx(CCDashConfig::DashCANFrame1, CCDashConfig::DashCANMsgMsk, CanRxMobHandle);
    can.BindListener(this, CanRxMobHandle, true);

    CanRxMobHandle = can.GetNextFreeMob();
    can.ConfigRx(CCDashConfig::DashCANFrame2, CCDashConfig::DashCANMsgMsk, CanRxMobHandle);
    can.BindListener(this, CanRxMobHandle, true);

    // create display screens
    // TODO
    //AddNextPage(new Charging(LCD, CANDataSafe));
}
//
void CCDash::Update(uint8_t)
{
    Screen::Update(0);

    // copy data out of volatile buffer
    ATOMIC_BLOCK(ATOMIC_FORCEON)
    {
        DashInfoSafe = *(const_cast<CCDashConfig::DashInfo*>(&DashInfoR));// get rid of cv qualifier
    }
}

// called on Can rx for Mob. Get received data with GetCANData()
// dlc is the data length code of the received message. It may be different
// than the one set in FrameData
void CCDash::INT_Call_GotFrame( const CANRaw::CAN_FRAME_HEADER& FrameData,
                                const CANRaw::CAN_DATA& Data )
{
    Screen::INT_Call_GotFrame(FrameData, Data);// tracks Rx occurrence
    // copy out data
    for(uint8_t i = 0; i < CCDashConfig::NUM_CAN_MESSAGES; ++i)
    {
        if(FrameData.id == CCDashConfig::DashInfoLayout[i])
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
}

void CCDash::RunningDraw()
{
    if(pages[currentPage] != nullptr)
    {
        pages[currentPage]->Draw();
    }
}

void CCDash::AddNextPage(DashPage* page)
{
    for(uint8_t i = 0; i < NUMPAGES; ++i)
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

    LCD.Cmd_Inflate(-115660);
    LCD.WriteCmdfromflash(MSFont, sizeof(MSFont));
    LCD.Finish();

    LCD.DLStart();
    LCD.BitmapHandle(1);
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
    Serial.println(FSTR("FEDash::OnRotaryInputChange0"));
}
void CCDash::OnRotaryInputChange1( uint8_t pos )
{
    Screen::OnRotaryInputChange1(pos);
    Serial.println(FSTR("FEDash::OnRotaryInputChange1"));
}
void CCDash::OnRotaryInputChange2( uint8_t pos )
{
    Screen::OnRotaryInputChange2(pos);
    Serial.println(FSTR("FEDash::OnRotaryInputChange2"));
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


