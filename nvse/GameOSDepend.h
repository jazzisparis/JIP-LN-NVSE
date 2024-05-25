#pragma once

#define MAX_CONTROL_BIND 28

enum ControlCode
{
	/*00*/eCtrlID_Forward,
	/*01*/eCtrlID_Backward,
	/*02*/eCtrlID_Left,
	/*03*/eCtrlID_Right,
	/*04*/eCtrlID_Attack,
	/*05*/eCtrlID_Activate,
	/*06*/eCtrlID_AimBlock,
	/*07*/eCtrlID_ReadyItem,
	/*08*/eCtrlID_Crouch,
	/*09*/eCtrlID_Run,
	/*0A*/eCtrlID_AlwaysRun,
	/*0B*/eCtrlID_AutoMove,
	/*0C*/eCtrlID_Jump,
	/*0D*/eCtrlID_TogglePOV,
	/*0E*/eCtrlID_MenuMode,
	/*0F*/eCtrlID_SleepWait,
	/*10*/eCtrlID_VATS_,
	/*11*/eCtrlID_Hotkey1,
	/*12*/eCtrlID_AmmoSwap,
	/*13*/eCtrlID_Hotkey3,
	/*14*/eCtrlID_Hotkey4,
	/*15*/eCtrlID_Hotkey5,
	/*16*/eCtrlID_Hotkey6,
	/*17*/eCtrlID_Hotkey7,
	/*18*/eCtrlID_Hotkey8,
	/*19*/eCtrlID_QuickSave,
	/*1A*/eCtrlID_QuickLoad,
	/*1B*/eCtrlID_Grab
};

enum KeyState
{
	kKeyState_Held,
	kKeyState_Pressed,
	kKeyState_Depressed,
	kKeyState_Changed
};

enum ControllerState
{
	kCtrlState_NotHeld = -1,
	kCtrlState_Held,
	kCtrlState_Depressed,
	kCtrlState_Pressed
};

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

enum XboxButtonBit
{
	kXboxBit_DPAD_UP,
	kXboxBit_DPAD_DOWN,
	kXboxBit_DPAD_LEFT,
	kXboxBit_DPAD_RIGHT,
	kXboxBit_START,
	kXboxBit_BACK,
	kXboxBit_LS_BUTTON,
	kXboxBit_RS_BUTTON,
	kXboxBit_LB,
	kXboxBit_RB,
	kXboxBit_GUIDE,
	kXboxBit_BUTTON_A = 0xC,
	kXboxBit_BUTTON_B,
	kXboxBit_BUTTON_X,
	kXboxBit_BUTTON_Y
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

enum SpecialInputCode : UInt32
{
	kInputCode_Backspace =	0x80000000,
	kInputCode_ArrowLeft =	0x80000001,
	kInputCode_ArrowRight =	0x80000002,
	kInputCode_ArrowUp =	0x80000003,
	kInputCode_ArrowDown =	0x80000004,
	kInputCode_Home =		0x80000005,
	kInputCode_End =		0x80000006,
	kInputCode_Delete =		0x80000007,
	kInputCode_Enter =		0x80000008,
	kInputCode_PageUp =		0x80000009,
	kInputCode_PageDown =	0x8000000A
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
	UInt8				mouseSensitivity;		// 1B50
	UInt8				pad1B51[3];				// 1B51
	UInt32				unk1B54;				// 1B54
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

	__forceinline static OSInputGlobals *GetSingleton() {return *(OSInputGlobals**)0x11F35CC;}
};
static_assert(sizeof(OSInputGlobals) == 0x1C04);

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
	virtual bool	QueueTask(BSPackedTask *pTask);
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

// 10
struct TaskQueueInterface
{
	BSTCommonScrapHeapMessageQueue	*taskQueueMain;	// 00
	BSTCommonScrapHeapMessageQueue	*taskQueueScnd;	// 04
	BSTCommonScrapHeapMessageQueue	*taskQueue;		// 08	Same as main
	UInt32							threadID;		// 0C

	__forceinline static TaskQueueInterface *GetSingleton() {return *(TaskQueueInterface**)0x11DF1A8;}
};
extern TaskQueueInterface *g_scrapHeapQueue;

// A4
class OSGlobals
{
public:
	UInt8							oneMore;		// 00
	UInt8							quitGame;		// 01
	UInt8							exitToMainMenu;	// 02
	UInt8							isWindowActive;	// 03
	UInt8							byte04;			// 04
	UInt8							byte05;			// 05
	UInt8							tfcState;		// 06
	bool							freezeTime;		// 07
	HWND							window;			// 08
	HINSTANCE						procInstance;	// 0C
	UInt32							mainThreadID;	// 10
	HANDLE							mainThreadHandle;	// 14
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

	__forceinline static OSGlobals *GetSingleton() {return *(OSGlobals**)0x11DEA0C;}
};
static_assert(sizeof(OSGlobals) == 0xA4);

extern OSGlobals *g_OSGlobals;