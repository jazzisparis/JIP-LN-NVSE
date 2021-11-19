#pragma once

enum
{
	kDeviceType_Keyboard = 1,
	kDeviceType_Mouse
};

enum
{
	// first 256 for keyboard, then 8 mouse buttons, then mouse wheel up, wheel down
	kMacro_MouseButtonOffset = 256,
	kMacro_MouseWheelOffset = kMacro_MouseButtonOffset + 8,

	kMaxMacros = kMacro_MouseWheelOffset + 2,
};

class DIHookControl
{
public:
	enum
	{
		// data sources
		kFlag_GameState =				1 << 0,	// input passed to game post-filtering
		kFlag_RawState =				1 << 1,	// user input
		kFlag_InsertedState =			1 << 2,	// keydown was inserted by script
		kFlag_Pressed =					kFlag_GameState | kFlag_RawState | kFlag_InsertedState,

		// modifiers
		kFlag_IgnoreDisabled_User =		1 << 3,	// ignore user-disabled keys
		kFlag_IgnoreDisabled_Script =	1 << 4,	// ignore script-disabled keys
		kFlag_IgnoreDisabled =			kFlag_IgnoreDisabled_User | kFlag_IgnoreDisabled_Script,

		kFlag_DefaultBackCompat =		kFlag_GameState,
	};

	enum
	{
		kDisable_User =		1 << 0,
		kDisable_Script =	1 << 1,

		kDisable_All =		kDisable_User | kDisable_Script,
	};

	bool __fastcall IsKeyPressed(UInt32 keycode, UInt32 flags = 1);
	bool IsKeyPressedRaw(UInt32 keycode);
	bool IsLMBPressed();
	bool IsKeyDisabled(UInt32 keycode);
	bool IsKeyHeld(UInt32 keycode);
	bool IsKeyTapped(UInt32 keycode);

	void SetKeyDisableState(UInt32 keycode, bool bDisable);
	void SetLMBDisabled(bool bDisable);
	void SetKeyHeldState(UInt32 keycode, bool bHold);
	void TapKey(UInt32 keycode);

private:
	struct KeyInfo
	{
		bool	rawState;		// state from dinput last update
		bool	gameState;		// state sent to the game last update
		bool	insertedState;	// true if a script pushed/held this key down last update

		bool	hold;			// key is held down
		bool	tap;			// key is being tapped
		bool	userDisable;	// key cannot be pressed by user
		bool	scriptDisable;	// key cannot be pressed by script
	};

	void		**vtbl;
	KeyInfo		m_keys[kMaxMacros];
};
static_assert(sizeof(DIHookControl) == 0x74C);

extern DIHookControl *g_DIHookCtrl;