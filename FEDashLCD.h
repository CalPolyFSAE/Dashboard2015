/*
 * FEDashLCD.h
 *
 *  Created on: Oct 11, 2015
 *      Author: thomaswillson
 */

#ifndef FEDASHLCD_H_
#define FEDASHLCD_H_

#include "DashLCD.h"

class FEDashLCD: public DashLCD {
public:
	enum class DashPages
		: uint8_t {
			WaitingForCAN, Driving, LapTrigger, Warning, Systems, Brakes, Charging, Suspension, Drivetrain, Performance,
	};

	enum class MCControlState
		: uint8_t {
			SC_Initial, SC_History, SC_ShallowHistory, SC_Terminal, BroadcastDiag, CurrentDrop, Enabled, Disabled
	};

	enum class ShutdownState
		: uint8_t {
			SC_Initial, SC_History, SC_ShallowHistory, SC_Terminal, AIR_Coolant_On, AIR_On, Precharge, TSMS_Off
	};

	enum class WarningSeverity
		: uint8_t {
			Okay, ShortWarning, LongWarning, ReturnToPits, Error, Danger
	};

	enum class eBMSChargingState
		: uint8_t {
			Disconnected, PreCharging, MainCharging, Balancing, ChargeFinishedOK, ChargingError
	};

	enum class eBMSLastChargeError
		: uint8_t {
			NoError,
		NoCellCommunication,
		NoCellCommunicationNoCAN,
		ChargeDurationExceeded,
		CellCommunicationLostDuringCharge,
		CannotSetBalancingVoltage,
		CellTempsTooHigh,
		NoCellCommsSoNoTemp,
		CellNumberMismatch,
		CellOvervoltage,
		CellProtectionEvent
	};

	enum class WarningMessage
		: uint8_t {
			ControllerTemperature,
		MotorTemperature,
		BatteryTemperature,
		BatteryLowVoltage,
		LVBattery,
		sbRIOTemperature,
		Precharge,
		BSPD,
		IMD,
		RemoteReturnToPits,
		EStopInterrupted,
		NoCellComms,
		MCOverSpeed,
		MCOverCurrent,
		MCOverVoltage,
		MCOverTemp,
		MCDirectionCommand,
		MCInverterResponseTimeout,
		MCDesatFault,
		MCHardwareOverCurrentFault,
		MCUnderVoltage,
		MCCommandMessageLost,
		MCMotorOverTemp,
		MCModAOverTemp,
		MCModBOverTem,
		MCModCOverTemp,
		MCPCBOverTemp,
		MCGateDrv1OverTemp,
		MCGateDrv2OverTemp,
		MCDateDrv3OverTemp,
		MCCurrentSensorFault,
		MCResolverNotConnected,
		ShutdownLatchTripped,
		UnknownBMS,
		RemoteEmergency
	};

	typedef union DashPage {
		uint8_t NDashPage;
		DashPages dashPage;
	} DashPage;

	typedef struct WarningCANMessage { //0xF1
		uint8_t NDashPage; //Required
		WarningSeverity warningSeverity;
		uint16_t associatedValue; // Half-Precision
		WarningMessage warningMessage;
		uint8_t notOK;
		uint8_t ledOutputs; // Bit-array: Green=0, Yellow=1, Red=2
		uint8_t unused;
	} WarningCANMessage;

	typedef struct DashCAN1Driving {
		uint16_t TMC;
		uint16_t TMotor;
		uint16_t TCellMax;
		uint16_t rBrakeBalLast;
	} DashCAN1Driving;

	typedef struct DashCAN1Charging {
		uint16_t TCellMin;
		uint16_t TCellMax;
		uint16_t VCellMin;
		uint16_t VCellMax;
	} DashCAN1Charging;

	typedef struct DashCAN2Driving {
		MCControlState eMCControlState;
		ShutdownState eShutdownState;
		uint16_t vCar;
		uint16_t tCurrentDelta;
		uint16_t VCellMin;
	} DashCAN2Driving;

	typedef struct DashCAN2Charging {
		uint16_t TCellMean;
		uint16_t VCellMean;
		uint16_t VTotal;
		eBMSChargingState chargingState;
		eBMSLastChargeError chargeError;
	} DashCAN2Charging;

	typedef struct DashCAN1Trig {
		uint32_t tLastLap;
		uint32_t tLastLapDelta;
	} DashCAN3Driving;

	typedef struct DashCAN1Systems {
		uint16_t TMCMax;
		uint16_t TMotorMax;
		uint16_t TCellMaxMax;
		uint16_t VBatteryMin;
	} DashCAN1Systems;

	typedef struct DashCAN2Systems {
		uint16_t VBattery;
	} DashCAN2Systems;

	typedef struct DashCAN1Brakes {
		uint16_t pBrakeF;
		uint16_t pBrakeR;
		uint16_t rBrakeBal;
		uint16_t pBrakeFMax;
	} DashCAN1Brakes;

	typedef struct DashCAN2Brakes {
		uint16_t pBrakeRMax;
		uint16_t rBrakeBalMin;
		uint16_t rBrakeBalmax;
	} DashCAN2Brakes;


	typedef struct DashCAN3Charging {
		uint16_t VChargerSetpoint;
		uint16_t IChargerSetpoint;
		uint16_t VChargerActual;
		uint16_t IChargerActual;
	} DashCAN3Charging;

	typedef struct DashCAN4Charging {
		uint8_t bChargerConnected;
	} DashCAN4Charging;

	typedef union DashCAN1 { //0xF0
		uint8_t data[8];
		DashCAN1Driving driving;
		DashCAN1Trig trig;
		DashCAN1Systems systems;
		DashCAN1Brakes brakes;
		DashCAN1Charging charging;
	} DashCAN1;

	typedef union DashCAN2 { //0xF2
		uint8_t data[8];
		DashCAN2Driving driving;
		DashCAN2Systems systems;
		DashCAN2Brakes brakes;
		DashCAN2Charging charging;
	} DashCAN2;

	typedef union DashCAN3 { //0xF3
		uint8_t data[8];
		DashCAN3Charging charging;
	} DashCAN3;

	typedef union DashCAN4 { //0xF4
		uint8_t data[8];
		DashCAN4Charging charging;
	} DashCAN4;

	//casting CAN data to a known structure
	//Data from DashLCD.h
	DashCAN1 *dashCAN1 = (DashCAN1 *) dashCAN1Data;
	DashCAN2 *dashCAN2 = (DashCAN2 *) dashCAN2Data;
	DashCAN3 *dashCAN3 = (DashCAN3 *) dashCAN3Data;
	DashCAN4 *dashCAN4 = (DashCAN4 *) dashCAN4Data;
	WarningCANMessage *warningCAN = (WarningCANMessage *) warningCANData;
	DashPage *dashPage = (DashPage *) &DashboardData.NDashPage;

	virtual ~FEDashLCD() {
		return;
	}

	virtual void sub_init() {
		return;
	}

	void updateDashboard();

protected:

	PGM_P blackRotaryString(int position);
	PGM_P yellowRotaryString(int position);
	PGM_P redRotaryString(int position);

	void brakes() {

	}

	void drivetrain() {

	}

	//charging screen
	void charging();

	void performance() {

	}

	void systems() {

	}

	//warning screen
	void warning();

	void driving();

	void lapTrigger();

	const char * PROGMEM warningMessageToString(WarningMessage warning);
};

#endif /* FEDASHLCD_H_ */
