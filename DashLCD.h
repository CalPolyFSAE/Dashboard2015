#include <stdio.h>
#include <stdint.h>
#include <avr/pgmspace.h>
#include "FTDI _V1/FT_VM801P43_50.h"

#include <AVRLibrary/CPFECANLib.h>
#include "float16.hpp"
#include <AVRLibrary/arduino/Arduino.h>
#include "CPFERotarySwitch.h"

static const uint8_t PROGMEM CPRacingLogo[] = {
	#include "GraphicsAssets/CPRacingLogo.h"
};

static const uint8_t PROGMEM ArialBlackNumberFont[] = {
	#include "GraphicsAssets/ArialBlackNumberFont.h"
};

static constexpr uint8_t WarningCANMessageID = 0x10;
static constexpr uint8_t DashCAN1ID = 0x0E0; //0x11;
static constexpr uint8_t DashCAN2ID = 0x0E1;//0x12;
static constexpr uint8_t DashCAN3ID = 0x13;
static constexpr uint8_t DashCAN4ID = 0x14;
static constexpr uint8_t DashCANInputID = 0x15;

static const CPFECANLib::MSG PROGMEM DashCAN1MSG =
{	{	DashCAN1ID}, 8, 0, 0, 0};

static const CPFECANLib::MSG PROGMEM DashCAN1Mask = { {0xFFF}, 8, 1, 1, 0};

static const CPFECANLib::MSG PROGMEM DashCAN2MSG =
{	{	DashCAN2ID}, 8, 0, 0, 0};

static const CPFECANLib::MSG PROGMEM DashCAN2Mask = { {0xFFF}, 8, 1, 1, 0};

static const CPFECANLib::MSG PROGMEM DashCAN3MSG =
{	{	DashCAN3ID}, 8, 0, 0, 0};

static const CPFECANLib::MSG PROGMEM DashCAN3Mask = { {0xFFF}, 8, 1, 1, 0};

static const CPFECANLib::MSG PROGMEM DashCAN4MSG =
{	{	DashCAN4ID}, 8, 0, 0, 0};

static const CPFECANLib::MSG PROGMEM DashCAN4Mask = { {0xFFF}, 8, 1, 1, 0};

static const CPFECANLib::MSG PROGMEM DashCANWarningMSG = {
	{	WarningCANMessageID}, 8, 0, 0, 0};

static const CPFECANLib::MSG PROGMEM DashCANWarningMask = { {0xFFF}, 8, 1, 1,
	0};

static const uint8_t ROTARY_MAX_DESC_LENGTH = 32;

class DashLCD {
public:
	virtual ~DashLCD() {
		return;
	}

	enum class OutputState
		: uint8_t {
			Off, Flashing, On
	};

	typedef struct DASHBOARD_DATA {
		uint8_t NDashPage;

		OutputState redLED;
		OutputState yellowLED;
		OutputState greenLED;

		bool waitingForCANOverride;
		uint8_t previousRotaryPositions[CPFERotarySwitch::NUM_ROTARYS];
		bool rotaryOverride;
		CPFERotarySwitch::RotarySwitches rotaryToShow;

	} DASHBOARD_DATA;

	volatile DASHBOARD_DATA DashboardData;

	virtual void updateDashboard() {
		return;
	}

	void TIMER2_OVF_INT() { //Timer 2 OVF INT for CAN Timeout
		++CAN_OVFCount;
		++TransOVFCount;
		++RotaryDispOVFCount;
		++LEDFlashOVFCount;

		if (CAN_OVFCount > CAN_TIMER_OVF_COUNT_MAX) {
			CAN_OVFCount = 0;
			DashboardData.waitingForCANOverride = true;
		}

		if (RotaryDispOVFCount >= ROTARY_DISP_TIMER_OVF_COUNT_MAX) {
			RotaryDispOVFCount = 0;
			DashboardData.rotaryOverride = false;
		}

		if (LEDFlashOVFCount > LED_FLASH_TIMER_OVF_COUNT_MAX) {
			LEDFlashOVFCount = 0;

			if (DashboardData.redLED == OutputState::Flashing) {
				if (PORTB & BIT7)
					PORTB &= ~(BIT7);
				else
					PORTB |= BIT7;
			}
			if (DashboardData.yellowLED == OutputState::Flashing) {
				if (PORTB & BIT6)
					PORTB &= ~(BIT6);
				else
					PORTB |= BIT6;
			}
			if (DashboardData.greenLED == OutputState::Flashing) {
				if (PORTB & BIT5)
					PORTB &= ~(BIT5);
				else
					PORTB |= BIT5;
			}
		}
	}

	void init(CPFECANLib::CAN_MSG_RX canRxIntFunc) {
		Serial.begin(57600);

		if (bootupConfigure()) {
			Serial.println("Init Failed");
		}

		DashboardData.waitingForCANOverride = true;
		waitingForCAN(false); //Faster boot.
		LCD.DisplayOn();

		uploadLogoToController();
		uploadArialBlackNumberFontToController();

		CPFECANLib::init(CPFECANLib::CAN_BAUDRATE::B1M, canRxIntFunc);
		initCAN_RX();

		//Init CAN timeout timer (Timer 2)
		TCCR2A = (1 << CS22) | (1 << CS21) | (1 << CS20); //Normal mode, prescale 1/1024 for 63 OVF/sec
		TIMSK2 = (1 << TOIE2); //Timer 2 OVF Interrupt

		//Init Rotary Switches
		CPFERotarySwitch::init(12);

		//Init Digital Inputs for Buttons
		DDRC &= ~((1 << 4) - 1); //Set direction
		PORTC |= ((1 << 4) - 1); //Enable pullups

		DDRB |= BIT7 + BIT6 + BIT5;
		PORTB &= ~(BIT7 + BIT6 + BIT5);

		DDRF = 0x00;
	}

	virtual void sub_init();

	void updateDisplay() {
		CPFERotarySwitch::runTasks();

		if (TransOVFCount >= TRANS_TIMER_OVF_COUNT_MAX) {
			TransOVFCount = 0;
			transmitDashboardInfo();
		}

		if (DashboardData.redLED == OutputState::On) {
			PORTB |= BIT7;
		} else if (DashboardData.redLED == OutputState::Off) {
			PORTB &= ~BIT7;
		}

		if (DashboardData.yellowLED == OutputState::On) {
			PORTB |= BIT6;
		} else if (DashboardData.yellowLED == OutputState::Off) {
			PORTB &= ~BIT6;
		}

		if (DashboardData.greenLED == OutputState::On) {
			PORTB |= BIT5;
		} else if (DashboardData.greenLED == OutputState::Off) {
			PORTB &= ~BIT5;
		}

		if (false){// False for rotary disable//(DashboardData.rotaryOverride) {
			rotaryOverride();
		} else if (DashboardData.waitingForCANOverride) {
			waitingForCAN();
		} else {
			updateDashboard();
		}
	}

	void CAN_RX_Int(CPFECANLib::MSG *msg, uint8_t mobNum) {
		resetTimeoutTimer();
		DashboardData.waitingForCANOverride = false;

		switch (msg->identifier.standard) {
		case DashCAN1ID:
			memcpy((void *) &dashCAN1Data, msg->data, sizeof(dashCAN1Data));
			RX_DashCAN1(true);
			break;
		case DashCAN2ID:
			memcpy((void *) &dashCAN2Data, msg->data, sizeof(dashCAN2Data));
			RX_DashCAN2(true);
			break;
		case DashCAN3ID:
			memcpy((void *) &dashCAN3Data, msg->data, sizeof(dashCAN3Data));
			RX_DashCAN3(true);
			break;
		case DashCAN4ID:
			memcpy((void *) &dashCAN4Data, msg->data, sizeof(dashCAN4Data));
			RX_DashCAN4(true);
			break;
		case WarningCANMessageID:
			memcpy((void *) &warningCANData, msg->data, sizeof(warningCANData));
			DashboardData.NDashPage = warningCANData[0];
			RX_WarningCAN(true);
			break;
		}
	}

protected:
	const uint8_t CAN_TIMER_OVF_COUNT_MAX = 254;
	volatile uint8_t CAN_OVFCount;

	const uint8_t TRANS_TIMER_OVF_COUNT_MAX = 12;
	volatile uint8_t TransOVFCount;

	const uint8_t ROTARY_DISP_TIMER_OVF_COUNT_MAX = 60;
	volatile uint8_t RotaryDispOVFCount;

	const uint8_t LED_FLASH_TIMER_OVF_COUNT_MAX = 20;
	volatile uint8_t LEDFlashOVFCount;

	FT801IMPL_SPI LCD;

	static constexpr uint8_t DashCAN1Mob = 0;
	static constexpr uint8_t DashCAN2Mob = 1;
	static constexpr uint8_t DashCAN3Mob = 2;
	static constexpr uint8_t DashCAN4Mob = 3;
	static constexpr uint8_t WarningCANMob = 4;
	static constexpr uint8_t DashCANInputMob = 5;

	typedef struct DashCANInput { //0xF5
		uint8_t ButtonsArray;
		uint8_t RedRotary;
		uint8_t YellowRotary;
		uint8_t BlackRotary;
	} DashCANInput;

	volatile uint8_t dashCAN1Data[8];
	volatile uint8_t dashCAN2Data[8];
	volatile uint8_t dashCAN3Data[8];
	volatile uint8_t dashCAN4Data[8];
	volatile uint8_t warningCANData[8];

	virtual PGM_P blackRotaryString(int position) {
		return 0;
	}
	virtual PGM_P yellowRotaryString(int position) {
		return 0;
	}
	virtual PGM_P redRotaryString(int position) {
		return 0;
	}

	void initCAN_RX() {
		RX_DashCAN1(false);
		RX_DashCAN2(false);
		RX_DashCAN3(false);
		RX_DashCAN4(false);
		RX_WarningCAN(false);
	}

	void transmitDashboardInfo() {
		uint8_t dataBuffer[8];
		DashCANInput *data = (DashCANInput*) dataBuffer;
		CPFECANLib::MSG msg;

		msg.identifier.standard = DashCANInputID;
		msg.data = (uint8_t *) &dataBuffer;
		msg.dlc = 8;
		msg.ide = 0;
		msg.rtr = 0;

		data->ButtonsArray = ~PINC;
		data->BlackRotary = CPFERotarySwitch::getPosition(CPFERotarySwitch::RotarySwitches::BLACK);
		data->YellowRotary = CPFERotarySwitch::getPosition(CPFERotarySwitch::RotarySwitches::YELLOW);
		data->RedRotary = CPFERotarySwitch::getPosition(CPFERotarySwitch::RotarySwitches::RED);

		if (data->BlackRotary != DashboardData.previousRotaryPositions[(uint8_t) CPFERotarySwitch::RotarySwitches::BLACK]) {
			DashboardData.rotaryOverride = true;
			DashboardData.rotaryToShow = CPFERotarySwitch::RotarySwitches::BLACK;
			RotaryDispOVFCount = 0;
		} else if (data->YellowRotary != DashboardData.previousRotaryPositions[(uint8_t) CPFERotarySwitch::RotarySwitches::YELLOW]) {
			DashboardData.rotaryOverride = true;
			DashboardData.rotaryToShow = CPFERotarySwitch::RotarySwitches::YELLOW;
			RotaryDispOVFCount = 0;
		} else if (data->RedRotary != DashboardData.previousRotaryPositions[(uint8_t) CPFERotarySwitch::RotarySwitches::RED]) {
			DashboardData.rotaryOverride = true;
			DashboardData.rotaryToShow = CPFERotarySwitch::RotarySwitches::RED;
			RotaryDispOVFCount = 0;
		}

		for (int i = 0; i < CPFERotarySwitch::NUM_ROTARYS; ++i) {
			DashboardData.previousRotaryPositions[i] = CPFERotarySwitch::getPosition((CPFERotarySwitch::RotarySwitches) i);
		}

		CPFECANLib::sendMsgUsingMOB(DashCANInputMob, &msg);

		//Request Updated Positions for next iteration
		CPFERotarySwitch::requestUpdatedPositions();
	}

	void RX_DashCAN1(bool interruptMode) {
		CPFECANLib::enableMOBAsRX_PROGMEM(DashCAN1Mob, &DashCAN1MSG, &DashCAN1Mask, interruptMode);
	}

	void RX_DashCAN2(bool interruptMode) {
		CPFECANLib::enableMOBAsRX_PROGMEM(DashCAN2Mob, &DashCAN2MSG, &DashCAN2Mask, interruptMode);
	}

	void RX_DashCAN3(bool interruptMode) {
		CPFECANLib::enableMOBAsRX_PROGMEM(DashCAN3Mob, &DashCAN3MSG, &DashCAN3Mask, interruptMode);
	}

	void RX_DashCAN4(bool interruptMode) {
		CPFECANLib::enableMOBAsRX_PROGMEM(DashCAN4Mob, &DashCAN4MSG, &DashCAN4Mask, interruptMode);
	}

	void RX_WarningCAN(bool interruptMode) {
		CPFECANLib::enableMOBAsRX_PROGMEM(WarningCANMob, &DashCANWarningMSG, &DashCANWarningMask, interruptMode);
	}

	int16_t bootupConfigure() {
		LCD.Init(FT_DISPLAY_RESOLUTION, 0, false);
		return 0;
	}

	void uploadLogoToController() {
		LCD.Cmd_Inflate(108676);
		LCD.WriteCmdfromflash(CPRacingLogo, sizeof(CPRacingLogo));
		LCD.Finish();
		LCD.DLStart();
		LCD.BitmapHandle(0);
		LCD.BitmapSource(108676);
		LCD.BitmapLayout(FT_ARGB1555, 500, 49);
		LCD.BitmapSize(FT_BILINEAR, FT_BORDER, FT_BORDER, 250, 49);
		LCD.DLEnd();
		LCD.Finish();
	}

	void uploadArialBlackNumberFontToController() {
		LCD.Cmd_Inflate(0);
		LCD.WriteCmdfromflash(ArialBlackNumberFont, sizeof(ArialBlackNumberFont));
		LCD.Finish();
		LCD.DLStart();
		LCD.BitmapHandle(1);
		LCD.BitmapSource(-340940);
		LCD.BitmapLayout(FT_L8, 68, 114);
		LCD.BitmapSize(FT_NEAREST, FT_BORDER, FT_BORDER, 68, 114);
		LCD.Cmd_SetFont(0, 0);
		LCD.DLEnd();
		LCD.Finish();
	}

	void waitingForCAN() {
		waitingForCAN(true);
	}

	void waitingForCAN(bool withLogo) {
		LCD.DLStart();
		const char Display_string[] = "Cal Poly FSAE";

		LCD.ColorRGB(0xFF, 0xFF, 0xFF);
		LCD.Cmd_Text(FT_DISPLAYWIDTH / 2, FT_DISPLAYHEIGHT / 8, 29, FT_OPT_CENTER, Display_string);
		LCD.ColorRGB(0xFF, 0x00, 0x00);
		LCD.PrintText(FT_DISPLAYWIDTH / 2, FT_DISPLAYHEIGHT / 2, 29, FT_OPT_CENTER, "WAITING FOR CAN DATA...");

		if (withLogo) {
			LCD.ColorRGB(0xFF, 0xFF, 0xFF);
			LCD.Begin(FT_BITMAPS);
			LCD.Vertex2ii(FT_DISPLAYWIDTH / 4, 215, 0, 0);
			LCD.End();
		}

		LCD.DLEnd();
		LCD.Finish();
	}


	void rotaryOverride() {
		uint32_t color = 0x000000;
		uint8_t position = CPFERotarySwitch::getPosition(DashboardData.rotaryToShow);
		char positionDescription[ROTARY_MAX_DESC_LENGTH];

		switch (DashboardData.rotaryToShow) {
		case CPFERotarySwitch::RotarySwitches::BLACK:
			color = 0x000000;
			strncpy_P(positionDescription, blackRotaryString(position), ROTARY_MAX_DESC_LENGTH);
			break;
		case CPFERotarySwitch::RotarySwitches::YELLOW:
			color = 0xFFFF00;
			strncpy_P(positionDescription, yellowRotaryString(position), ROTARY_MAX_DESC_LENGTH);
			break;
		case CPFERotarySwitch::RotarySwitches::RED:
			color = 0xFF0000;
			strncpy_P(positionDescription, redRotaryString(position), ROTARY_MAX_DESC_LENGTH);
			break;
		}

		LCD.DLStart();
		LCD.ClearColorRGB(color);
		LCD.Clear(1, 1, 1);

		LCD.ColorRGB(~color);
		LCD.PrintText(FT_DISPLAYWIDTH / 2, FT_DISPLAYHEIGHT / 2, 31, FT_OPT_CENTER, "%d", position);
		LCD.PrintText(25, 25, 31, 0, "%s", positionDescription);

		LCD.DLEnd();
		LCD.Finish();
	}

	float swap(float d) {
		float a;
		unsigned char *dst = (unsigned char *) &a;
		unsigned char *src = (unsigned char *) &d;
		dst[0] = src[3];
		dst[1] = src[2];
		dst[2] = src[1];
		dst[3] = src[0];
		return a;
	}

	uint32_t swap(uint32_t d) {
		uint32_t a;
		unsigned char *dst = (unsigned char *) &a;
		unsigned char *src = (unsigned char *) &d;
		dst[0] = src[3];
		dst[1] = src[2];
		dst[2] = src[1];
		dst[3] = src[0];
		return a;
	}

	uint16_t swap(uint16_t d) {
		uint16_t a;
		unsigned char *dst = (unsigned char *) &a;
		unsigned char *src = (unsigned char *) &d;
		dst[0] = src[1];
		dst[1] = src[0];
		return a;
	}

	void resetTimeoutTimer() {
		CAN_OVFCount = 0;
	}
};
