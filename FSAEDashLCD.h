/*
 * FSAEDashLCD.h
 *
 *  Created on: Mar 2, 2016
 *      Author: Daniel Baron
 */


#ifndef FEDASHLCD_H_
#define FEDASHLCD_H_

#include "DashLCD.h"

static const char PROGMEM WarningMessage_EngineTemperature[] =
"Engine Hot: %.2fC";
static const char PROGMEM WarningMessage_OilTemperature[]=
"Oil Temperature: %.2fC";
static const char PROGMEM WarningMessage_OilPressure[] =
"Oil Pressure: %.2fC";
static const char PROGMEM WarningMessage_BatteryLow[] =
"Battery Low: %.2fC";


static const char PROGMEM RotaryRed1[] = "MC Off";
static const char PROGMEM RotaryRed2[] = "120Nm";
static const char PROGMEM RotaryRed3[] = "120Nm";
static const char PROGMEM RotaryRed4[] = "120Nm";
static const char PROGMEM RotaryRed5[] = "120Nm";
static const char PROGMEM RotaryRed6[] = "Invalid";
static const char PROGMEM RotaryRed7[] = "Invalid";
static const char PROGMEM RotaryRed8[] = "Invalid";
static const char PROGMEM RotaryRedUnused[] = "Invalid";

PGM_P const RotaryRedStringTable[] PROGMEM =
{
	RotaryRed1, RotaryRed2, RotaryRed3, RotaryRed4, RotaryRed5, RotaryRed6, RotaryRed7, RotaryRed8, RotaryRedUnused, RotaryRedUnused, RotaryRedUnused, RotaryRedUnused
};

static const char PROGMEM RotaryYellow1[] = "CLK DIV 8";
static const char PROGMEM RotaryYellow2[] = "CLK DIV 4";
static const char PROGMEM RotaryYellow3[] = "CLK DIV 2";
static const char PROGMEM RotaryYellow4[] = "CLK DIV 0";
static const char PROGMEM RotaryYellow5[] = "Invalid";
static const char PROGMEM RotaryYellow6[] = "Invalid";
static const char PROGMEM RotaryYellow7[] = "Invalid";
static const char PROGMEM RotaryYellow8[] = "Invalid";
static const char PROGMEM RotaryYellowUnused[] = "Invalid";

PGM_P const RotaryYellowStringTable[] PROGMEM = {RotaryYellow1, RotaryYellow2,
	RotaryYellow3, RotaryYellow4, RotaryYellow5, RotaryYellow6, RotaryYellow7,
	RotaryYellow8, RotaryYellowUnused, RotaryYellowUnused, RotaryYellowUnused,
	RotaryYellowUnused};

static const char PROGMEM RotaryBlack1[] = "Driver1";
static const char PROGMEM RotaryBlack2[] = "Driver2";
static const char PROGMEM RotaryBlack3[] = "Pit";
static const char PROGMEM RotaryBlack4[] = "Engine";
static const char PROGMEM RotaryBlack5[] = "Brake";
static const char PROGMEM RotaryBlack6[] = "Suspension";
static const char PROGMEM RotaryBlack7[] = "Track";
static const char PROGMEM RotaryBlack8[] = "Invalid";
static const char PROGMEM RotaryBlackUnused[] = "Invalid";

PGM_P const RotaryBlackStringTable[] PROGMEM =
{
	RotaryBlack1, RotaryBlack2, RotaryBlack3, RotaryBlack4, RotaryBlack5, RotaryBlack6, RotaryBlack7, RotaryBlack8, RotaryBlackUnused, RotaryBlackUnused,RotaryBlackUnused,RotaryBlackUnused
};

static const size_t STATE_MAX_DESC_LENGTH = 30;



class FSAEDashLCD: public DashLCD {
public:
	enum class DashPages
		: uint8_t {
			WaitingForCAN, Driving, LapTrigger, Warning, Systems, Brakes, Electrical, Suspension, Drivetrain, Performance,
	};


	enum class WarningSeverity
		: uint8_t {
			Okay, ShortWarning, LongWarning, ReturnToPits, Error, Danger
	};

	enum class WarningMessage
		: uint8_t {
			EngineTemperature,
			OilTemperature,
			OilPressure,
			BatteryLow

	};


	typedef struct DashCAN1Driving {//0x190
		uint16_t EngineTemp;
		uint16_t RPM;
		uint16_t Gear;
		uint16_t Speed;
	};



	typedef struct DashCAN1Engine {//0x191
		uint16_t Lambda;
		uint16_t OilTemp;
		uint16_t MAP;
		uint16_t ThrottlePOS;
	};

	typedef struct DashCAN2Engine {//0x192
		uint16_t WaterTemp;
	};

	typedef struct DashCAN1Brakes {//0x193
		uint16_t pBrakeF;
		uint16_t pBrakeR;
	};
//Continue here
	typedef struct DashCAN2Brakes {
		uint16_t pBrakeRMax;
		uint16_t rBrakeBalMin;
		uint16_t rBrakeBalmax;
	};

	typedef union DashCAN1 { //0xF0
		uint8_t data[8];
		DashCAN1Driving driving;
		DashCAN1Trig trig;
		DashCAN1Systems systems;
		DashCAN1Brakes brakes;
	};

	typedef union DashCAN2 { //0xF2
		uint8_t data[8];
		DashCAN2Driving driving;
		DashCAN2Systems systems;
		DashCAN2Brakes brakes;
	};

	typedef union DashCAN3 { //0xF3
		uint8_t data[8];
	};

	typedef union DashCAN4 { //0xF4
		uint8_t data[8];
	};

	DashCAN1 *dashCAN1 = (DashCAN1 *) dashCAN1Data;
	DashCAN2 *dashCAN2 = (DashCAN2 *) dashCAN2Data;
	DashCAN3 *dashCAN3 = (DashCAN3 *) dashCAN3Data;
	DashCAN4 *dashCAN4 = (DashCAN4 *) dashCAN4Data;
	WarningCANMessage *warningCAN = (WarningCANMessage *) warningCANData;
	DashPage *dashPage = (DashPage *) &DashboardData.NDashPage;

	virtual ~FSAEDashLCD() {
		return;
	}

	void updateDashboard() {
		dashPage->NDashPage = warningCAN->NDashPage;


		switch (dashPage->dashPage) {
		case DashPages::Brakes:
			brakes();
			break;
		case DashPages::Drivetrain:
			drivetrain();
			break;
		case DashPages::Driving:
			driving();
			break;
		case DashPages::Electrical:
			electrical();
			break;
		case DashPages::LapTrigger:
			lapTrigger();
			break;
		case DashPages::Performance:
			performance();
			break;
		case DashPages::Systems:
			systems();
			break;
		case DashPages::WaitingForCAN:
			waitingForCAN();
			break;
		case DashPages::Warning:
			warning();
			break;
		default:
			waitingForCAN();
			break;
		}
	}

protected:
	PGM_P blackRotaryString(int position) {
		return (PGM_P) pgm_read_word(&(RotaryBlackStringTable[position]));
	}
	PGM_P yellowRotaryString(int position) {
		return (PGM_P) pgm_read_word(&(RotaryYellowStringTable[position]));
	}
	PGM_P redRotaryString(int position) {
		return (PGM_P) pgm_read_word(&(RotaryRedStringTable[position]));
	}

	void brakes() {

	}

	void drivetrain() {

	}

	void electrical() {

	}

	void performance() {

	}

	void systems() {

	}

	void warning() {
		bool displayBoxes;
		const char *severityText;
		uint32_t color = 0x00000;

		switch (warningCAN->warningSeverity) {
		case WarningSeverity::ShortWarning:
		case WarningSeverity::LongWarning:
			displayBoxes = millis() % 500 > 250;
			severityText = "WARNING!";
			color = 0xFFFF00;
			break;
		case WarningSeverity::Error:
			displayBoxes = millis() % 500 > 250;
			severityText = "ERROR!";
			color = 0xFF0000;
			break;
		case WarningSeverity::Danger:
			displayBoxes = true;
			severityText = "DANGER!";
			if (millis() % 600 > 450)
				color = 0xFFFF00;
			else if (millis() % 600 > 300)
				color = 0x00FF00;
			else if (millis() % 600 > 150)
				color = 0x000000;
			else
				color = 0xFF0000;
			break;
		case WarningSeverity::ReturnToPits:
			displayBoxes = true;
			severityText = "Return to Pits";
			color = 0xFFFF00;
			break;
		default:
			displayBoxes = true;
			severityText = "Unknown Severity";
		}

		LCD.DLStart();

		LCD.ClearColorRGB(0xFFFFFF);
		LCD.Clear(1, 1, 1);

		LCD.ColorRGB(0x000000);
		LCD.PrintText(FT_DISPLAYWIDTH / 2, FT_DISPLAYHEIGHT / 4, 31, FT_OPT_CENTER, severityText);
		LCD.PrintTextFlash(FT_DISPLAYWIDTH / 2, FT_DISPLAYHEIGHT - 100, 29, FT_OPT_CENTER, warningMessageToString(warningCAN->warningMessage), warningCAN->associatedValue);

		if (displayBoxes) {
			LCD.ClearColorRGB(color);
			LCD.ScissorSize(75, FT_DISPLAYHEIGHT);
			LCD.ScissorXY(0, 0);
			LCD.Clear(1, 1, 1);
			LCD.ScissorXY(FT_DISPLAYWIDTH - 75, 0);
			LCD.Clear(1, 1, 1);
		}

		LCD.DLEnd();
		LCD.Finish();
	}

	void driving() {
		float TMC, TMotor, TCellMax, rBrakeBalLast, vCar, tCurrentDelta, VCellMin;
		char ShutdownStateDesc[STATE_MAX_DESC_LENGTH];
		char MCControlStateDesc[STATE_MAX_DESC_LENGTH];

		strncpy_P(ShutdownStateDesc, (PGM_P) pgm_read_word(&(ShutdownStateStringTable[(uint8_t) dashCAN2->driving.eShutdownState])), STATE_MAX_DESC_LENGTH);
		strncpy_P(MCControlStateDesc, (PGM_P) pgm_read_word(&(MCStateStringTable[(uint8_t) dashCAN2->driving.eMCControlState])), STATE_MAX_DESC_LENGTH);

		float16::toFloat32(&TMC, swap(dashCAN1->driving.TMC));
		float16::toFloat32(&TMotor, swap(dashCAN1->driving.TMotor));
		float16::toFloat32(&TCellMax, swap(dashCAN1->driving.TCellMax));
		float16::toFloat32(&rBrakeBalLast, swap(dashCAN1->driving.rBrakeBalLast));
		float16::toFloat32(&vCar, swap(dashCAN2->driving.vCar));
		float16::toFloat32(&tCurrentDelta, swap(dashCAN2->driving.tCurrentDelta));
		float16::toFloat32(&VCellMin, swap(dashCAN2->driving.VCellMin));

		LCD.DLStart();

		LCD.ColorRGB(0x00, 0xFF, 0xFF);
		LCD.PrintText(5, 0, 28, 0, "TMC: %.2f", TMC);
		LCD.PrintText(5, 25, 28, 0, "TMotor: %.2f", TMotor);
		LCD.PrintText(5, 50, 28, 0, "TCellMax: %.2f", TCellMax);
		LCD.PrintText(5, 75, 28, 0, "rBrakeBalLast: %.2f", rBrakeBalLast);
		LCD.PrintText(5, 100, 28, 0, "vCar: %.2f", vCar);
		LCD.PrintText(5, 125, 28, 0, "VCellMin: %.2f", VCellMin);
		LCD.PrintText(5, 150, 28, 0, "eMCControlState: %s", MCControlStateDesc);
		LCD.PrintText(5, 175, 28, 0, "eShutdownState: %s", ShutdownStateDesc);

		LCD.ColorRGB(0xFFFFFF);
		LCD.PrintText(5, 200, 30, 0, "%.1f", tCurrentDelta);

		LCD.ColorRGB(0xFFFFFF);
		LCD.Cmd_FGColor(0xFF0000);
		LCD.Cmd_BGColor(0xFF0000);
		LCD.Cmd_Slider(FT_DISPLAYWIDTH - 30, 20, 20, 180, 0, 100 - (uint16_t) TMotor, 100);
		LCD.ColorRGB(0xFF0000);
		LCD.PrintText(FT_DISPLAYWIDTH - 20, 240, 31, FT_OPT_CENTER, "M");

		LCD.ColorRGB(0xFFFFFF);
		LCD.Cmd_FGColor(0xFF0000);
		LCD.Cmd_BGColor(0xFF0000);
		LCD.Cmd_Slider(FT_DISPLAYWIDTH - 90, 20, 20, 180, 0, 100 - (uint16_t) TMC, 100);
		LCD.ColorRGB(0xFF0000);
		LCD.PrintText(FT_DISPLAYWIDTH - 80, 240, 31, FT_OPT_CENTER, "C");

		LCD.DLEnd();
		LCD.Finish();
	}

	void lapTrigger() {
		uint32_t time = dashCAN1->trig.tLastLap;
		int32_t delta = dashCAN1->trig.tLastLapDelta;

		uint8_t lMin = time / 60000;
		time -= lMin * 60000;
		uint8_t lSec = time / 1000;
		time -= lSec * 1000;

		int8_t dMin = delta / 60000;
		delta -= dMin * 60000;
		int8_t dSec = delta / 1000;
		delta -= dSec * 1000;

		LCD.DLStart();
		LCD.ClearColorRGB(0xFFFFFF);
		LCD.Clear(1, 1, 1);

		LCD.ColorRGB(0x000000);
		LCD.PrintText(FT_DISPLAYWIDTH / 2, FT_DISPLAYHEIGHT / 4, 31, FT_OPT_CENTER, "tLastLap: %02d:%02d.%03d", lMin, lSec, time);
		LCD.PrintText(FT_DISPLAYWIDTH / 2, FT_DISPLAYHEIGHT * 3 / 4, 31, FT_OPT_CENTER, "tLastLapDelta: %02d:%02d.%03d", dMin, dSec, delta);

		LCD.DLEnd();
		LCD.Finish();
	}

	const char * PROGMEM warningMessageToString(WarningMessage warning) {
		switch (warning) {
			case WarningMessage::EngineTemperature:
			return WarningMessage_EngineTemperature;
			case WarningMessage::BatteryLow:
			return WarningMessage_BatteryLow;
			case WarningMessage::OilPressure:
			return WarningMessage_OilPressure;
			case WarningMessage::OilTemperature:
			return WarningMessage_OilTemperature;


		}
		return WarningMessage_Invalid;
	}
};

#endif /* FEDASHLCD_H_ */
 *
