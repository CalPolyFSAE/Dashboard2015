/*
 * Display.h
 *
 *  Created on: Jan 12, 2018
 *      Author: root
 */

#ifndef DASHBOARD2_SCREEN_H_
#define DASHBOARD2_SCREEN_H_

#include "Subsystem.h"
#include "CANLib.h"

#include "FTDI _V1/FT_VM801P43_50.h"

// interval to send out sw positions on CAN (ms)
#define DASHCANOUTPUTINTERVAL 12

// structure of outgoing input data
struct DashInputCANMsgDataFormat
{
    uint8_t ButtonsArray;
    uint8_t RedRotary;
    uint8_t YellowRotary;
    uint8_t BlackRotary;
};

// CAN message format used for outgoing button positions
constexpr CANRaw::CAN_FRAME_HEADER DashInputCANMsg =
    {
        0xF5,             // id
        0,                // rtr
        0,                // ide
        8                 // dlc
    };


// Display Subsystem
// logic differers between cars
class Screen : public Subsystem, public CANListener
{
public:
    friend class Subsystem;

protected:
    // Lcd control
    FT801IMPL_SPI LCD;

    Screen();

    // startup
    virtual void Init() override;
    virtual void Update(uint8_t) override;

    // called on Can rx for Mob. Get received data with GetCANData()
    // dlc is the data length code of the received message. It may be different
    // than the one set in FrameData
    virtual void INT_Call_GotFrame( const CANRaw::CAN_FRAME_HEADER& FrameData,
                                    const CANRaw::CAN_DATA& Data ) override;

    // events
    // no can data has been received in MAXNOCANUPDATES update cycles
    virtual void OnNoCANData() {}

    // every 12 ms?
    void sendSwitchPositions(uint8_t);

private:

    // logo upload
    void uploadLogoToController();
    // shows logo and some text
    void displayStartingScreen();

    // logo data
    static const uint8_t PROGMEM CPRacingLogo[];

    // has there been a CAN message since the last update
    volatile bool bRxCANSinceLastUpdate = false;

    // handle on the mob configured for outgoing messages
    CANRaw::CAN_MOB CanTxMobHandle = CANRaw::CAN_MOB::MOB_NONE;

};



#endif /* DASHBOARD2_SCREEN_H_ */
