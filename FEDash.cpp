/*
 * FEDash.cpp
 *
 *  Created on: Apr 11, 2018
 *      Author: root
 */

#include "FEDash.h"


void FEDash::Init()
{
    Screen::Init();
}
//
void FEDash::Update(uint8_t)
{
    Screen::Update(0);
}

// used for input OnChange callback
void FEDash::OnRotaryInputChange(int8_t)
{

}
// used for input OnChange callback
void FEDash::OnButtonInputChange(int8_t)
{

}
