/*
 * Input.h
 *
 *  Created on: Mar 29, 2018
 *      Author: root
 */

#ifndef DASHBOARD2_INPUT_H_
#define DASHBOARD2_INPUT_H_

#include "Subsystem.h"
#include "ADCManager.h"

class Input : public Subsystem<Input>, ADCManagerCallbackInterface
{
public:

    virtual void INT_Call_ADC_Finished(const uint16_t& value, uint8_t channel) override;// start next ADC read

protected:

    // TODO add list of ADC channels to read and corresponding input channel

    virtual void Update() override;// start ADC read
    virtual void Init() override;

};


#endif /* DASHBOARD2_INPUT_H_ */
