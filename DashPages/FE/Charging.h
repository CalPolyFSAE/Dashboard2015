/*
 * Charging.h
 *
 *  Created on: Apr 14, 2018
 *      Author: root
 */

#ifndef DASHPAGES_CHARGING_H_
#define DASHPAGES_CHARGING_H_

#include "../DashPage.h"

class Charging : public DashPage
{
public:
    Charging(class FT801IMPL_SPI&, class FEDashDataPack& data);

    virtual void Begin() override;
    virtual void Draw() override;

protected:
    class FT801IMPL_SPI* LCD;
};

#endif /* DASHPAGES_CHARGING_H_ */
