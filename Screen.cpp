/*
 * Screen.cpp
 *
 *  Created on: Apr 10, 2018
 *      Author: root
 */

#include "Screen.h"
#include "util/atomic.h"
#include "Input.h"
#include "CANLib.h"


// PROTECTED
Screen::Screen() :
    Subsystem(CONFIG::SCREENINTERVAL)
{

}

// startup
void Screen::Init()
{
    Subsystem::Init();
    // set up graphics controller

    LCD.Init(FT_DISPLAY_RESOLUTION, 0, false);

    LCD.DisplayOn();

    uploadLogoToController();

    displayStartingScreen();

    // set up CAN Mob for outgoing input data
    CANRaw& can = CANRaw::StaticClass();
    CanTxMobHandle = can.GetNextFreeMob();
    can.ConfigTx(DashInputCANMsg, CanTxMobHandle);

#ifdef DASHCANOUTPUTINTERVAL
    // Switch CAN message event
    if (SubsystemControl::StaticClass ().RegisterEvent (
            delegate::from_method<Screen, &Screen::sendSwitchPositions> (this),
            DASHCANOUTPUTINTERVAL) < 0)
    {
#ifdef DEBUG_PRINT
        Serial.print (FSTR("[ERROR]: "));
        Serial.print (__FILE__);
        Serial.print (FSTR(" at "));
        Serial.print (__LINE__);
        Serial.print (FSTR(": RegisterEvent returned a negative number"));
#endif //DEBUG_PRINT
#endif //DASHCANOUTPUTINTERVAL
    }
}

void Screen::Update(uint8_t)
{
    Subsystem::Update(0);
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

void Screen::sendSwitchPositions(uint8_t)
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
    // just need to keep track of message rx
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

void Screen::displayStartingScreen() {
    LCD.DLStart ();
    const char Display_string[] = "Cal Poly FSAE";

    LCD.ColorRGB (0xFF, 0xFF, 0xFF);
    LCD.Cmd_Text (FT_DISPLAYWIDTH / 2, FT_DISPLAYHEIGHT / 8, 29, FT_OPT_CENTER,
                  "Cal Poly FSAE");
    LCD.ColorRGB (0xFF, 0x00, 0x00);
    LCD.PrintText (FT_DISPLAYWIDTH / 2, FT_DISPLAYHEIGHT / 2, 29, FT_OPT_CENTER,
                   "...STARTING...");

    LCD.ColorRGB (0xFF, 0xFF, 0xFF);
    LCD.Begin (FT_BITMAPS);
    LCD.Vertex2ii (FT_DISPLAYWIDTH / 4, 215, 0, 0);
    LCD.End ();

    LCD.DLEnd ();
    LCD.Finish ();
}


