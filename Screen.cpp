/*
 * Screen.cpp
 *
 *  Created on: Apr 10, 2018
 *      Author: root
 */

#include "Screen.h"
#include "util/atomic.h"

// PROTECTED
Screen::Screen() :
    Subsystem(CONFIG::SCREENINTERVAL)
{

}

// startup
void Screen::Init()
{
    Subsystem::Init();
    Serial.println("Screen::Init()");

    // set up graphics controller
    //LCD.Init(FT_DISPLAY_RESOLUTION, 0, false);
    //LCD.DisplayOn();
}

void Screen::Update(uint8_t)
{
    static uint8_t CANNoRxCounter = 0;
    bool cpyRxCAN = bRxCANSinceLastUpdate;
    bRxCANSinceLastUpdate = false;
    if(!cpyRxCAN)
    {
        ++CANNoRxCounter;
        if(CANNoRxCounter > CONFIG::MAXNOCANUPDATES)
        {
            OnNoCANData();
            CANNoRxCounter = 0;
        }
    }

}

// get copy of volatile FrameData
void Screen::GetHeaderData(CANRaw::CAN_FRAME_HEADER& outh)
{
    ATOMIC_BLOCK(ATOMIC_FORCEON)
    {
        // const_cast will cast away volatility
        outh = *const_cast<CANRaw::CAN_FRAME_HEADER*>(&header);
    }
}
// get copy of volatile Data
void Screen::GetData(CANRaw::CAN_DATA& outd)
{
    ATOMIC_BLOCK(ATOMIC_FORCEON)
    {
        // const_cast will cast away volatility
        outd = *const_cast<CANRaw::CAN_DATA*>(&data);
    }
}

// PRIVATE

const uint8_t PROGMEM Screen::CPRacingLogo[] = {
#include "GraphicsAssets/CPRacingLogo.inc"
};

// called on Can rx for Mob. Get received data with GetCANData()
// dlc is the data length code of the received message. It may be different
// than the one set in FrameData
void Screen::INT_Call_GotFrame( const CANRaw::CAN_FRAME_HEADER& FrameData,
                                const CANRaw::CAN_DATA& Data )
{
    // copy out data
    header = FrameData;
    data = Data;
    bRxCANSinceLastUpdate = true;
}

void Screen::uploadLogoToController() {
    LCD.Cmd_Inflate (108676);
    LCD.WriteCmdfromflash (CPRacingLogo, sizeof(CPRacingLogo));
    LCD.Finish ();
    LCD.DLStart ();
    LCD.BitmapHandle (0);
    LCD.BitmapSource (108676);
    LCD.BitmapLayout (FT_ARGB1555, 500, 49);
    LCD.BitmapSize (FT_BILINEAR, FT_BORDER, FT_BORDER, 250, 49);
    LCD.DLEnd ();
    LCD.Finish ();
}


