/*
 * Charging.cpp
 *
 *  Created on: Apr 18, 2018
 *      Author: root
 */

#include "Charging.h"
#include "../../CANLib.h"
//#include "AVRLibrary/arduino/float16.hpp"

struct ChargingDashData
{
    uint16_t val0;
    uint16_t val1;
    uint16_t val2;
    uint16_t val3;

};

Charging::Charging(class FT801IMPL_SPI& LCD, class FEDashDataPack& data)
{
    this->LCD = &LCD;
}

void Charging::Begin()
{

}

void Charging::Draw()
{
    float TCellMax, TCellMin, VCellMax, VCellMin, TCellMean, VCellMean, VTotal,
            VChargerSetpoint, IChargerSetpoint, VChargerActual, IChargerActual;
/*
    float16::toFloat32 (&TCellMax, swap (dashCAN1->charging.TCellMax));
    float16::toFloat32 (&TCellMin, swap (dashCAN1->charging.TCellMin));
    float16::toFloat32 (&VCellMax, swap (dashCAN1->charging.VCellMax));
    float16::toFloat32 (&VCellMin, swap (dashCAN1->charging.VCellMin));

    float16::toFloat32 (&TCellMean, swap (dashCAN2->charging.TCellMean));
    float16::toFloat32 (&VCellMean, swap (dashCAN2->charging.VCellMean));
    float16::toFloat32 (&VTotal, swap (dashCAN2->charging.VTotal));

    float16::toFloat32 (&VChargerSetpoint,
                        swap (dashCAN3->charging.VChargerSetpoint));
    float16::toFloat32 (&IChargerSetpoint,
                        swap (dashCAN3->charging.IChargerSetpoint));
    float16::toFloat32 (&VChargerActual,
                        swap (dashCAN3->charging.VChargerActual));
    float16::toFloat32 (&IChargerActual,
                        swap (dashCAN3->charging.IChargerActual));

    char BMSChargingState[BMS_CHARGING_STATE_MAX_LENGTH];
    char BMSChargingError[BMS_CHARGING_ERROR_MAX_LENGTH];

    strncpy_P (
            BMSChargingState,
            (PGM_P) pgm_read_word (
                    &(BMSChargingStateStringTable[(uint8_t) dashCAN2->charging.chargingState])),
            BMS_CHARGING_STATE_MAX_LENGTH);
    strncpy_P (
            BMSChargingError,
            (PGM_P) pgm_read_word (
                    &(BMSChargingErrorStringTable[(uint8_t) dashCAN2->charging.chargeError])),
            BMS_CHARGING_ERROR_MAX_LENGTH);

    LCD.DLStart ();

    LCD.ClearColorRGB (0x000000);
    LCD.Clear (1, 1, 1);

    LCD.ColorRGB (0x00, 0xFF, 0xFF);
    LCD.PrintText (5, 0, 28, 0, "TCellMax: %.2f", TCellMax);
    LCD.PrintText (5, 25, 28, 0, "TCellMin: %.2f", TCellMin);
    LCD.PrintText (5, 50, 28, 0, "TCellMean: %.2f", TCellMean);
    LCD.PrintText (5, 75, 28, 0, "VCellMax: %.2f", VCellMax);
    LCD.PrintText (5, 100, 28, 0, "VCellMin: %.2f", VCellMin);
    LCD.PrintText (5, 125, 28, 0, "VCellMean: %.2f", VCellMean);
    LCD.PrintText (5, 150, 28, 0, "VTotal: %.2f", VTotal);
    LCD.PrintText (5, 175, 28, 0, "Charging State: %s", BMSChargingState);
    LCD.PrintText (5, 200, 28, 0, "Last Charging Error: %s", BMSChargingError);
    LCD.PrintText (190, 0, 28, 0, "VChargerSetpoint: %.2f", VChargerSetpoint);
    LCD.PrintText (190, 25, 28, 0, "IChargerSetpoint: %.2f", IChargerSetpoint);
    LCD.PrintText (190, 50, 28, 0, "VChargerActual: %.2f", VChargerActual);
    LCD.PrintText (190, 75, 28, 0, "IChargerActual: %.2f", IChargerActual);
    LCD.PrintText (190, 100, 28, 0, "bChargerConnected: %s",
                   dashCAN4->charging.bChargerConnected ? "Yes" : "No");

    LCD.DLEnd ();
    LCD.Finish ();*/
}
