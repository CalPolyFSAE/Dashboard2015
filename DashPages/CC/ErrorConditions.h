/*
 * ErrorConditions.h
 *
 *  Created on: May 14, 2018
 *      Author: root
 */

#ifndef DASHPAGES_CC_ERRORCONDITIONS_H_
#define DASHPAGES_CC_ERRORCONDITIONS_H_

#include "CCDashConfig.h"
#include "Error.h"

class ErrorConditions
{
public:
    ErrorConditions(const CCDashConfig::DashInfo& info);


    Error GetPriorityError();
    Error CycleError();
    Error GetCurrentError();

protected:
    const CCDashConfig::DashInfo& DashInfo;
    Error Errors[CCDashConfig::ERROR_ARRAY_SIZE];
};

#endif /* DASHPAGES_CC_ERRORCONDITIONS_H_ */
