/*
 * Debug.cpp
 *
 *  Created on: Apr 22, 2018
 *      Author: root
 */

#include "Debug.h"
#include "AVRLibrary/arduino/Arduino.h"

//used to output message
static void Debug::LogMessageln( Error& err ) {

}

static void Debug::LogMessageln( const __FlashStringHelper* c) {
    Serial.println(c);
}

static void Debug::LogMessageln( uint8_t num ) {
    Serial.println(num);
}

static void Debug::LogMessage( Error& err ) {

}

static void Debug::LogMessage( const __FlashStringHelper* c) {
    Serial.println(c);
}

static void Debug::LogMessage( uint8_t num ) {
    Serial.println(num);
}

static void Debug::LogDebugMessage( const __FlashStringHelper* msg , const __FlashStringHelper* file, const __FlashStringHelper* line) {
    Serial.print(FSTR("[DEBUG]: "));
    Serial.print(FSTR("{ "));
    Serial.print(file);
    Serial.print(FSTR(" "));
    Serial.print(line);
    Serial.print(FSTR(" }: "));
    Serial.println(msg);
}


