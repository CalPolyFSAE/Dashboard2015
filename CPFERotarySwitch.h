#include <avr/interrupt.h>
#include <util/atomic.h>

#include <AVRLibrary/arduino/Arduino.h>
#include <AVRLibrary/CPFEAVRAnalog.h>

//handling function for ADC results
void rotarySwitchResultHandler( uint16_t result, void *info );

class CPFERotarySwitch
{
public:
    static constexpr uint8_t NUM_ROTARYS = 3;

    enum class RotarySwitches
        : uint8_t
        {
            YELLOW, RED, BLACK
    };
private:

    static uint8_t positionCount;
    static volatile int8_t currentRotaryADCConversion;

    static constexpr uint8_t RED_ADC = 1;
    static constexpr uint8_t YELLOW_ADC = 0;
    static constexpr uint8_t BLACK_ADC = 2;

    static CPFEAVRAnalog analogPins[NUM_ROTARYS];
    static volatile uint8_t positions[NUM_ROTARYS];

public:

    static void init( uint8_t positionCount ) {
        CPFERotarySwitch::positionCount = positionCount;
        currentRotaryADCConversion = -1;

        for (int i = 0; i < NUM_ROTARYS; ++i)
        {
            analogPins[i].analogPinNumber = i;
        }
    }

    static void requestUpdatedPositions() {
        //working with variable that is used in an interrupt routine
        ATOMIC_BLOCK(ATOMIC_FORCEON)
        {
            if (currentRotaryADCConversion < 0)
            { // If previous set of conversions finished, start a new round
                currentRotaryADCConversion = 0;
            }
        }
    }

    static uint8_t getPosition( RotarySwitches rotarySwitch ) {
        return positions[(uint8_t) rotarySwitch];
    }

    static void runTasks() { // Start any requested conversions.
        if (currentRotaryADCConversion >= 0)
        {
            while (!CPFEAVRAnalog::adcAvailable ())
                // Wait for conversion in progress to finish
                ;
            analogPins[currentRotaryADCConversion].startConversion (
                    &rotarySwitchResultHandler, nullptr);
        }
    }

    static uint8_t calculatePositionFromADCResult( uint16_t result ) {
        uint8_t position = (uint8_t) (positionCount
                - (float) result / (1024.0 / (float) positionCount));

        if (position > positionCount - 1)
        {
            position = positionCount - 1;
        }

        return position;
    }

    static void resultHandler( uint16_t result, void *info ) {
        if (currentRotaryADCConversion >= 0)
        {
            positions[currentRotaryADCConversion] =
                    calculatePositionFromADCResult (result);

            if (currentRotaryADCConversion < NUM_ROTARYS - 1)
            {
                ++currentRotaryADCConversion;
            }
            else
            {
                currentRotaryADCConversion = -1;
            }
        }
    }
};
