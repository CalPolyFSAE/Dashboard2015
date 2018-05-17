/*
 * ErrorConditions.h
 *
 *  Created on: May 14, 2018
 *      Author: root
 */

#ifndef DASHPAGES_CC_ERRORCONDITIONS_H_
#define DASHPAGES_CC_ERRORCONDITIONS_H_

#include "stdint.h"
#include "CCDashConfig.h"
#include "Error.h"


//TODO: this error system is awful
class ErrorConditions
{
public:
    ErrorConditions(const CCDashConfig::ConvertedInfo& info);

    // returns the most severe error
    Error GetPriorityError();
    // cycles current error
    Error CycleError();
    // gets the current error
    Error GetCurrentError();

protected:

    struct ErrorMonitor
    {
        bool bIsSet;
        bool bHasBeenAck;
        uint8_t ticksSinceAck;
    };

    uint8_t currentErrorIndex;
    ErrorMonitor monitors[Error::NUM_ERROR_TYPES];

    const CCDashConfig::ConvertedInfo& DashInfo;

    // periodically checks for new errors
    void Refresh(uint8_t);

    void SetError(Error);
};

#endif /* DASHPAGES_CC_ERRORCONDITIONS_H_ */
