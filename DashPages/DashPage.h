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
    struct Color
    {
        uint8_t r,g,b;
        Color() :
            Color(0,0,0)
        {}
        Color(uint8_t r, uint8_t g, uint8_t b):
            r(r), g(g), b(b)
        {}
    };
    const Color RED = Color(255, 0, 0);
    const Color YELLOW = Color(255, 255, 0);
    const Color BLUE = Color(0, 76, 255);
    const Color GREEN = Color(0, 255, 0);
    const Color WHITE = Color(255, 255, 255);
    const Color BLACK = Color(0, 0, 0);

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

    inline static float motecToFloat( uint16_t value, float scaler = 100.0 ) {
        return (float) swap (value) / scaler;
    }
};


#endif /* DASHPAGES_DASHPAGE_H_ */
