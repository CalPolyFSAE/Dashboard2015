/*
 * FEDash.cpp
 *
 *  Created on: Apr 11, 2018
 *      Author: root
 */

#include "FEDash.h"
#include "../../Input.h"
#include "../../Delegate.h"
#include "../DashPage.h"
#include "FEDashConfig.h"
#include "Charging.h"

FEDash::FEDash() :
    CANData{},
    CANDataSafe{},
    pages{}
{

}

void FEDash::Init()
{
    Screen::Init();

    CANRaw& can = CANRaw::StaticClass();

    // set up CAN Mobs for incoming messages
    CANRaw::CAN_MOB CanRxMobHandle = can.GetNextFreeMob();
    can.ConfigRx(FEDashConfig::DashCAN0, FEDashConfig::DashCANMsgMsk, CanRxMobHandle);
    can.BindListener(this, CanRxMobHandle, true);

    CanRxMobHandle = can.GetNextFreeMob();
    can.ConfigRx(FEDashConfig::DashCAN1, FEDashConfig::DashCANMsgMsk, CanRxMobHandle);
    can.BindListener(this, CanRxMobHandle, true);

    CanRxMobHandle = can.GetNextFreeMob();
    can.ConfigRx(FEDashConfig::DashCAN2, FEDashConfig::DashCANMsgMsk, CanRxMobHandle);
    can.BindListener(this, CanRxMobHandle, true);

    CanRxMobHandle = can.GetNextFreeMob();
    can.ConfigRx(FEDashConfig::DashCAN3, FEDashConfig::DashCANMsgMsk, CanRxMobHandle);
    can.BindListener(this, CanRxMobHandle, true);

    CanRxMobHandle = can.GetNextFreeMob();
    can.ConfigRx(FEDashConfig::WarningCAN, FEDashConfig::DashCANMsgMsk, CanRxMobHandle);
    can.BindListener(this, CanRxMobHandle, true);

    // create display screens
    // TODO
    AddNextPage(new Charging(LCD, CANDataSafe));
}
//
void FEDash::Update(uint8_t)
{
    Screen::Update(0);

    // copy data out of volatile buffer
    ATOMIC_BLOCK(ATOMIC_FORCEON)
    {
        CANDataSafe = *(const_cast<DashTypes::DashData*>(&CANData));// get rid of cv qualifier
    }
}

// called on Can rx for Mob. Get received data with GetCANData()
// dlc is the data length code of the received message. It may be different
// than the one set in FrameData
void FEDash::INT_Call_GotFrame( const CANRaw::CAN_FRAME_HEADER& FrameData,
                                const CANRaw::CAN_DATA& Data )
{
    Screen::INT_Call_GotFrame(FrameData, Data);// tracks Rx occurrence
    // copy out data
    for(uint8_t i = 0; i < DashTypes::DashDataPackLayoutSize; ++i)
    {
        if(FrameData.id == FEDashConfig::FEDashDataPackLayout[i])
        {
            CANData.Msg.data[i] = Data;
        }
    }
}

// on no can data rx for interval
void FEDash::OnNoCANData()
{
    Screen::OnNoCANData();
    //Serial.println(FSTR("FEDash::OnNoCANData"));
}

void FEDash::RunningDraw()
{
    if(pages[currentPage] != nullptr)
    {
        pages[currentPage]->Draw();
    }
}

void FEDash::AddNextPage(DashPage* page)
{
    for(uint8_t i = 0; i < FEDashConfig::NUMPAGES; ++i)
    {
        if(pages[i] == nullptr)
        {
            pages[i] = page;
            return;
        }
    }
}

// used for input Change callback
// rotary sw
void FEDash::OnRotaryInputChange0( uint8_t pos )
{
    Screen::OnRotaryInputChange0(pos);
    Serial.println(FSTR("FEDash::OnRotaryInputChange0"));
}
void FEDash::OnRotaryInputChange1( uint8_t pos )
{
    Screen::OnRotaryInputChange1(pos);
    Serial.println(FSTR("FEDash::OnRotaryInputChange1"));
}
void FEDash::OnRotaryInputChange2( uint8_t pos )
{
    Screen::OnRotaryInputChange2(pos);
    Serial.println(FSTR("FEDash::OnRotaryInputChange2"));
}
// buttons
void FEDash::OnButtonInputChange0( uint8_t pos )
{
    Screen::OnButtonInputChange0(pos);
    Serial.println(FSTR("FEDash::OnButtonInputChange0"));
}
void FEDash::OnButtonInputChange1( uint8_t pos )
{
    Screen::OnButtonInputChange1(pos);
    Serial.println(FSTR("FEDash::OnButtonInputChange1"));
}
