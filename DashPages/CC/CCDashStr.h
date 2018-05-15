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

class CCDashStr
{
public:
    static const size_t STATE_MAX_DESC_LENGTH;

    enum class ROTARY : uint8_t
    {
        LEFT,
        CENTER,
        RIGHT
    };

    static void getDialStr(ROTARY, uint8_t pos, char[]);

};


#endif /* DASHPAGES_CC_CCDASHSTR_H_ */
