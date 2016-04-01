/*
 * FEDashLCD.h
 *
 *  Created on: Oct 11, 2015
 *      Author: thomaswillson
 */

#ifndef FEDASHLCD_H_
#define FEDASHLCD_H_

#include "DashLCD.h"

static const char PROGMEM WarningMessage_ControllerTemperature[] =
"CONTROLLER WARM: %.2fC";
static const char PROGMEM WarningMessage_MotorTemperature[] =
"MOTOR WARM: %.2fC";
static const char PROGMEM WarningMessage_BatteryTemperature[] =
"BATTERY WARM: %.2fC";
static const char PROGMEM WarningMessage_BatteryLowVoltage[] =
"PACK LOW VOLTAGE: %.2fV";
static const char PROGMEM WarningMessage_LVBattery[] = "GLV LOW VOLTAGE: %.2fV";
static const char PROGMEM WarningMessage_sbRIOTemperature[] =
"sbRIO WARM: %.0fC";
static const char PROGMEM WarningMessage_Precharge[] = "HV PRECHARGING: %.0fV";
static const char PROGMEM WarningMessage_Invalid[] = "INVALID MSG";
static const char PROGMEM WarningMessage_BSPD[] = "BSPD ERROR";
static const char PROGMEM WarningMessage_IMD[] = "IMD ERROR";
static const char PROGMEM WarningMessage_RemotePit[] = "Remote Request";
static const char PROGMEM WarningMessage_EStop[] = "An E-Stop is Pressed";
static const char PROGMEM WarningMessage_NoCellComms[] = "NO CELL COMMS";
static const char PROGMEM WarningMessage_MCOverSpeed[] = "MC Over Speed";
static const char PROGMEM WarningMessage_MCOverCurrent[] = "MC Over Current";
static const char PROGMEM WarningMessage_MCOverVoltage[] = "MC Over Voltage";
static const char PROGMEM WarningMessage_MCOverTemp[] = "MC Inverter Overtemp";
static const char PROGMEM WarningMessage_MCDirectionCommand[] =
"MC Direction Error";
static const char PROGMEM WarningMessage_MCInverterResponseTimeout[] =
"MC Inverter Timeout";
static const char PROGMEM WarningMessage_MCDesatFault[] = "MC Desat Fault";
static const char PROGMEM WarningMessage_MCHardwareOverCurrentFault[] =
"MC HW Over-Current";
static const char PROGMEM WarningMessage_MCUnderVoltage[] = "MC Under Voltage";
static const char PROGMEM WarningMessage_MCCommandMessageLost[] =
"MC Command Message Lost";
static const char PROGMEM WarningMessage_MCMotorOverTemp[] = "Motor Over Temp";
static const char PROGMEM WarningMessage_MCModAOverTemp[] = "Module A Overtemp";
static const char PROGMEM WarningMessage_MCModBOverTemp[] = "Module B Overtemp";
static const char PROGMEM WarningMessage_MCModCOverTemp[] = "Module C Overtemp";
static const char PROGMEM WarningMessage_MCPCBOverTemp[] = "PCB Overtemp";
static const char PROGMEM WarningMessage_MCGateDrv1OverTemp[] =
"Gate DRV 1 Overtemp";
static const char PROGMEM WarningMessage_MCGateDrv2OverTemp[] =
"Gate DRV 2 Overtemp";
static const char PROGMEM WarningMessage_MCGateDrv3OverTemp[] =
"Gate DRV 3 Overtemp";
static const char PROGMEM WarningMessage_MCCurrentSensorFault[] =
"MC Curr Sensor Fault";
static const char PROGMEM WarningMessage_MCResolverNotConnected[] =
"Resolver Not Connected";
static const char PROGMEM WarningMessage_ShutdownLatchTripped[] =
"Shutdown Latch Tripped";
static const char PROGMEM WarningMessage_UnknownBMS[] = "Unknown BMS Error";
static const char PROGMEM WarningMessage_RemoteEmergency[] =
"REMOTE EMERGENCY SHUTDOWN";

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

static const char PROGMEM RotaryBlack1[] = "Default";
static const char PROGMEM RotaryBlack2[] = "Pit";
static const char PROGMEM RotaryBlack3[] = "BrakeBal";
static const char PROGMEM RotaryBlack4[] = "Throttle";
static const char PROGMEM RotaryBlack5[] = "Code";
static const char PROGMEM RotaryBlack6[] = "MULTI-6";
static const char PROGMEM RotaryBlack7[] = "MULTI-7";
static const char PROGMEM RotaryBlack8[] = "MULTI-8";
static const char PROGMEM RotaryBlackUnused[] = "Invalid";

PGM_P const RotaryBlackStringTable[] PROGMEM =
{
	RotaryBlack1, RotaryBlack2, RotaryBlack3, RotaryBlack4, RotaryBlack5, RotaryBlack6, RotaryBlack7, RotaryBlack8, RotaryBlackUnused, RotaryBlackUnused,RotaryBlackUnused,RotaryBlackUnused
};

static const size_t STATE_MAX_DESC_LENGTH = 30;

static const char PROGMEM MCState0[] = "SC.Initial";
static const char PROGMEM MCState1[] = "SC.History";
static const char PROGMEM MCState2[] = "SC.ShallowHistory";
static const char PROGMEM MCState3[] = "SC.Terminal";
static const char PROGMEM MCState4[] = "TransDiag";
static const char PROGMEM MCState5[] = "CurrentDrop";
static const char PROGMEM MCState6[] = "Enabled";
static const char PROGMEM MCState7[] = "Disabled";

PGM_P const MCStateStringTable[] PROGMEM =
{
	MCState0, MCState1, MCState2, MCState3, MCState4, MCState5, MCState6, MCState7
};

static const char PROGMEM ShutdownState0[] = "SC.Initial";
static const char PROGMEM ShutdownState1[] = "SC.History";
static const char PROGMEM ShutdownState2[] = "SC.ShallowHistory";
static const char PROGMEM ShutdownState3[] = "AIR's & Coolant On";
static const char PROGMEM ShutdownState4[] = "AIR's On";
static const char PROGMEM ShutdownState5[] = "Precharge";
static const char PROGMEM ShutdownState6[] = "TSMS Off";

PGM_P const ShutdownStateStringTable[] PROGMEM =
{
	ShutdownState0, ShutdownState1, ShutdownState2, ShutdownState3, ShutdownState4, ShutdownState5, ShutdownState6
};

class FEDashLCD: public DashLCD {
public:
	enum class DashPages
		: uint8_t {
			WaitingForCAN, Driving, LapTrigger, Warning, Systems, Brakes, Electrical, Suspension, Drivetrain, Performance,
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
		float associatedValue;
		WarningMessage warningMessage;
		uint8_t notOK;
	} WarningCANMessage;

	typedef struct DashCAN1Driving {
		uint16_t TMC;
		uint16_t TMotor;
		uint16_t TCellMax;
		uint16_t rBrakeBalLast;
	} DashCAN1Driving;

	typedef struct DashCAN2Driving {
		MCControlState eMCControlState;
		ShutdownState eShutdownState;
		uint16_t vCar;
		uint16_t tCurrentDelta;
		uint16_t VCellMin;
	} DashCAN2Driving;

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

	typedef union DashCAN1 { //0xF0
		uint8_t data[8];
		DashCAN1Driving driving;
		DashCAN1Trig trig;
		DashCAN1Systems systems;
		DashCAN1Brakes brakes;
	} DashCAN1;

	typedef union DashCAN2 { //0xF2
		uint8_t data[8];
		DashCAN2Driving driving;
		DashCAN2Systems systems;
		DashCAN2Brakes brakes;
	} DashCAN2;

	typedef union DashCAN3 { //0xF3
		uint8_t data[8];
	} DashCAN3;

	typedef union DashCAN4 { //0xF4
		uint8_t data[8];
	} DashCAN4;

	DashCAN1 *dashCAN1 = (DashCAN1 *) dashCAN1Data;
	DashCAN2 *dashCAN2 = (DashCAN2 *) dashCAN2Data;
	DashCAN3 *dashCAN3 = (DashCAN3 *) dashCAN3Data;
	DashCAN4 *dashCAN4 = (DashCAN4 *) dashCAN4Data;
	WarningCANMessage *warningCAN = (WarningCANMessage *) warningCANData;
	DashPage *dashPage = (DashPage *) &DashboardData.NDashPage;

	virtual ~FEDashLCD() {
		return;
	}

	void updateDashboard() {
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
		float associatedValue;

		float16::toFloat32(&associatedValue, warningCAN->associatedValue);

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
		LCD.PrintTextFlash(FT_DISPLAYWIDTH / 2, FT_DISPLAYHEIGHT - 100, 29, FT_OPT_CENTER, warningMessageToString(warningCAN->warningMessage), associatedValue);

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
			case WarningMessage::BatteryLowVoltage:
			return WarningMessage_BatteryLowVoltage;
			case WarningMessage::BatteryTemperature:
			return WarningMessage_BatteryTemperature;
			case WarningMessage::ControllerTemperature:
			return WarningMessage_ControllerTemperature;
			case WarningMessage::LVBattery:
			return WarningMessage_LVBattery;
			case WarningMessage::MotorTemperature:
			return WarningMessage_MotorTemperature;
			case WarningMessage::Precharge:
			return WarningMessage_Precharge;
			case WarningMessage::sbRIOTemperature:
			return WarningMessage_sbRIOTemperature;
			case WarningMessage::IMD:
			return WarningMessage_IMD;
			case WarningMessage::BSPD:
			return WarningMessage_BSPD;
			case WarningMessage::RemoteReturnToPits:
			return WarningMessage_RemotePit;
			case WarningMessage::EStopInterrupted:
			return WarningMessage_EStop;
			case WarningMessage::NoCellComms:
			return WarningMessage_NoCellComms;
			case WarningMessage::MCOverSpeed:
			return WarningMessage_MCOverSpeed;
			case WarningMessage::MCOverCurrent:
			return WarningMessage_MCOverCurrent;
			case WarningMessage::MCOverVoltage:
			return WarningMessage_MCOverVoltage;
			case WarningMessage::MCOverTemp:
			return WarningMessage_MCOverTemp;
			case WarningMessage::MCDirectionCommand:
			return WarningMessage_MCDirectionCommand;
			case WarningMessage::MCInverterResponseTimeout:
			return WarningMessage_MCInverterResponseTimeout;
			case WarningMessage::MCDesatFault:
			return WarningMessage_MCDesatFault;
			case WarningMessage::MCHardwareOverCurrentFault:
			return WarningMessage_MCHardwareOverCurrentFault;
			case WarningMessage::MCUnderVoltage:
			return WarningMessage_MCUnderVoltage;
			case WarningMessage::MCCommandMessageLost:
			return WarningMessage_MCCommandMessageLost;
			case WarningMessage::MCMotorOverTemp:
			return WarningMessage_MCMotorOverTemp;
			case WarningMessage::MCModAOverTemp:
			return WarningMessage_MCModAOverTemp;
			case WarningMessage::MCModBOverTem:
			return WarningMessage_MCModBOverTemp;
			case WarningMessage::MCModCOverTemp:
			return WarningMessage_MCModCOverTemp;
			case WarningMessage::MCPCBOverTemp:
			return WarningMessage_MCPCBOverTemp;
			case WarningMessage::MCGateDrv1OverTemp:
			return WarningMessage_MCGateDrv1OverTemp;
			case WarningMessage::MCGateDrv2OverTemp:
			return WarningMessage_MCGateDrv2OverTemp;
			case WarningMessage::MCDateDrv3OverTemp:
			return WarningMessage_MCGateDrv3OverTemp;
			case WarningMessage::MCCurrentSensorFault:
			return WarningMessage_MCCurrentSensorFault;
			case WarningMessage::MCResolverNotConnected:
			return WarningMessage_MCResolverNotConnected;
			case WarningMessage::ShutdownLatchTripped:
			return WarningMessage_ShutdownLatchTripped;
			case WarningMessage::UnknownBMS:
			return WarningMessage_UnknownBMS;
			case WarningMessage::RemoteEmergency:
			return WarningMessage_RemoteEmergency;
		}
		return WarningMessage_Invalid;
	}
};

#endif /* FEDASHLCD_H_ */
