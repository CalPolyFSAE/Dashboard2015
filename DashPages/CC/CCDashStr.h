/*
 * CCDashStr.h
 *
 *  Created on: May 12, 2018
 *      Author: root
 */

#ifndef DASHPAGES_CC_CCDASHSTR_H_
#define DASHPAGES_CC_CCDASHSTR_H_

#include "stdint.h"
#include "stddef.h"
#include "Error.h"

class CCDashStr
{
public:
    static const size_t STATE_MAX_DESC_LENGTH;
    static const size_t ERROR_MAX_MSG_LENGTH;
    static const size_t ERROR_MAX_TYPE_LENGTH;

    enum class ROTARY : uint8_t
    {
        LEFT,
        CENTER,
        RIGHT
    };

    static void getDialStr(ROTARY, uint8_t pos, char[]);

    static void getErrorTypeStr(Error, char[]);
    static void getErrorMsgStr(Error, char[]);

};


#endif /* DASHPAGES_CC_CCDASHSTR_H_ */
