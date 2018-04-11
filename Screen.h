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

// what info gets drawn to the screen
class DrawableScreen
{
public:
    virtual ~DrawableScreen() {}

    virtual void Construct() = 0;
    virtual void Update() = 0;
};

// Display Subsystem
// logic differers between cars
class Screen : public Subsystem<Screen>, CANListener
{
public:
    friend class Subsystem<Screen>;

protected:
    Screen();

    class DrawableScreen* screens[];

    // startup
    virtual void Init() override;
    //
    virtual void Update(uint8_t) override;

    // used for input OnChange callback
    virtual void OnRotaryInputChange(int8_t) {}
    // used for input OnChange callback
    virtual void OnButtonInputChange(int8_t) {}

private:

    // called on Can rx for Mob. Get received data with GetCANData()
    // dlc is the data length code of the received message. It may be different
    // than the one set in FrameData
    virtual void INT_Call_GotFrame( const CANRaw::CAN_FRAME_HEADER& FrameData,
                                    const CANRaw::CAN_DATA& Data ) override;

    // initial handlers of the Input event
    void OnInputADC(uint8_t);
    void OnInputTOG(uint8_t);

    // data from the last CAN frame
    volatile CANRaw::CAN_FRAME_HEADER header;
    volatile CANRaw::CAN_DATA data;

};



#endif /* DASHBOARD2_SCREEN_H_ */
