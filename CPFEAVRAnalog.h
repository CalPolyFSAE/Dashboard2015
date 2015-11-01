/*
 * CPFEAVRAnalog.h
 *
 *  Created on: Jun 1, 2015
 *      Author: thomaswillson
 */

#ifndef CPFEAVRANALOG_H_
#define CPFEAVRANALOG_H_

#include <stdint.h>
#include <avr/io.h>
#include "arduino/Arduino.h"

class CPFEAVRAnalog {

public:
   typedef void (*ConversionHandler)(uint16_t result, void *info);
   uint8_t analogPinNumber;

   CPFEAVRAnalog() {
      this->analogPinNumber = 0;
      handler = nullptr;
      handlerInfo = nullptr;
      currentConversion = nullptr;

      ADCSRA = (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0); // Set ADC prescaler to 128 - 125KHz sample rate @ 16MHz
      ADMUX = (1 << REFS0);
      // Set ADC reference to AVCC
      ADCSRA |= (1 << ADEN) | (1 << ADIE); // Enable ADC, ADC Interrupt Enable
   }

   CPFEAVRAnalog(uint8_t analogPinNumber) {
      this->analogPinNumber = analogPinNumber;
      handler = nullptr;
      handlerInfo = nullptr;
      currentConversion = nullptr;
      ADMUX = (1 << REFS0);
      ADCSRA = (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0); // Set ADC prescaler to 128 - 125KHz sample rate @ 16MHz
      ADCSRA |= (1 << ADEN) | (1 << ADIE); // Enable ADC, ADC Interrupt Enable
   }

   void setPinNumber(uint8_t analogPinNumber) {
      this->analogPinNumber = analogPinNumber;
   }

   void startConversion(ConversionHandler handler, void *handlerInfo) {
      if (adcAvailable()) {
         this->handler = handler;
         this->handlerInfo = handlerInfo;
         currentConversion = this;

         ADMUX = (1 << REFS0) | (analogPinNumber & 0xF); // Set ADC reference to AVCC
         ADCSRA |= (1 << ADSC);  // Start A2D Conversions
      }
   }

   static bool adcAvailable() {
      return currentConversion ? false : true;
   }

   static void adcInterruptHandler() {
      if (!adcAvailable()) {
         uint16_t result;
         result = ADCL;
         result |= ADCH << 8;

         (*currentConversion->handler)(result, currentConversion->handlerInfo);
         currentConversion = nullptr;
      }
   }

private:
   ConversionHandler handler;
   void *handlerInfo;

   static CPFEAVRAnalog *currentConversion;
};

#endif /* CPFEAVRANALOG_H_ */
