/*
 * FEDash.h
 *
 *  Created on: Apr 11, 2018
 *      Author: root
 */

#ifndef FEDASH_H_
#define FEDASH_H_

#include "Screen.h"


class FEDash : public Screen
{
public:
    friend class Subsystem<FEDash>;// should get rid of this type of stuff

protected:
    // current dash page
    uint8_t currentPage = 0;
    // handle on the mob configured for outgoing messages
    CANRaw::CAN_MOB CanTxMobHandle = CANRaw::CAN_MOB::MOB_NONE;
    // handle on the mob configured for outgoing messages
    CANRaw::CAN_MOB CanRxMobHandle = CANRaw::CAN_MOB::MOB_NONE;

    FEDash() {}

    // startup
    virtual void Init() override;
    //
    void Update(uint8_t);

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


    // every 12 ms?
    void sendSwitchPositions(uint8_t);
};

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


#endif /* FEDASH_H_ */
