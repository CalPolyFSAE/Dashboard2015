/*
 * Display.h
 *
 *  Created on: Jan 12, 2018
 *      Author: root
 */

#ifndef DASHBOARD2_SCREEN_H_
#define DASHBOARD2_SCREEN_H_

#include "Subsystem.h"

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
class Screen : public Subsystem<Screen>
{
public:
    Screen();
    virtual ~Screen() {}

protected:

    class DrawableScreen* screens[];

    // startup
    virtual void Init() override;
    //
    virtual void Update() override;

private:
};



#endif /* DASHBOARD2_SCREEN_H_ */
