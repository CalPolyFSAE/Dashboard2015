
#include <stdint.h>

#include "AVRLibrary/arduino/Arduino.h"

#include "Screen.h"
#include "Subsystem.h"
#include "Input.h"
#include "CANLib.h"
#include "ADCManager.h"
#include "DashPages/FE/FEDash.h"
#include "DashPages/CC/CCDash.h"
#include "Delegate.h"
#include "Dashboard2.h"

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

ISR(ADC_vect) {
    Subsystem::StaticClass<ADCManager>().INT_ADCFinished();
}

//interrupt for CAN timeout
//63/sec
//SIGNAL(TIMER2_OVF_vect) {

//}


int main() {

    init();// the display library uses delay from Arduino.

    pinConfig();

    timer1_init();

    sei();

    Serial.begin (SERIAL_BAUD);
    Serial.println (FSTR("-----------DASHBOARD--------------"));
#ifdef PRINT_VERSION_INFO
    Serial.print (FSTR("VERSION: "));
    Serial.println (VERSION);
    Serial.println(FSTR(__DATE__));
#endif // PRINT_VERSION_INFO

    uint8_t a = 0;
    while(a < 2)
    {
        for(volatile uint16_t i = 0; i != 0xFFFF; ++i)
        {
            i += 2;
            i -= 2;
            i += 1;
            i -= 1;
        }
        //Serial.println(FSTR("------------"));
        a++;
    }


    CANRaw& CAN = CANRaw::StaticClass();// this is not a subsystem
    CAN.Init(CANRaw::CAN_BAUDRATE::B1M);// set baudrate

    // create Subsystems
#if TEAM==FE
    Screen& screen = Subsystem::StaticClass<FEDash>();
#elif TEAM==FSAE
    Screen& screen = Subsystem::StaticClass<CCDash>();
#endif

    Input& input = Subsystem::StaticClass<Input> ();

    ADCManager& adc = Subsystem::StaticClass<ADCManager>();

    SubsystemControl::StaticClass().InitSubsystems();


    while(true)
    {
        SubsystemControl::StaticClass().MainLoop();
    }

}
