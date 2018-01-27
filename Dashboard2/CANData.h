/*
 * CANData.h
 *
 *  Created on: Jan 15, 2018
 *      Author: root
 */

#ifndef DASHBOARD2_CANDATA_H_
#define DASHBOARD2_CANDATA_H_

#include "Subsystem.h"

#define CANDATAMAX 8

class CANData : public Subsystem<CANData>
{
public:

    //TODO: this class needs to have a var of whatever type the CAN lib uses
    struct CANMSG
    {

    };

    CANData();
    virtual ~CANData() {}

    // CAN rx interrupt call
    void INT_CANRx();

protected:
    //startup
    virtual void Init() override;
    //
    virtual void Update() override;

    //data from CAN bus
    uint8_t data[CANDATAMAX];

protected:
    // does the CAN data need to be read from MOB
    volatile bool bCANNeedRead;
};


#endif /* DASHBOARD2_CANDATA_H_ */
