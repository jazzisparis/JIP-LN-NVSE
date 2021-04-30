#pragma once

// keeping this in a separate file so we don't need to include dinput/dsound everywhere

#define DIRECTINPUT_VERSION 0x0800
#define DIRECTSOUND_VERSION 0x0800

#define MAX_CONTROL_BIND 28

enum XboxControlCode
{
	kXboxCtrl_DPAD_UP = 1,
	kXboxCtrl_DPAD_DOWN,
	kXboxCtrl_DPAD_RIGHT = 4,
	kXboxCtrl_DPAD_LEFT,
	kXboxCtrl_START,
	kXboxCtrl_BACK,
	kXboxCtrl_LS_BUTTON,
	kXboxCtrl_RS_BUTTON,
	kXboxCtrl_BUTTON_A,
	kXboxCtrl_BUTTON_B,
	kXboxCtrl_BUTTON_X,
	kXboxCtrl_BUTTON_Y,
	kXboxCtrl_RB,
	kXboxCtrl_LB,
	kXboxCtrl_LT,
	kXboxCtrl_RT,
	kXboxCtrl_LS_UP = 0x13,
	kXboxCtrl_LS_DOWN,
	kXboxCtrl_LS_RIGHT = 0x16,
	kXboxCtrl_LS_LEFT,
};

enum XboxButtonMask
{
	kXboxMask_DPAD_UP = 1,
	kXboxMask_DPAD_DOWN = 2,
	kXboxMask_DPAD_LEFT = 4,
	kXboxMask_DPAD_RIGHT = 8,
	kXboxMask_START = 0x10,
	kXboxMask_BACK = 0x20,
	kXboxMask_LS_BUTTON = 0x40,
	kXboxMask_RS_BUTTON = 0x80,
	kXboxMask_LB = 0x100,
	kXboxMask_RB = 0x200,
	kXboxMask_GUIDE = 0x400,
	kXboxMask_BUTTON_A = 0x1000,
	kXboxMask_BUTTON_B = 0x2000,
	kXboxMask_BUTTON_X = 0x4000,
	kXboxMask_BUTTON_Y = 0x8000,
};

struct _IDirectInput8;

class DInputInterface
{
public:
	virtual void	Unk_00(void);
	virtual void	Unk_01(void);
	virtual void	Unk_02(void);
	virtual void	Unk_03(void);
	virtual void	Unk_04(void);
	virtual void	Unk_05(void);
	virtual void	Unk_06(DInputInterface *intrfc, UInt32 arg2, void *arg3);
	virtual UInt32	GetStatesChanged(DInputInterface *intrfc);
	virtual void	Unk_08(void);
	virtual void	OutputStates(DInputInterface *intrfc, UInt32 bufSize, char *buffer);
	virtual void	Unk_0A(void);
	virtual void	Unk_0B(DInputInterface *intrfc, void *arg2);
	virtual void	Unk_0C(void);
	virtual void	Unk_0D(DInputInterface *intrfc, HWND window, UInt32 type);
	virtual void	Unk_0E(void);
	virtual void	Unk_0F(void);
	virtual void	Unk_10(void);
	virtual void	Unk_11(void);
	virtual void	Unk_12(void);
	virtual void	Unk_13(void);
	virtual void	Unk_14(void);
	virtual void	Unk_15(void);
	virtual void	Unk_16(void);
	virtual void	Unk_17(void);
	virtual void	Unk_18(void);
	virtual void	QueryStates(DInputInterface *intrfc);
};

// 1C04
class OSInputGlobals
{
public:
	enum
	{
		kFlag_HasJoysticks =	1 << 0,
		kFlag_HasMouse =		1 << 1,
		kFlag_HasKeyboard =		1 << 2,
		kFlag_BackgroundMouse =	1 << 3,
	};

	UInt8				controllerDisabled;		// 0000
	UInt8				pad0001[3];				// 0001
	UInt32				flags;					// 0004
	_IDirectInput8		*directInput;			// 0008
	void				*ptr000C[8];			// 000C
	DInputInterface		*keyboardInterface;		// 002C
	DInputInterface		*mouseInterface;		// 0030
	UInt32				unk0034[320];			// 0034
	UInt32				unk0534[1264];			// 0534
	UInt32				unk18F4;				// 18F4
	UInt8				currKeyStates[256];		// 18F8
	UInt8				lastKeyStates[256];		// 19F8
	UInt32				unk1AF8[11];			// 1AF8
	int					currMouseMovementX;		// 1B24
	int					currMouseMovementY;		// 1B28
	int					currMouseWheelScroll;	// 1B2C
	UInt8				currButtonStates[8];	// 1B30
	int					lastMouseMovementX;		// 1B38
	int					lastMouseMovementY;		// 1B3C
	int					lastMouseWheelScroll;	// 1B40
	UInt8				lastButtonStates[8];	// 1B44
	UInt32				ltrtButtonState;		// 1B4C
	UInt32				unk1B50[2];				// 1B50
	UInt8				buttonStates1B58[8];	// 1B58
	UInt32				unk1B60[8];				// 1B60
	UInt32				*controllerVibration;	// 1B80
	UInt32				unk1B84;				// 1B84
	UInt8				controllerNotDisabled;	// 1B88
	UInt8				pad1B89[3];				// 1B89
	UInt32				unk1B8C[2];				// 1B8C
	UInt8				keyBinds[28];			// 1B94
	UInt8				mouseBinds[28];			// 1BB0
	UInt8				joystickBinds[28];		// 1BCC
	UInt8				controllerBinds[28];	// 1BE8
};
STATIC_ASSERT(sizeof(OSInputGlobals) == 0x1C04);

extern OSInputGlobals *g_inputGlobals;
#define KEYBOARD_BIND(ctrlID) g_inputGlobals->keyBinds[ctrlID]
#define MOUSE_BIND(ctrlID) g_inputGlobals->mouseBinds[ctrlID]
#define CONTROLLER_BIND(ctrlID) g_inputGlobals->controllerBinds[ctrlID]

// 20
struct BSPackedTask
{
	union TaskArg
	{
		UInt8		byteVal;
		SInt32		intVal;
		UInt32		uintVal;
		float		fltVal;
		void		*ptrVal;
		TESForm		*formVal;
	};

	UInt32			type;		// 00
	UInt32			cmdOpcode;	// 04
	UInt32			thisObj;	// 08	refID
	TaskArg			args[5];	// 0C
};

// 10
struct ScrapHeapBuffer
{
	BSPackedTask	*buffer;
	BSPackedTask	*firstFree;
	BSPackedTask	*bufferEnd;
	UInt32			unk0C;
};

// 28
class BSTCommonScrapHeapMessageQueue
{
public:
	virtual void	Destroy(bool doFree);
	virtual void	Unk_01(void);
	virtual void	Unk_02(void);
	virtual void	Unk_03(void);
	virtual void	Unk_04(void);
	virtual void	Unk_05(void);
	virtual void	Unk_06(void);

	typedef void (__cdecl *DoQueuedTask)(BSPackedTask *taskData);

	UInt32				unk04;			// 04
	ScrapHeapBuffer		*taskBuffer;	// 08
	UInt32				unk0C;			// 0C
	void				*ptr10;			// 10
	UInt32				currCount;		// 14
	HANDLE				semaphore;		// 18
	UInt32				maxCount;		// 1C
	DoQueuedTask		doQueuedTask;	// 20	0x87B990
	UInt8				byte24;			// 24
	UInt8				pad25[3];		// 25
};

// A4
class OSGlobals
{
public:
	UInt8							oneMore;		// 00
	UInt8							quitGame;		// 01
	UInt8							exitToMainMenu;	// 02
	UInt8							byte03;			// 03
	UInt8							byte04;			// 04
	UInt8							byte05;			// 05
	UInt8							tfcState;		// 06
	bool							freezeTime;		// 07
	HWND							window;			// 08
	HINSTANCE						procInstance;	// 0C
	UInt32							mainThreadID;	// 10
	UInt32							unk14;			// 14
	ScrapHeapBuffer					shBuffer18;		// 18
	BSTCommonScrapHeapMessageQueue	shQueue28;		// 28
	ScrapHeapBuffer					shBuffer50;		// 50
	BSTCommonScrapHeapMessageQueue	shQueue60;		// 60
	BSShaderAccumulator				*shaderAccum88;	// 88
	BSShaderAccumulator				*shaderAccum8C;	// 8C
	BSShaderAccumulator				*shaderAccum90;	// 90
	BSShaderAccumulator				*shaderAccum94;	// 94
	BSShaderAccumulator				*shaderAccum98;	// 98
	UInt32							unk9C;			// 9C
	NiCamera						*cameraA0;		// A0
};
STATIC_ASSERT(sizeof(OSGlobals) == 0xA4);