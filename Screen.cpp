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
    CurrentState = StartupStates::STARTUP;
}

// startup
void Screen::Init()
{
    Subsystem::Init();
    // set up graphics controller

    LCD.Init(FT_DISPLAY_RESOLUTION, 0, false);

    LCD.DisplayOn();

    uploadLogoToController();

    // set up CAN Mob for outgoing input data
    CANRaw& can = CANRaw::StaticClass();
    CanTxMobHandle = can.GetNextFreeMob();
    can.ConfigTx(DashInputCANMsg, CanTxMobHandle);

#ifdef DASHCANOUTPUTINTERVAL
    // Switch CAN message event
    if (SubsystemControl::StaticClass ().RegisterEvent (
            delegate::from_method<Screen, &Screen::SendSwitchPositions> (this),
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

    checkNoCAN();

    displayStateActions();
}

void Screen::ForceStateRunning()
{
    CurrentState = StartupStates::RUNNING;
}

void Screen::SendSwitchPositions(uint8_t)
{
    CANRaw::CAN_DATA data {};
    DashInputCANMsgDataFormat* inputData = (DashInputCANMsgDataFormat*) data.byte;

    // the indices used here are based on position of the rotary switch in CONFIG::ADCINPUTS[]
    inputData->RedRotary = Subsystem::StaticClass<Input> ().getRotaryPos (Input::ROTARY::RED);
    inputData->YellowRotary = Subsystem::StaticClass<Input> ().getRotaryPos (Input::ROTARY::YELLOW);
    inputData->BlackRotary = Subsystem::StaticClass<Input> ().getRotaryPos (Input::ROTARY::BLACK);

    // this is not a great way of doing this, but it simplifies the
    // operation by avoiding multiple calls to Input::getButtonPos(index) and
    // getting rid of the need to format the data for the message.
    // It will cause issues if the buttons change pins on mcu
    inputData->ButtonsArray = ~PINC;

    CANRaw::StaticClass().INTS_TxData(data, CanTxMobHandle);
}

// PRIVATE

const uint8_t PROGMEM Screen::CPRacingLogo[] = {
#include "GraphicsAssets/CPRacingLogo.inc"
};

void Screen::displayStateActions()
{
    switch(CurrentState)
    {
        case StartupStates::RUNNING:
            RunningDraw();
            break;
        case StartupStates::WAITING_FOR_INPUT:
            displayWaitingScreen();
            break;
        case StartupStates::STARTUP:
            displayStartingScreen();
            break;
    }
}

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
    LCD.BitmapSource (108676);// source address in GPU memory
    LCD.BitmapLayout (FT_ARGB1555, 500, 49);
    LCD.BitmapSize (FT_BILINEAR, FT_BORDER, FT_BORDER, 250, 49);
    LCD.DLEnd ();
    LCD.Finish ();
}

void Screen::displayStartingScreen() {

    // display for about 3 seconds
    static uint8_t StartupTimer = 200;
    if (StartupTimer > 0)
    {
        --StartupTimer;
        if (StartupTimer == 0)
            CurrentState = StartupStates::WAITING_FOR_INPUT;
    }

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

void Screen::displayWaitingScreen()
{
    LCD.DLStart ();
    LCD.Clear(0,0,0);
    LCD.ColorRGB(255, 255, 255);

    uint8_t rpos = Subsystem::StaticClass<Input> ().getRotaryPos (Input::ROTARY::RED);
    uint8_t ypos = Subsystem::StaticClass<Input> ().getRotaryPos (Input::ROTARY::YELLOW);
    uint8_t bpos = Subsystem::StaticClass<Input> ().getRotaryPos (Input::ROTARY::BLACK);

    if(rpos != 0 || ypos != 0 || bpos != 0)
    {
        LCD.Cmd_Text(228, 137, 26, FT_OPT_CENTER, "Switch position error");
    }

    LCD.Cmd_Scale(32768, 32768);
    LCD.Cmd_SetMatrix();

    LCD.Cmd_Number(147, 178, 30, 0, rpos);
    LCD.Cmd_Number(216, 178, 30, 0, ypos);
    LCD.Cmd_Number(285, 178, 30, 0, bpos);

    LCD.Cmd_LoadIdentity();
    LCD.Cmd_SetMatrix();

    if(!bIsCANData)
    {
        LCD.ColorRGB(255, 0, 0);
        LCD.Begin(FT_RECTS);
        LCD.Vertex2ii(152, 45, 0, 0);
        LCD.Vertex2ii(309, 85, 0, 0);
        LCD.End();

        LCD.ColorRGB(255, 255, 0);
        LCD.Cmd_Text(230, 64, 28, FT_OPT_CENTER, "NO CAN DATA");
    }

    LCD.DLEnd();
    LCD.Finish();
/*
COLOR_RGB(255, 255, 255)
CMD_TEXT(228, 137, 26, OPT_CENTER, "Switch position error")

CMD_SCALE(32768, 32768)
CMD_SETMATRIX()
CMD_NUMBER(147, 178, 30, 0, 7)
CMD_NUMBER(216, 178, 30, 0, 7)
CMD_NUMBER(285, 178, 30, 0, 7)

CMD_LOADIDENTITY()
CMD_SETMATRIX()


COLOR_RGB(255, 0, 0)
BEGIN(RECTS)
VERTEX2II(152, 45, 0, 0)
VERTEX2II(309, 85, 0, 0)
END()
COLOR_RGB(255, 255, 0)
CMD_TEXT(230, 64, 28, OPT_CENTER, "NO CAN DATA")
     */
}


void Screen::checkNoCAN()
{
    static uint8_t CANNoRxCounter = 0;
    static uint8_t CANRxCounter = 0;
    bool cpyRxCAN = bRxCANSinceLastUpdate;
    bRxCANSinceLastUpdate = false;
    if(!cpyRxCAN)
    {
        ++CANNoRxCounter;
        CANRxCounter = 0;
        if(CANNoRxCounter > CONFIG::MAXNOCANUPDATES)
        {
            OnNoCANData();
            bIsCANData = false;
            CANNoRxCounter = 0;
        }
    }else
    {
        ++CANRxCounter;
        CANNoRxCounter = 0;
        if(CANRxCounter > CONFIG::MAXNOCANUPDATES)
        {
            bIsCANData = true;
            CANRxCounter = 0;
        }
    }
}


