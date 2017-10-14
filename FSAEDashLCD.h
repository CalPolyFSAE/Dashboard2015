/*
 * FSAEDashLCD.h
 *
 *  Created on: Mar 2, 2016
 *      Author: Daniel Baron
 */

#ifndef FEDASHLCD_H_
#define FEDASHLCD_H_

#include "DashLCD.h"


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
		uint8_t Brightness;
	} DashCAN3;

	DashCAN1 *dashCAN1 = (DashCAN1 *) dashCAN1Data;
	DashCAN2 *dashCAN2 = (DashCAN2 *) dashCAN2Data;
	DashCAN3 *dashCAN3 = (DashCAN3 *) dashCAN3Data;
//	DashCAN4 *dashCAN4 = (DashCAN4 *) dashCAN4Data;
//	WarningCANMessage *warningCAN = (WarningCANMessage *) warningCANData;

	WarningData warningData;
	uint8_t previousBrightness;

public:

	virtual ~FSAEDashLCD() {
		return;
	}

	virtual void sub_init() {
		previousBrightness = dashCAN3->Brightness = 100;
	}

	void updateDashboard();

protected:
	PGM_P blackRotaryString(int position);
	PGM_P yellowRotaryString(int position);
	PGM_P redRotaryString(int position);

	float motecToFloat(uint16_t value, float scaler=100.0) {
		return (float)swap(value) / scaler;
	}

	bool warningOverride();

	void warning();

	void driving();

	void lapTrigger();

	void sensors();

	const char * PROGMEM warningMessageToString(WarningMessage warning);
};

#endif /* FEDASHLCD_H_ */

