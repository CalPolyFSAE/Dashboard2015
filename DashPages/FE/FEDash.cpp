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
    CANDataS{},
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

    // hook up callbacks
    // input
    Input& input = Subsystem::StaticClass<Input>();
    input.BindOnChangeButton(delegate::from_method<FEDash, &FEDash::OnButtonInputChange0>(this), Input::BUTTON::BTNL);
    input.BindOnChangeButton(delegate::from_method<FEDash, &FEDash::OnButtonInputChange1>(this), Input::BUTTON::BTNR);
    input.BindOnChangeRotary(delegate::from_method<FEDash, &FEDash::OnRotaryInputChange0>(this), Input::ROTARY::BLACK);
    input.BindOnChangeRotary(delegate::from_method<FEDash, &FEDash::OnRotaryInputChange1>(this), Input::ROTARY::RED);
    input.BindOnChangeRotary(delegate::from_method<FEDash, &FEDash::OnRotaryInputChange2>(this), Input::ROTARY::YELLOW);

    // create display screens
    // TODO
    AddNextPage(new Charging(LCD, CANDataS));
}
//
void FEDash::Update(uint8_t)
{
    Screen::Update(0);

    // copy data out of volatile buffer
    ATOMIC_BLOCK(ATOMIC_FORCEON)
    {
        CANDataS = *(const_cast<DashTypes::DashData*>(&CANData));// get rid of cv qualifier
    }

    if(pages[currentPage] != nullptr)
    {
        pages[currentPage]->Draw();
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
    Serial.println(FSTR("FEDash::OnNoCANData"));
}

void FEDash::AddNextPage(DashPage* page)
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

// used for input Change callback
// rotary sw
void FEDash::OnRotaryInputChange0( uint8_t pos )
{
    Serial.println(FSTR("FEDash::OnRotaryInputChange0"));
}
void FEDash::OnRotaryInputChange1( uint8_t pos )
{
    Serial.println(FSTR("FEDash::OnRotaryInputChange1"));
}
void FEDash::OnRotaryInputChange2( uint8_t pos )
{
    Serial.println(FSTR("FEDash::OnRotaryInputChange2"));
}
// buttons
void FEDash::OnButtonInputChange0( uint8_t pos )
{
    Serial.println(FSTR("FEDash::OnButtonInputChange0"));
}
void FEDash::OnButtonInputChange1( uint8_t pos )
{
    Serial.println(FSTR("FEDash::OnButtonInputChange1"));
}
