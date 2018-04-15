/*
 * FEDash.h
 *
 *  Created on: Apr 11, 2018
 *      Author: root
 */

#ifndef FEDASH_H_
#define FEDASH_H_

#include "Screen.h"

// CAN message format used for incoming display data
constexpr CANRaw::CAN_FRAME_HEADER DashDisplayCANMsg =
    {
        0x00,             // id
        0,                // rtr
        0,                // ide
        8                 // dlc
    };
constexpr CANRaw::CAN_FRAME_MASK DashDisplayCANMsgMsk =
    {
        0xFFFF,           // id
        true,             // rtr
        true              // ide
    };

class FEDash : public Screen
{
public:
    friend class Subsystem;// should get rid of this type of stuff

protected:

    FEDash() {}

    // startup
    virtual void Init() override;
    //
    void Update(uint8_t);

    // called on Can rx for Mob. Get received data with GetCANData()
    // dlc is the data length code of the received message. It may be different
    // than the one set in FrameData
    virtual void INT_Call_GotFrame( const CANRaw::CAN_FRAME_HEADER& FrameData,
                                    const CANRaw::CAN_DATA& Data ) override;

    //events
    virtual void OnNoCANData() override;

    // used for input callback
    // rotary sw
    void OnRotaryInputChange0(uint8_t);
    void OnRotaryInputChange1(uint8_t);
    void OnRotaryInputChange2(uint8_t);
    // buttons
    void OnButtonInputChange0(uint8_t);
    void OnButtonInputChange1(uint8_t);

    // current dash page
    uint8_t currentPage = 0;

    // handle on the mob configured for outgoing messages
    CANRaw::CAN_MOB CanRxMobHandle = CANRaw::CAN_MOB::MOB_NONE;

    // data from the last CAN frame
    volatile CANRaw::CAN_FRAME_HEADER header;
    volatile CANRaw::CAN_DATA data;
};


#endif /* FEDASH_H_ */
