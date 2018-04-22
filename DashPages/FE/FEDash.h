/*
 * FEDash.h
 *
 *  Created on: Apr 11, 2018
 *      Author: root
 */

#ifndef DASHPAGES_FE_FEDASH_H_
#define DASHPAGES_FE_FEDASH_H_

#include "../../Screen.h"
#include "FEDashConfig.h"
#include "../DashTypes.h"

#define NUMPAGES 5

class FEDash : public Screen
{
public:
    friend class Subsystem;// should get rid of this type of stuff

protected:

    FEDash();

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

    // add next page to list
    void AddNextPage(class DashPage*);

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

    // data from the last CAN frame
    volatile DashTypes::DashData CANData;
    DashTypes::DashData CANDataS;

    // dashPages
    class DashPage* pages[NUMPAGES];
};


#endif /* DASHPAGES_FE_FEDASH_H_ */
