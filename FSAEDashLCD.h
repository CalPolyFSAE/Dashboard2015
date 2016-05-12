/*
 * FSAEDashLCD.h
 *
 *  Created on: Mar 2, 2016
 *      Author: Daniel Baron
 */

#ifndef FEDASHLCD_H_
#define FEDASHLCD_H_

#include "DashLCD.h"

static const char PROGMEM WarningMessage_EngineTemperature[] = "Engine Hot: %.1fC";
static const char PROGMEM WarningMessage_OilTemperature[] = "Oil Temperature: %.1fC";
static const char PROGMEM WarningMessage_OilPressure[] = "Oil Pressure: %.1f psi";
static const char PROGMEM WarningMessage_BatteryLow[] = "Battery Low: %.1fC";
static const char PROGMEM WarningMessage_Invalid[] = "Invalid Warning Msg.";

static const char PROGMEM RotaryRed1[] = "LC - Default";
static const char PROGMEM RotaryRed2[] = "LC - -100rpm";
static const char PROGMEM RotaryRed3[] = "LC - +100rpm";
static const char PROGMEM RotaryRed4[] = "LC - -250rpm";
static const char PROGMEM RotaryRed5[] = "LC - +250rpm";
static const char PROGMEM RotaryRed6[] = "Invalid";
static const char PROGMEM RotaryRed7[] = "Invalid";
static const char PROGMEM RotaryRed8[] = "Invalid";
static const char PROGMEM RotaryRedUnused[] = "Invalid";

PGM_P const RotaryRedStringTable[] PROGMEM =
{
	RotaryRed1, RotaryRed2, RotaryRed3, RotaryRed4, RotaryRed5, RotaryRed6, RotaryRed7, RotaryRed8, RotaryRedUnused, RotaryRedUnused, RotaryRedUnused, RotaryRedUnused
};

static const char PROGMEM RotaryYellow1[] = "Brightness: 25%";
static const char PROGMEM RotaryYellow2[] = "Brightness: 50%";
static const char PROGMEM RotaryYellow3[] = "Brightness: 75%";
static const char PROGMEM RotaryYellow4[] = "Brightness: 100%";
static const char PROGMEM RotaryYellow5[] = "Invalid";
static const char PROGMEM RotaryYellow6[] = "Invalid";
static const char PROGMEM RotaryYellow7[] = "Invalid";
static const char PROGMEM RotaryYellow8[] = "Invalid";
static const char PROGMEM RotaryYellowUnused[] = "Invalid";

PGM_P const RotaryYellowStringTable[] PROGMEM = {RotaryYellow1, RotaryYellow2,
	RotaryYellow3, RotaryYellow4, RotaryYellow5, RotaryYellow6, RotaryYellow7,
	RotaryYellow8, RotaryYellowUnused, RotaryYellowUnused, RotaryYellowUnused,
	RotaryYellowUnused};

static const char PROGMEM RotaryBlack1[] = "Page - Auto";
static const char PROGMEM RotaryBlack2[] = "Page - Driving";
static const char PROGMEM RotaryBlack3[] = "Page - Sensors";
static const char PROGMEM RotaryBlack4[] = "Invalid";
static const char PROGMEM RotaryBlack5[] = "Invalid";
static const char PROGMEM RotaryBlack6[] = "Invalid";
static const char PROGMEM RotaryBlack7[] = "Invalid";
static const char PROGMEM RotaryBlack8[] = "Invalid";
static const char PROGMEM RotaryBlackUnused[] = "Invalid";

PGM_P const RotaryBlackStringTable[] PROGMEM =
{
	RotaryBlack1, RotaryBlack2, RotaryBlack3, RotaryBlack4, RotaryBlack5, RotaryBlack6, RotaryBlack7, RotaryBlack8, RotaryBlackUnused, RotaryBlackUnused,RotaryBlackUnused,RotaryBlackUnused
};

static const size_t STATE_MAX_DESC_LENGTH = 30;

class FSAEDashLCD: public DashLCD {
protected:
	enum class DashPages
		: uint8_t {
			WaitingForCAN, Driving, Warning, Sensors
	};

	enum class WarningSeverity
		: uint8_t {
			Okay, ShortWarning, LongWarning, ReturnToPits, Error, Danger
	};

	enum class WarningMessage
		: uint8_t {
			EngineTemperature, OilTemperature, OilPressure, BatteryLow

	};

	typedef struct WarningData {
		WarningSeverity severity;
		WarningMessage message;
		float associatedValue;
	} WarningData;

	typedef struct DashCAN1 { // 0x0E0
		uint16_t EngineTemp;
		uint16_t RPM;
		uint16_t Gear;
		uint16_t Speed;
	} DashCAN1;

	typedef struct DashCAN2 { // 0x0E1
		uint16_t Lambda;
		uint16_t OilTemp;
		uint16_t MAP;
		uint16_t ThrottlePOS;
	} DashCAN2;

	typedef struct DashCAN3 { // 0x0E2
		uint16_t BatteryVoltage;
		uint16_t OilPressure;
		uint16_t IAT;
		uint16_t Unused3;
	} DashCAN3;

	DashCAN1 *dashCAN1 = (DashCAN1 *) dashCAN1Data;
	DashCAN2 *dashCAN2 = (DashCAN2 *) dashCAN2Data;
	DashCAN3 *dashCAN3 = (DashCAN3 *) dashCAN3Data;
//	DashCAN4 *dashCAN4 = (DashCAN4 *) dashCAN4Data;
//	WarningCANMessage *warningCAN = (WarningCANMessage *) warningCANData;

	WarningData warningData;
public:

	virtual ~FSAEDashLCD() {
		return;
	}

	void updateDashboard() {
		switch (CPFERotarySwitch::getPosition(CPFERotarySwitch::RotarySwitches::BLACK)) {
		case 1:  // Driving
			driving();
			break;
		case 3:  // Sensors
			sensors();
			break;
		default: // Auto mode
			if (warningOverride()) {
				warning();
			} else {
				driving();
			}
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

	float motecToFloat(uint16_t value, float scaler=100.0) {
		return (float)swap(value) / scaler;
	}

	bool warningOverride() {
		bool warningActive = false;

		const float EngineReturnToPitTemp = 205.0;
		const float EngineDangerTemp = 212.0;

		const float BatteryReturnToPitVoltage = 12.0;
		const float OilPressureWarning = 5.0;
		const float OilTempThreshold = 210.0;

		float EngineTemp, OilPressure, OilTemp, BatteryVoltage;
		EngineTemp = motecToFloat(dashCAN1->EngineTemp);
		OilPressure = motecToFloat(dashCAN3->OilPressure);
		OilTemp = motecToFloat(dashCAN2->OilTemp);
		BatteryVoltage = motecToFloat(dashCAN3->BatteryVoltage);

		if (EngineTemp > EngineReturnToPitTemp) {
			warningActive = true;
			warningData.associatedValue = EngineTemp;
			warningData.severity = EngineTemp > EngineDangerTemp ? WarningSeverity::Danger : WarningSeverity::ReturnToPits;
			warningData.message = WarningMessage::EngineTemperature;
		}
		else if (OilPressure < OilPressureWarning) {
			warningActive = true;
			warningData.associatedValue = OilPressure;
			warningData.severity = WarningSeverity::LongWarning;
			warningData.message = WarningMessage::OilPressure;
		}
		else if (OilTemp < OilTempThreshold) {
			warningActive = true;
			warningData.associatedValue = OilTemp;
			warningData.severity = WarningSeverity::LongWarning;
			warningData.message = WarningMessage::OilTemperature;
		}
		else if (BatteryVoltage < BatteryReturnToPitVoltage) {
			warningActive = true;
			warningData.associatedValue = BatteryVoltage;
			warningData.severity = WarningSeverity::ReturnToPits;
			warningData.message = WarningMessage::BatteryLow;
		}

		return warningActive;
	}

	void warning() {
		bool displayBoxes;
		const char *severityText;
		uint32_t color = 0x00000;

		switch (warningData.severity) {
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
		LCD.PrintTextFlash(FT_DISPLAYWIDTH / 2, FT_DISPLAYHEIGHT - 100, 29, FT_OPT_CENTER, warningMessageToString(warningData.message), warningData.associatedValue);

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
		float EngineTemp, RPM, Gear, Speed, OilTemp, OilPressure, BatteryVoltage, Lambda;

		EngineTemp = motecToFloat(dashCAN1->EngineTemp);
		RPM = motecToFloat(dashCAN1->RPM);
		Gear = motecToFloat(dashCAN1->Gear);
		Speed = motecToFloat(dashCAN1->Speed);
		OilTemp = motecToFloat(dashCAN2->OilTemp);
		OilPressure = motecToFloat(dashCAN3->OilPressure);
		BatteryVoltage = motecToFloat(dashCAN3->BatteryVoltage);
		Lambda = motecToFloat(dashCAN2->Lambda);

		LCD.DLStart();

		LCD.ColorRGB(0x00, 0xFF, 0xFF);
		LCD.PrintText(5, 0, 28, 0, "ET: %.1", EngineTemp);
		LCD.PrintText(5, 25, 28, 0, "RPM: %.0f", RPM);
		LCD.PrintText(5, 50, 28, 0, "Speed: %.1f", Speed);
		LCD.PrintText(5, 75, 28, 0, "Oil Temp: %.1f", OilTemp);
		LCD.PrintText(5, 75, 28, 0, "Oil Pres.: %.1fpsi", OilPressure);
		LCD.PrintText(5, 100, 28, 0, "Battery: %.1fV", BatteryVoltage);
		LCD.PrintText(5, 125, 28, 0, "Lambda: %.2f", Lambda);

		LCD.PrintText(FT_DISPLAYWIDTH / 2, FT_DISPLAYHEIGHT / 2, 1, FT_OPT_CENTER, "%d", Gear);

		LCD.ColorRGB(0xFFFFFF);
		LCD.Cmd_FGColor(0xFF0000);
		LCD.Cmd_BGColor(0xFF0000);
		LCD.Cmd_Slider(FT_DISPLAYWIDTH - 30, 20, 20, 180, 0, 220 - (uint16_t) EngineTemp, 220);

		LCD.ColorRGB(0xFFFFFF);
		LCD.Cmd_FGColor(0xFF0000);
		LCD.Cmd_BGColor(0xFF0000);
		LCD.Cmd_Slider(FT_DISPLAYWIDTH - 80, 20, 20, 180, 0, 10000 - (uint16_t) RPM, 10000);

		LCD.DLEnd();
		LCD.Finish();
	}

	void lapTrigger() {
//		uint32_t time = dashCAN1->trig.tLastLap;
//		int32_t delta = dashCAN1->trig.tLastLapDelta;
//
//		uint8_t lMin = time / 60000;
//		time -= lMin * 60000;
//		uint8_t lSec = time / 1000;
//		time -= lSec * 1000;
//
//		int8_t dMin = delta / 60000;
//		delta -= dMin * 60000;
//		int8_t dSec = delta / 1000;
//		delta -= dSec * 1000;
//
//		LCD.DLStart();
//		LCD.ClearColorRGB(0xFFFFFF);
//		LCD.Clear(1, 1, 1);
//
//		LCD.ColorRGB(0x000000);
//		LCD.PrintText(FT_DISPLAYWIDTH / 2, FT_DISPLAYHEIGHT / 4, 31, FT_OPT_CENTER, "tLastLap: %02d:%02d.%03d", lMin, lSec, time);
//		LCD.PrintText(FT_DISPLAYWIDTH / 2, FT_DISPLAYHEIGHT * 3 / 4, 31, FT_OPT_CENTER, "tLastLapDelta: %02d:%02d.%03d", dMin, dSec, delta);
//
//		LCD.DLEnd();
//		LCD.Finish();
	}

	void sensors() {
		float TPS = motecToFloat(dashCAN2->ThrottlePOS), MAP = motecToFloat(dashCAN2->MAP), IAT = motecToFloat(dashCAN3->IAT);

		LCD.DLStart();

		LCD.ColorRGB(0x00, 0xFF, 0xFF);
		LCD.PrintText(5, 0, 28, 0, "TPS: %.1%", TPS);
		LCD.PrintText(5, 25, 28, 0, "MAP: %.2fpsi", MAP);
		LCD.PrintText(5, 50, 28, 0, "IAT: %.1F", IAT);

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

