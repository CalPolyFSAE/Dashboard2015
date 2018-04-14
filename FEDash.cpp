/*
 * FEDash.cpp
 *
 *  Created on: Apr 11, 2018
 *      Author: root
 */

#include "FEDash.h"
#include "Input.h"
#include "Delegate.h"

void FEDash::Init()
{
    Screen::Init();

    // set up CAN Mob for outgoing input data
    CANRaw& can = CANRaw::StaticClass();
    CanTxMobHandle = can.GetNextFreeMob();
    can.ConfigTx(DashInputCANMsg, CanTxMobHandle);

    // set up CAN Mob for incoming messages
    CanRxMobHandle = can.GetNextFreeMob();
    can.ConfigRx(DashDisplayCANMsg, DashDisplayCANMsgMsk, CanRxMobHandle);
    can.BindListener(this, CanRxMobHandle, true);

    // hook up callbacks
    // input
    Input& input = Input::StaticClass();
    input.BindOnChangeButton(delegate::from_method<FEDash, &FEDash::OnButtonInputChange0>(this), 0);
    input.BindOnChangeButton(delegate::from_method<FEDash, &FEDash::OnButtonInputChange1>(this), 1);
    input.BindOnChangeRotary(delegate::from_method<FEDash, &FEDash::OnRotaryInputChange0>(this), 0);
    input.BindOnChangeRotary(delegate::from_method<FEDash, &FEDash::OnRotaryInputChange1>(this), 1);
    input.BindOnChangeRotary(delegate::from_method<FEDash, &FEDash::OnRotaryInputChange2>(this), 2);

    // Switch CAN message event
    if (SubsystemControl::StaticClass ().RegisterEvent (
            delegate::from_method<FEDash, &FEDash::sendSwitchPositions> (this),
            12) < 0)
    {
#ifdef DEBUG_PRINT
        Serial.print (FSTR("[ERROR]: "));
        Serial.print (__FILE__);
        Serial.print (FSTR(" at "));
        Serial.print (__LINE__);
        Serial.print (FSTR(": RegisterEvent returned a negative number"));
#endif //DEBUG_PRINT
    }

    // update event
    if (SubsystemControl::StaticClass ().RegisterEvent (
            delegate::from_method<FEDash, &FEDash::Update> (this),
            CONFIG::INPUTINTERVAL) < 0)
    {
#ifdef DEBUG_PRINT
        Serial.print (FSTR("[ERROR]: "));
        Serial.print (__FILE__);
        Serial.print (FSTR(" at "));
        Serial.print (__LINE__);
        Serial.print (FSTR(": RegisterEvent returned a negative number"));
#endif //DEBUG_PRINT
    }

    // create display screens
    // TODO
}
//
void FEDash::Update(uint8_t)
{
    Serial.println(FSTR("FEDash"));
}

// used for input Change callback
// rotary sw
void FEDash::OnRotaryInputChange0( uint8_t pos )
{
    Serial.println(FSTR("OnRotaryInputChange0"));
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


void FEDash::sendSwitchPositions(uint8_t)
{
    Serial.println(FSTR("sendSwitchPositions"));
    CANRaw::CAN_DATA data {};
    DashInputCANMsgDataFormat* inputData = (DashInputCANMsgDataFormat*) data.byte;

    // the indices used here are based on position of the rotary switch in CONFIG::ADCINPUTS[]
    inputData->RedRotary = Input::StaticClass ().getRotaryPos (0);
    inputData->YellowRotary = Input::StaticClass ().getRotaryPos (1);
    inputData->BlackRotary = Input::StaticClass ().getRotaryPos (2);

    // this is not a great way of doing this, but it simplifies the
    // operation by avoiding multiple calls to Input::getButtonPos(index) and
    // getting rid of the need to format the data for the message.
    // It will cause issues if the buttons change pins on micro
    inputData->ButtonsArray = ~PINC;

    CANRaw::StaticClass().INTS_TxData(data, CanTxMobHandle);
}
