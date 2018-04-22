/*
 * Charging.h
 *
 *  Created on: Apr 14, 2018
 *      Author: root
 */

#ifndef DASHPAGES_CHARGING_H_
#define DASHPAGES_CHARGING_H_

#include "../DashPage.h"
#include "../DashTypes.h"
#include "../../FTDI _V1/FT_VM801P43_50.h"

class Charging : public DashPage
{
public:
    Charging(FT801IMPL_SPI&, const DashTypes::DashData& data);

    virtual void Begin() override;
    virtual void Draw() override;

    struct ChargingDashDataLayout;

protected:
    FT801IMPL_SPI* LCD;
    const ChargingDashDataLayout* data;
};

#endif /* DASHPAGES_CHARGING_H_ */
