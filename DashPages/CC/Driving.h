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
#include "Error.h"

class Driving : public DashPage
{
public:
    Driving(FT801IMPL_SPI&, const CCDashConfig::ConvertedInfo&);

    virtual void Begin() override;
    virtual void Draw() override;

    void SetDisplayError(Error);

    void SetDisplayNoCAN(bool);

protected:
    const CCDashConfig::ConvertedInfo& DashInfo;
    FT801IMPL_SPI& LCD;

    Error CurrentError;
    bool bDisplayNoCAN;

    static constexpr char GearText[] = {'0', '1', '2', '3', '4', '5', '/'};
};


#endif /* DASHPAGES_CC_DRIVING_H_ */
