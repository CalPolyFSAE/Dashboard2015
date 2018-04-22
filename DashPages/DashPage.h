/*
 * DashPage.h
 *
 *  Created on: Apr 14, 2018
 *      Author: root
 */

#ifndef DASHPAGES_DASHPAGE_H_
#define DASHPAGES_DASHPAGE_H_

#include "stdint.h"

// what info gets drawn to the screen
class DashPage
{
public:
    virtual ~DashPage() {}

    virtual void Begin() = 0;
    virtual void Draw() = 0;

    inline static float swap( float d ) {
        float a;
        unsigned char *dst = (unsigned char *) &a;
        unsigned char *src = (unsigned char *) &d;
        dst[0] = src[3];
        dst[1] = src[2];
        dst[2] = src[1];
        dst[3] = src[0];
        return a;
    }

    inline static uint32_t swap( uint32_t d ) {
        uint32_t a;
        unsigned char *dst = (unsigned char *) &a;
        unsigned char *src = (unsigned char *) &d;
        dst[0] = src[3];
        dst[1] = src[2];
        dst[2] = src[1];
        dst[3] = src[0];
        return a;
    }

    inline static uint16_t swap( uint16_t d ) {
        uint16_t a;
        unsigned char *dst = (unsigned char *) &a;
        unsigned char *src = (unsigned char *) &d;
        dst[0] = src[1];
        dst[1] = src[0];
        return a;
    }
};


#endif /* DASHPAGES_DASHPAGE_H_ */
