/*
 * Screen.cpp
 *
 *  Created on: Apr 10, 2018
 *      Author: root
 */

#include "Screen.h"
#include "Input.h"


// PROTECTED
Screen::Screen() :
    Subsystem<Screen>(16)// 16ms update interval
{

}

// startup
void Screen::Init()
{
    Subsystem<Screen>::Init();

    // set up graphics controller


    // hook up callbacks
    Input::StaticClass().BindOnChangeButton(delegate::from_method<Screen, &Screen::OnInputTOG>(this));
    Input::StaticClass().BindOnChangeButton(delegate::from_method<Screen, &Screen::OnInputADC>(this));

    // create display screens
}
//
void Screen::Update(uint8_t)
{
    Subsystem<Screen>::Update(0);
}

// PRIVATE

// called on Can rx for Mob. Get received data with GetCANData()
// dlc is the data length code of the received message. It may be different
// than the one set in FrameData
void Screen::INT_Call_GotFrame( const CANRaw::CAN_FRAME_HEADER& FrameData,
                                const CANRaw::CAN_DATA& Data )
{
    // copy out data
    header = FrameData;
    data = Data;
}

// initial handlers of the Input event
void Screen::OnInputADC(uint8_t pos)
{
    OnRotaryInputChange(pos);
}
void Screen::OnInputTOG(uint8_t pos)
{
    OnButtonInputChange(pos);
}


