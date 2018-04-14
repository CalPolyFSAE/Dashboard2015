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

    Serial.println("FEDash::Init()");

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
    Input& input = Subsystem::StaticClass<Input>();
    input.BindOnChangeButton(delegate::from_method<FEDash, &FEDash::OnButtonInputChange0>(this), 0);
    input.BindOnChangeButton(delegate::from_method<FEDash, &FEDash::OnButtonInputChange1>(this), 1);
    input.BindOnChangeRotary(delegate::from_method<FEDash, &FEDash::OnRotaryInputChange0>(this), 0);
    input.BindOnChangeRotary(delegate::from_method<FEDash, &FEDash::OnRotaryInputChange1>(this), 1);
    input.BindOnChangeRotary(delegate::from_method<FEDash, &FEDash::OnRotaryInputChange2>(this), 2);

    // Switch CAN message event
    if (SubsystemControl::StaticClass ().RegisterEvent (
            delegate::from_method<FEDash, &FEDash::sendSwitchPositions> (this),
            DASHCANOUTPUTINTERVAL) < 0)
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
    Screen::Update(0);
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


void FEDash::sendSwitchPositions(uint8_t)
{
    CANRaw::CAN_DATA data {};
    DashInputCANMsgDataFormat* inputData = (DashInputCANMsgDataFormat*) data.byte;

    // the indices used here are based on position of the rotary switch in CONFIG::ADCINPUTS[]
    inputData->RedRotary = Subsystem::StaticClass<Input> ().getRotaryPos (0);
    inputData->YellowRotary = Subsystem::StaticClass<Input> ().getRotaryPos (1);
    inputData->BlackRotary = Subsystem::StaticClass<Input> ().getRotaryPos (2);

    // this is not a great way of doing this, but it simplifies the
    // operation by avoiding multiple calls to Input::getButtonPos(index) and
    // getting rid of the need to format the data for the message.
    // It will cause issues if the buttons change pins on micro
    inputData->ButtonsArray = ~PINC;

    CANRaw::StaticClass().INTS_TxData(data, CanTxMobHandle);
}
