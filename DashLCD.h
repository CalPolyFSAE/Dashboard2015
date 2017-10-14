#include <stdio.h>
#include <stdint.h>
#include <avr/pgmspace.h>
#include "FTDI _V1/FT_VM801P43_50.h"

#include <AVRLibrary/CPFECANLib.h>
#include "float16.hpp"
#include <AVRLibrary/arduino/Arduino.h>
#include "CPFERotarySwitch.h"

//variables should not be declared in header files
//use extern... then define in DashLCD.cpp if they need to be used by other files

static const uint8_t PROGMEM CPRacingLogo[] = {
	#include "GraphicsAssets/CPRacingLogo.h"
};

static const uint8_t PROGMEM ArialBlackNumberFont[] = {
	#include "GraphicsAssets/ArialBlackNumberFont.h"
};

//CAN message IDs
static constexpr uint8_t WarningCANMessageID = 0x10;
static constexpr uint8_t DashCAN1ID = 0x0E0; //0x11;
static constexpr uint8_t DashCAN2ID = 0x0E1;//0x12;
static constexpr uint8_t DashCAN3ID = 0x13;
static constexpr uint8_t DashCAN4ID = 0x14;
static constexpr uint8_t DashCANInputID = 0x15;

static const CPFECANLib::MSG PROGMEM DashCAN1MSG =
	{	{DashCAN1ID}, 8, 0, 0, 0};

static const CPFECANLib::MSG PROGMEM DashCAN1Mask = { {0xFFF}, 8, 1, 1, 0};

static const CPFECANLib::MSG PROGMEM DashCAN2MSG =
	{	{DashCAN2ID}, 8, 0, 0, 0};

static const CPFECANLib::MSG PROGMEM DashCAN2Mask = { {0xFFF}, 8, 1, 1, 0};

static const CPFECANLib::MSG PROGMEM DashCAN3MSG =
	{	{DashCAN3ID}, 8, 0, 0, 0};

static const CPFECANLib::MSG PROGMEM DashCAN3Mask = { {0xFFF}, 8, 1, 1, 0};

static const CPFECANLib::MSG PROGMEM DashCAN4MSG =
	{	{DashCAN4ID}, 8, 0, 0, 0};

static const CPFECANLib::MSG PROGMEM DashCAN4Mask = { {0xFFF}, 8, 1, 1, 0};

static const CPFECANLib::MSG PROGMEM DashCANWarningMSG = 
	{	{WarningCANMessageID}, 8, 0, 0, 0};

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

	void TIMER2_OVF_INT();//Timer 2 OVF INT for CAN Timeout

	//initialize hardware settings
	void init(CPFECANLib::CAN_MSG_RX canRxIntFunc);

	virtual void sub_init();

	//called by main loop
	void updateDisplay();

	//can recieve interrupt function
	void CAN_RX_Int(CPFECANLib::MSG *msg, uint8_t mobNum);

protected:

	typedef struct DashCANInput { //0xF5
		uint8_t ButtonsArray;
		uint8_t RedRotary;
		uint8_t YellowRotary;
		uint8_t BlackRotary;
	} DashCANInput;

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

	//sends switch positions over CAN
	void transmitDashboardInfo();

	//Enables or diables interrupts for certain CAN messages(based on ID)?
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

	//upload logo to LCD controller
	void uploadLogoToController();

	//upload font to LCD controller
	void uploadArialBlackNumberFontToController();

	void waitingForCAN() {
		waitingForCAN(true);
	}
	void waitingForCAN(bool withLogo);

	//display the position of the most recently changed rotary switch
	void rotaryOverride();

	//flips byte order
	float swap(float d);
	uint32_t swap(uint32_t d);
	uint16_t swap(uint16_t d);

	void resetTimeoutTimer() {
		CAN_OVFCount = 0;
	}
};
