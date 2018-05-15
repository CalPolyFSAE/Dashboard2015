/*
 * Driving.h
 *
 *  Created on: May 13, 2018
 *      Author: root
 */

#ifndef DASHPAGES_CC_DRIVING_H_
#define DASHPAGES_CC_DRIVING_H_

#include "../DashPage.h"
#include "CCDashConfig.h"
#include "../../FTDI _V1/FT_VM801P43_50.h"

class Driving : DashPage
{
public:
    Driving(FT801IMPL_SPI&, const CCDashConfig::DashInfo&);

    virtual void Begin() override;
    virtual void Draw() override;

protected:
    const CCDashConfig::DashInfo& DashInfo;
    FT801IMPL_SPI& LCD;
};


#endif /* DASHPAGES_CC_DRIVING_H_ */
