/*
 * Display.h
 *
 *  Created on: Jan 12, 2018
 *      Author: root
 */

#ifndef DASHBOARD2_DISPLAY_H_
#define DASHBOARD2_DISPLAY_H_

#include "Subsystem.h"

// what info gets drawn to the screen
class DrawableScreen
{
public:
    virtual ~DrawableScreen() {}

    virtual void Draw() = 0;
    virtual void UpdateDisplay() = 0;
};

// Display Subsystem
class Display : public Subsystem<Display>
{
public:
    Display();
    virtual ~Display() {}

protected:
    // startup
    virtual void Init() override;
    //
    virtual void Update() override;

private:
};



#endif /* DASHBOARD2_DISPLAY_H_ */
