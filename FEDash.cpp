/*
 * FEDash.cpp
 *
 *  Created on: Apr 11, 2018
 *      Author: root
 */

#include "FEDash.h"
#include "Input.h"
#include "Delegate.h"
#include "DashPages/DashPage.h"

void FEDash::Init()
{
    Screen::Init();

    Serial.println("FEDash::Init()");

    CANRaw& can = CANRaw::StaticClass();
    // set up CAN Mob for incoming messages
    CanRxMobHandle = can.GetNextFreeMob();
    can.ConfigRx(DashDisplayCANMsg, DashDisplayCANMsgMsk, CanRxMobHandle);
    can.BindListener(this, CanRxMobHandle, true);

    // hook up callbacks
    // input
    Input& input = Subsystem::StaticClass<Input>();
    input.BindOnChangeButton(delegate::from_method<FEDash, &FEDash::OnButtonInputChange0>(this), 0);
    input.BindOnChangeButton(delegate::from_method<FEDash, &FEDash::OnButtonInputChange1>(this), 1);
    input.BindOnChangeRotary(delegate::from_method<FEDash, &FEDash::OnRotaryInputChange0>(this), 0);
    input.BindOnChangeRotary(delegate::from_method<FEDash, &FEDash::OnRotaryInputChange1>(this), 1);
    input.BindOnChangeRotary(delegate::from_method<FEDash, &FEDash::OnRotaryInputChange2>(this), 2);

    // create display screens
    // TODO
}
//
void FEDash::Update(uint8_t)
{
    Screen::Update(0);
}

// called on Can rx for Mob. Get received data with GetCANData()
// dlc is the data length code of the received message. It may be different
// than the one set in FrameData
void FEDash::INT_Call_GotFrame( const CANRaw::CAN_FRAME_HEADER& FrameData,
                                const CANRaw::CAN_DATA& Data )
{
    Screen::INT_Call_GotFrame(FrameData, Data);// tracks Rx occurrence
    // copy out data
    header = FrameData;
    data = Data;
}

// on no can data rx for interval
void FEDash::OnNoCANData()
{
    Serial.println(FSTR("FEDash::OnNoCANData"));
}

// used for input Change callback
// rotary sw
void FEDash::OnRotaryInputChange0( uint8_t pos )
{
    Serial.println(FSTR("FEDash::OnRotaryInputChange0"));
}
void FEDash::OnRotaryInputChange1( uint8_t pos )
{

}
void FEDash::OnRotaryInputChange2( uint8_t pos )
{

}
// buttons
void FEDash::OnButtonInputChange0( uint8_t pos )
{

}
void FEDash::OnButtonInputChange1( uint8_t pos )
{

}
