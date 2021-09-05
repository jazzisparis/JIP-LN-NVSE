#pragma once

struct alignas(16) XINPUT_GAMEPAD_EX
{
	UInt32		eventCount;		// 00
	UInt16		wButtons;		// 04
	UInt8		bLeftTrigger;	// 06
	UInt8		bRightTrigger;	// 07
	SInt16		sThumbLX;		// 08
	SInt16		sThumbLY;		// 0A
	SInt16		sThumbRX;		// 0C
	SInt16		sThumbRY;		// 0E
};
extern XINPUT_GAMEPAD_EX s_gamePad;

extern bool s_controllerReady;

typedef UInt32 (__stdcall *_XInputGetStateEx)(UInt32, XINPUT_GAMEPAD_EX*);
extern _XInputGetStateEx XInputGetStateEx;

extern int s_deadZoneLS, s_deadZoneRS;
extern double s_deadZoneLSg, s_deadZoneRSg, s_deadZoneLSd, s_deadZoneRSd;

struct XInputStateMods
{
	UInt16		buttonSkip;		// 00
	UInt16		buttonHold;		// 02
	UInt16		buttonTap;		// 04
	UInt8		triggerMods;	// 06
	UInt8		stickSkip;		// 07
};
extern XInputStateMods s_XIStateMods;

UInt32 __stdcall Hook_XInputGetState(UInt32 index, XINPUT_GAMEPAD_EX *currState);

bool HookXInput();

bool __fastcall GetXIControlPressed(XINPUT_GAMEPAD_EX *xiState, UInt32 ctrlID);

bool __fastcall GetXIControlDisabled(UInt32 ctrlID);

void __fastcall SetXIControlDisabled(UInt32 ctrlID, bool bDisable);

void __fastcall SetXIControlHeld(UInt32 ctrlID, bool bHold);

bool __fastcall TapXIControl(UInt32 ctrlID);