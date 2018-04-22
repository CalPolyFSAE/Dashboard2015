/*
 * Charging.cpp
 *
 *  Created on: Apr 18, 2018
 *      Author: root
 */

#include "Charging.h"
#include "../../CANLib.h"
#include "FEDashStr.h"
#include "../../float16.h"

/*
 * typedef struct DashCAN1Charging {
                uint16_t TCellMin;
                uint16_t TCellMax;
                uint16_t VCellMin;
                uint16_t VCellMax;
        } DashCAN1Charging;

    typedef struct DashCAN2Charging {
            uint16_t TCellMean;
            uint16_t VCellMean;
            uint16_t VTotal;
            eBMSChargingState chargingState;
            eBMSLastChargeError chargeError;
    } DashCAN2Charging;

    typedef struct DashCAN3Charging {
                uint16_t VChargerSetpoint;
                uint16_t IChargerSetpoint;
                uint16_t VChargerActual;
                uint16_t IChargerActual;
        } DashCAN3Charging;

       typedef struct DashCAN4Charging {
                uint8_t bChargerConnected;
        } DashCAN4Charging;
 */

struct Charging::ChargingDashDataLayout
{
    uint16_t TCellMin;// CAN0
    uint16_t TCellMax;
    uint16_t VCellMin;
    uint16_t VCellMax;

    uint16_t TCellMean;// CAN1
    uint16_t VCellMean;
    uint16_t VTotal;
    FEDashStr::eBMSChargingState chargingState;
    FEDashStr::eBMSLastChargeError chargeError;

    uint16_t VChargerSetpoint;// CAN2
    uint16_t IChargerSetpoint;
    uint16_t VChargerActual;
    uint16_t IChargerActual;

    uint8_t bChargerConnected;// CAN3
    uint8_t data12_2;// unused
    uint16_t data13; // unused
    uint16_t data14; // unused
    uint16_t data15; // unused
};

Charging::Charging(FT801IMPL_SPI& LCD, const DashTypes::DashData& data)
{
    this->LCD = &LCD;
    this->data = (ChargingDashDataLayout*)(&data);

    // check that the config structures are the same size
    // assert(sizeof(ChargingDashDataLayout) == sizeof(DashTypes::DashData))
}

void Charging::Begin()
{

}

void Charging::Draw()
{
    float TCellMax, TCellMin, VCellMax, VCellMin, TCellMean, VCellMean, VTotal,
            VChargerSetpoint, IChargerSetpoint, VChargerActual, IChargerActual;

    float16::toFloat32 (&TCellMax, swap (data->TCellMax));
    float16::toFloat32 (&TCellMin, swap (data->TCellMin));
    float16::toFloat32 (&VCellMax, swap (data->VCellMax));
    float16::toFloat32 (&VCellMin, swap (data->VCellMin));

    float16::toFloat32 (&TCellMean, swap (data->TCellMean));
    float16::toFloat32 (&VCellMean, swap (data->VCellMean));
    float16::toFloat32 (&VTotal, swap (data->VTotal));

    float16::toFloat32 (&VChargerSetpoint,
                        swap (data->VChargerSetpoint));
    float16::toFloat32 (&IChargerSetpoint,
                        swap (data->IChargerSetpoint));
    float16::toFloat32 (&VChargerActual,
                        swap (data->VChargerActual));
    float16::toFloat32 (&IChargerActual,
                        swap (data->IChargerActual));

    char BMSChargingState[FEDashStr::BMS_CHARGING_STATE_MAX_LENGTH];
    char BMSChargingError[FEDashStr::BMS_CHARGING_ERROR_MAX_LENGTH];

    strncpy_P (
            BMSChargingState,
            (PGM_P) pgm_read_word (
                    &(BMSChargingStateStringTable[(uint8_t) data->chargingState])),
            BMS_CHARGING_STATE_MAX_LENGTH);
    strncpy_P (
            BMSChargingError,
            (PGM_P) pgm_read_word (
                    &(BMSChargingErrorStringTable[(uint8_t) data->chargeError])),
            BMS_CHARGING_ERROR_MAX_LENGTH);

    LCD->DLStart ();

    LCD->ClearColorRGB (0x000000);
    LCD->Clear (1, 1, 1);

    LCD->ColorRGB (0x00, 0xFF, 0xFF);
    LCD->PrintText (5, 0, 28, 0, "TCellMax: %.2f", TCellMax);
    LCD->PrintText (5, 25, 28, 0, "TCellMin: %.2f", TCellMin);
    LCD->PrintText (5, 50, 28, 0, "TCellMean: %.2f", TCellMean);
    LCD->PrintText (5, 75, 28, 0, "VCellMax: %.2f", VCellMax);
    LCD->PrintText (5, 100, 28, 0, "VCellMin: %.2f", VCellMin);
    LCD->PrintText (5, 125, 28, 0, "VCellMean: %.2f", VCellMean);
    LCD->PrintText (5, 150, 28, 0, "VTotal: %.2f", VTotal);
    LCD->PrintText (5, 175, 28, 0, "Charging State: %s", BMSChargingState);
    LCD->PrintText (5, 200, 28, 0, "Last Charging Error: %s", BMSChargingError);
    LCD->PrintText (190, 0, 28, 0, "VChargerSetpoint: %.2f", VChargerSetpoint);
    LCD->PrintText (190, 25, 28, 0, "IChargerSetpoint: %.2f", IChargerSetpoint);
    LCD->PrintText (190, 50, 28, 0, "VChargerActual: %.2f", VChargerActual);
    LCD->PrintText (190, 75, 28, 0, "IChargerActual: %.2f", IChargerActual);
    LCD->PrintText (190, 100, 28, 0, "bChargerConnected: %s",
                    data->bChargerConnected ? "Yes" : "No");

    LCD->DLEnd ();
    LCD->Finish ();
}
