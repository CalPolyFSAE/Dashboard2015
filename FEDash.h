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
    FEDash() {}

    // startup
    virtual void Init() override;
    //
    virtual void Update(uint8_t) override;

    // used for input OnChange callback
    virtual void OnRotaryInputChange(int8_t) override;
    // used for input OnChange callback
    virtual void OnButtonInputChange(int8_t) override;
};



#endif /* FEDASH_H_ */
