/*
 * Debug.h
 *
 *  Created on: Apr 22, 2018
 *      Author: root
 */

#ifndef SERIALPRINT_DEBUG_H_
#define SERIALPRINT_DEBUG_H_

#include "stdint.h"
#include "avr/pgmspace.h"
#include "SerialError.h"

// a string helper that replaces the Arduino F macro and maintains const
class __FlashStringHelper;
#define FSTR(string_literal) (reinterpret_cast<const __FlashStringHelper*>(PSTR(string_literal)))

#define DEBUG_PRINT(msg) {Debug::LogDebugMessage(FSTR(msg), FSTR(__FILE__), FSTR(__LINE__));}

class Debug
{
public:
    //used to output message
    static void LogMessageln( SerialError& err );
    static void LogMessageln( const __FlashStringHelper* c);
    static void LogMessageln( uint8_t num );

    static void LogMessage( SerialError& err );
    static void LogMessage( const __FlashStringHelper* c);
    static void LogMessage( uint8_t num );

    static void LogDebugMessage(const __FlashStringHelper* msg , const __FlashStringHelper* file, const __FlashStringHelper* line);
};


#endif /* SERIALPRINT_DEBUG_H_ */
