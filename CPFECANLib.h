/*
 * CPFECANLib.h
 *
 *  Created on: Apr 15, 2015
 *      Author: thomaswillson
 */

#ifndef CPFECANLIB_H_
#define CPFECANLIB_H_

#include <stdint.h>
#include <stdlib.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include "arduino/Arduino.h"

#define BIT0 0x01
#define BIT1 0x02
#define BIT2 0x04
#define BIT3 0x08
#define BIT4 0x10
#define BIT5 0x20
#define BIT6 0x40
#define BIT7 0x80

class CPFECANLib {
public:
   typedef union {
      uint32_t extended;
      uint16_t standard;
   } MSG_ID;

   typedef struct {
      MSG_ID identifier;
      uint8_t dlc;
      uint8_t rtr;
      uint8_t ide;
      uint8_t* data;
   } MSG;

   enum struct CHANNEL_STATE {
      ENABLED, STANDBY, LISTENING
   };

   enum struct MOB_MODE {
      DISABLED, TX, RX, AUTOMATIC_REPLY, FRAME_BUFFER_RX
   };

   enum struct ERROR_TYPE {
      BIT_ERROR, STUFF_ERROR, CRC_ERROR, FORM_ERROR, ACK_ERROR
   };

   enum struct CAN_BAUDRATE {
      B1M, B500K, B250K, B125K
   };

   typedef void (*CAN_MSG_RX)(MSG *msg, uint8_t mobNumber);

   static CAN_MSG_RX rxIntFunc;

   static void init(CAN_BAUDRATE baud, CAN_MSG_RX func) {
      rxIntFunc = func;

      CANGCON = (1 << SWRES);

      // MOb0..7 Interrupts: MOb0: Off, MOb1: Off, MOb2: Off, MOb3: Off, MOb4: Off, MOb5: Off, MOb6: Off, MOb7: Off
      CANIE2 = 0x00;
      // MOb8..14 Interrupts: MOb8: Off, MOb9: Off, MOb10: Off, MOb11: Off, MOb12: Off, MOb13: Off, MOb14: Off
      CANIE1 = 0x00;
      // CAN Interrupts:
      // Timer Overrun: Off
      // General Errors: Off
      // Frame Buffer: Off
      // MOb Errors: Off
      // Transmit: Off
      // Receive: On
      // Bus Off: Off
      // All, except Timer Overrun: Off
      CANGIE = (1 << ENRX) | (1 << ENIT);
      // Highest Interrupt Priority: MOb0
      CANHPMOB = 0x00;

      switch (baud) {
      case CAN_BAUDRATE::B1M:
         CANBT1 = 0x00;
         CANBT2 = 0x0C;
         CANBT3 = 0x36;
         break;
      case CAN_BAUDRATE::B500K:
         CANBT1 = 0x02;
         CANBT2 = 0x0C;
         CANBT3 = 0x37;
         break;
      case CAN_BAUDRATE::B250K:
         CANBT1 = 0x06;
         CANBT2 = 0x0C;
         CANBT3 = 0x37;
         break;
      case CAN_BAUDRATE::B125K:
         CANBT1 = 0x0E;
         CANBT2 = 0x0C;
         CANBT3 = 0x37;
         break;
      }
      // CAN Timer Clock Period: 1.000 us
      CANTCON = 0x00;
      CANGCON |= (1 << ENASTB);

      for (int i = 0; i < 16; ++i) {
         CANPAGE = i << 4;
         CANCDMOB = 0x00;
         CANSTMOB = 0x00;
      }

      sei();
   }

   static void disableAllMOB() {
      for (int i = 0; i < 14; i++) {
         setMOB(i);
         CANCDMOB = 0x00;
      }
   }
   static void disableMOB(uint8_t n) {
      setMOB(n);

      if (n < 8) {
         CANIE2 &= ~(1 << n);
      } else {
         CANIE1 &= !(1 << (n - 8));
      }

      CANCDMOB = 0x00;
   }
   static void sendMsgUsingMOB(uint8_t n, MSG *msg) {
      setMOB(n);
      setID(msg->ide, msg->identifier);
      CANCDMOB = (msg->dlc & 0x0F);
      CANIDT4 |= (msg->rtr && 1) << RTRTAG;
      CANSTMOB = 0x00;

      for (int i = 0; i < 8; i++) {
         CANMSG = msg->data[i];
      }

      CANCDMOB |= (1 << CONMOB0) + (((msg->ide) && 1) << 4); //Must happen last
   }
   static void enableMOBAsRX(uint8_t n, const MSG *msg, const MSG *mask,
      bool interruptMode) {
      setMOB(n);

      CANSTMOB = 0x00;

      if (!interruptMode) {
         if (n < 8) {
            CANIE2 |= 1 << n;
         } else {
            CANIE1 |= 1 << (n - 8);
         }

         setID(msg->ide, msg->identifier);
         CANIDT4 |= (msg->rtr && 1) << RTRTAG;

         setMask(msg->ide, mask->identifier);
         CANIDM4 |= ((mask->rtr && 1) << RTRMSK) + ((mask->ide && 1) << IDEMSK);
      }

      CANCDMOB = (1 << CONMOB1) | (((msg->ide) && 1) << 4) | (msg->dlc & 0x0F); //Must happen last
   }
   static void enableMOBAsRX_PROGMEM(uint8_t n, const MSG *msgFLASH,
      const MSG *maskFLASH, bool interruptMode) {
      MSG msg;
      MSG mask;

      memcpy_P((void *) &msg, msgFLASH, sizeof(MSG));

      if (!interruptMode)
         memcpy_P((void *) &mask, maskFLASH, sizeof(MSG));

      enableMOBAsRX(n, &msg, &mask, interruptMode);
   }
   static void enableMOBAsFrameBuffer(uint8_t n, MSG *msg) {
      //TODO: Implement Frame Buffer Mode
   }

   static void rxInt() {
      MSG msg;
      uint8_t msgData[8];
      msg.data = msgData;

      for (int i = 0; i < 8; i++) {
         msg.data[i] = CANMSG;
      }
      msg.dlc = CANCDMOB & 0x0F;
      msg.ide = (CANCDMOB & (1 << IDE)) && 1;
      msg.rtr = (CANIDT4 & (1 << RTRTAG)) && 1;
      if (!msg.ide) {
         msg.identifier.standard = (CANIDT1 << 3) + (CANIDT2 >> 5);
      } else {
         msg.identifier.extended = ((uint32_t) CANIDT1 << 21) + (CANIDT2 << 13)
            + (CANIDT3 << 5) + (CANIDT4 >> 3);
      }

      rxIntFunc(&msg, CANPAGE);
   }

private:
   static void setMOB(uint8_t MOB) {
      CANPAGE = MOB << 4;
   }

   static void setMask(bool ide, MSG_ID id) {
      if (!ide) {
         CANIDM1 = (id.standard >> 3) & 0xFF;
         CANIDM2 = (id.standard << 5) & 0xFF;
         CANIDM3 = 0x00;
         CANIDM4 = 0x00;
      } else {
         CANIDM1 = (id.extended >> 21) & 0xFF;
         CANIDM2 = (id.extended >> 13) & 0xFF;
         CANIDM3 = (id.extended >> 5) & 0xFF;
         CANIDM4 = (id.extended << 3) & 0xF8;
      }
   }
   static void setID(bool ide, MSG_ID id) {
      if (!ide) {
         CANIDT1 = (id.standard >> 3) & 0xFF;
         CANIDT2 = (id.standard << 5) & 0xFF;
         CANIDT3 = 0x00;
         CANIDT4 = 0x00;
      } else {
         CANIDT1 = (id.extended >> 21) & 0xFF;
         CANIDT2 = (id.extended >> 13) & 0xFF;
         CANIDT3 = (id.extended >> 5) & 0xFF;
         CANIDT4 = (id.extended << 3) & 0xF8;
      }
   }
};

#endif /* CPFECANLIB_H_ */
