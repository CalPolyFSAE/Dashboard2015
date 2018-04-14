
#include <stdint.h>
#include <string.h>

#include "AVRLibrary/arduino/Arduino.h"

#include "Screen.h"
#include "Subsystem.h"
#include "Input.h"
#include "CANLib.h"
#include "FEDash.h"

#include "Delegate.h"

#define TEAM FE //Other: FSAE

static void pinConfig() { // run by main to configure pins
    //Init Digital Inputs for Buttons
    DDRC &= ~(DDC0 | DDC1); //Set direction
    PORTC |= (PC0 | PC1); //Enable pullups

    // output for LEDs
    DDRB |= (DDB5 | DDB6 | DDB7); // set up as outputs
    PORTB &= ~(PB5 | PB6 | PB7); // set outputs low

    // ADC inputs
    DDRF = 0x00;
}

static void timer1_init() {
    TCCR1B |= (1 << WGM12) | (1 << CS11); //set timer 1 to CTC mode and prescaler 8
    TCNT1 = 0; //initialize counter
    OCR1A = 1999; //This sets interrupt to 1000Hz: [((16MHz/8)(0.001s))-1]
    TIMSK1 |= (1 << OCIE1A); //enable compare interrupt
}

//interrupt function for timing
ISR(TIMER1_COMPA_vect) {
    SubsystemControl::StaticClass().INT_CALL_TickAllTC();
}

//interrupt for CAN timeout
//63/sec
//SIGNAL(TIMER2_OVF_vect) {

//}


int main() {

    Serial.begin (SERIAL_BAUD);
    Serial.println (FSTR("DASHBOARD"));
#ifdef PRINT_VERSION_INFO
    Serial.print (FSTR("VERSION: "));
    Serial.println (VERSION);
    Serial.println(FSTR(__DATE__));
#endif // PRINT_VERSION_INFO

    cli();

    pinConfig();

    timer1_init();

    CANRaw& CAN = CANRaw::StaticClass();// this is not a subsystem
    CAN.Init(CANRaw::CAN_BAUDRATE::B1M);// set baudrate

    // create Subsystems
#if TEAM==FE
    Screen& screen = FEDash::StaticClass();
#elif TEAM==FSAE
    Screen& screen = FEDash::StaticClass();
#endif

    Input& input = Input::StaticClass ();

    SubsystemControl::StaticClass().InitSubsystems();

    sei();

    while(1)
    {
        SubsystemControl::StaticClass().MainLoop();
    }
}
