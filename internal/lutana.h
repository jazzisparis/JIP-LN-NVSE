#pragma once

enum LNEvents
{
	kLNEventID_OnCellEnter,
	kLNEventID_OnCellExit,
	kLNEventID_OnPlayerGrab,
	kLNEventID_OnPlayerRelease,
	kLNEventID_OnCrosshairOn,
	kLNEventID_OnCrosshairOff,
	kLNEventID_OnButtonDown,
	kLNEventID_OnButtonUp,
	kLNEventID_OnKeyDown,
	kLNEventID_OnKeyUp,
	kLNEventID_OnControlDown,
	kLNEventID_OnControlUp,
	kLNEventID_OnTriggerDown,
	kLNEventID_OnTriggerUp,
	kLNEventID_Max =					kLNEventID_OnKeyDown,

	kLNEventMask_OnCellEnter =			1 << kLNEventID_OnCellEnter,
	kLNEventMask_OnCellExit =			1 << kLNEventID_OnCellExit,
	kLNEventMask_OnCell =				kLNEventMask_OnCellEnter | kLNEventMask_OnCellExit,
	kLNEventMask_OnPlayerGrab =			1 << kLNEventID_OnPlayerGrab,
	kLNEventMask_OnPlayerRelease =		1 << kLNEventID_OnPlayerRelease,
	kLNEventMask_OnPlayerGR =			kLNEventMask_OnPlayerGrab | kLNEventMask_OnPlayerRelease,
	kLNEventMask_OnCrosshairOn =		1 << kLNEventID_OnCrosshairOn,
	kLNEventMask_OnCrosshairOff =		1 << kLNEventID_OnCrosshairOff,
	kLNEventMask_OnCrosshair =			kLNEventMask_OnCrosshairOn | kLNEventMask_OnCrosshairOff,
	kLNEventMask_OnButtonDown =			1 << kLNEventID_OnButtonDown,
	kLNEventMask_OnButtonUp =			1 << kLNEventID_OnButtonUp,
	kLNEventMask_OnButton =				kLNEventMask_OnButtonDown | kLNEventMask_OnButtonUp,
	kLNEventMask_OnKeyDown =			1 << kLNEventID_OnKeyDown,
	kLNEventMask_OnKeyUp =				1 << kLNEventID_OnKeyUp,
	kLNEventMask_OnKey =				kLNEventMask_OnKeyDown | kLNEventMask_OnKeyUp,
	kLNEventMask_OnControlDown =		1 << kLNEventID_OnControlDown,
	kLNEventMask_OnControlUp =			1 << kLNEventID_OnControlUp,
	kLNEventMask_OnControl =			kLNEventMask_OnControlDown | kLNEventMask_OnControlUp,
	kLNEventMask_OnTriggerDown =		1 << kLNEventID_OnTriggerDown,
	kLNEventMask_OnTriggerUp =			1 << kLNEventID_OnTriggerUp,
	kLNEventMask_OnTrigger =			kLNEventMask_OnTriggerDown | kLNEventMask_OnTriggerUp,
};

struct LNEventData
{
	UInt8				eventID;
	UInt8				filterType;
	bool				remove;
	UInt8				byte03;
	LambdaVarContext	callback;
	union
	{
		TESForm			*form;
		tList<TESForm>	*list;
		UInt32			typeID;
	};

	LNEventData() {}
	LNEventData(UInt8 _eventID, Script *_script, bool _remove, UInt8 _filterType = 0, UInt32 _typeID = 0) :
		eventID(_eventID), filterType(_filterType), remove(_remove), callback(_script), typeID(_typeID) {}

	bool EvalFilter(TESObjectREFR *evalRefr, TESForm *evalBase) const;
};

class LNEventFinder
{
	LNEventData		*data;
	
public:
	LNEventFinder(LNEventData &evntData) : data(&evntData) {}

	bool operator==(const LNEventData &rhs) const
	{
		if (data->callback != rhs.callback)
			return false;
		if (data->remove && !rhs.filterType)
			return true;
		if (data->filterType != rhs.filterType)
			return false;
		if ((data->eventID < kLNEventID_OnButtonDown) || data->remove)
			return data->typeID == rhs.typeID;
		return (data->typeID & rhs.typeID) != 0;
	}
};

bool LNEventData::EvalFilter(TESObjectREFR *evalRefr, TESForm *evalBase) const
{
	switch (filterType)
	{
		case 0: return true;
		case 1: return form == evalRefr;
		case 2: return form == evalBase;
		case 3:
		{
			auto iter = list->Head();
			do
			{
				if ((iter->data == evalRefr) || (iter->data == evalBase))
					return true;
			}
			while (iter = iter->next);
			return false;
		}
		case 4: return typeID == evalBase->typeID;
		default: return false;
	}
}

struct LNDInpuCallbacks
{
	EventCallbackScripts	onDown;
	EventCallbackScripts	onUp;

	bool Empty() const {return onDown.Empty() && onUp.Empty();}
};

typedef Vector<LNEventData> LNEventCallbacks;
TempObject<LNEventCallbacks> s_LNEvents[kLNEventID_Max] = {{}, {}, {}, {}, {}, {}, {}, {}};

typedef Map<UInt32, LNDInpuCallbacks> LNDInputEventsMap;
TempObject<LNDInputEventsMap> s_LNOnKeyEvents, s_LNOnControlEvents;

TempObject<EventCallbackScripts> s_LNOnTriggerEvents[4] = {{}, {}, {}, {}};

TempObject<UnorderedMap<const char*, UInt32, 0x20, false>> s_LNEventNames;

const bool kValidKeyCode[] =
{
	0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 1, 0, 1, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1
};

const bool kValidFilterForm[] =
{
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
	1, 1, 1, 0, 0, 1, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0
};

UInt32 s_LNEventFlags = 0;
UInt32 s_inputEventClear = 0;

bool SetInputEventHandler(UInt32 eventMask, Script *script, SInt32 keyID, bool doAdd)
{
	bool onKey = (eventMask & kLNEventMask_OnKey) != 0;
	bool onUp = (eventMask == kLNEventMask_OnKeyUp) || (eventMask == kLNEventMask_OnControlUp);
	eventMask = onKey ? kLNEventMask_OnKey : kLNEventMask_OnControl;
	LNDInputEventsMap &events = onKey ? s_LNOnKeyEvents : s_LNOnControlEvents;
	bool result = false;
	if (keyID < 0)
	{
		if (!doAdd)
			for (auto iter = events.Begin(); iter; ++iter)
			{
				result |= onUp ? iter().onUp.Erase(script) : iter().onDown.Erase(script);
				if (iter().Empty())
					s_inputEventClear |= eventMask;
			}
		return result;
	}
	else if (onKey)
	{
		if ((keyID >= sizeof(kValidKeyCode)) || !kValidKeyCode[keyID])
			return false;
	}
	else if (keyID > 27)
		return false;
	LNDInpuCallbacks *callbacks;
	if (doAdd)
	{
		if (events.Insert(keyID, &callbacks))
			s_LNEventFlags |= eventMask;
		result = onUp ? callbacks->onUp.Insert(script) : callbacks->onDown.Insert(script);
	}
	else if (callbacks = events.GetPtr(keyID))
	{
		result = onUp ? callbacks->onUp.Erase(script) : callbacks->onDown.Erase(script);
		if (callbacks->Empty())
			s_inputEventClear |= eventMask;
	}
	return result;
}

//	Called by xNVSE
UInt32 __stdcall GetLNEventMask(const char *eventName)
{
	return s_LNEventNames->Get(eventName);
}

//	Called by xNVSE
bool __stdcall ProcessLNEventHandler(UInt32 eventMask, Script *udfScript, bool addEvt, TESForm *formFilter, UInt32 numFilter)
{
	if (eventMask >= kLNEventMask_OnKeyDown)
		return SetInputEventHandler(eventMask, udfScript, numFilter, addEvt);

	UInt8 eventID = 0;
	while (!((eventMask >> eventID) & 1)) eventID++;

	LNEventData evntData(eventID, udfScript, !addEvt);

	if (formFilter)
	{
		if IS_ID(formFilter, BGSListForm)
		{
			evntData.list = &((BGSListForm*)formFilter)->list;
			if (evntData.list->Empty())
				return false;
			evntData.filterType = 3;
		}
		else if (eventID <= kLNEventID_OnCellExit)
		{
			if NOT_ID(formFilter, TESObjectCELL)
				return false;
			evntData.filterType = 2;
			evntData.form = formFilter;
		}
		else if IS_REFERENCE(formFilter)
		{
			evntData.filterType = 1;
			evntData.form = formFilter;
		}
		else
		{
			if (!kValidFilterForm[formFilter->typeID])
				return false;
			evntData.filterType = 2;
			evntData.form = formFilter;
		}
	}
	else if (eventID >= kLNEventID_OnCrosshairOn)
		if (numFilter)
		{
			evntData.typeID = numFilter;
			if (eventID <= kLNEventID_OnCrosshairOff)
			{
				if ((evntData.typeID > kFormType_TESCaravanMoney) || !kValidFilterForm[evntData.typeID])
					return false;
			}
			else if (evntData.typeID > 0xFFFF)
				return false;
			evntData.filterType = 4;
		}
		else if ((eventID >= kLNEventID_OnButtonDown) && addEvt)
			return false;

	LNEventCallbacks *callbacks = &s_LNEvents[eventID]();
	if (addEvt)
	{
		if (callbacks->Find(LNEventFinder(evntData)))
			return false;
		*callbacks->Append() = evntData;
		s_LNEventFlags |= eventMask;
	}
	else
	{
		if (!callbacks->Remove(LNEventFinder(evntData)))
			return false;
		if (callbacks->Empty()) s_LNEventFlags &= ~eventMask;
	}
	return true;
}

bool s_gameLoadFlagLN = true;

void LN_ProcessEvents()
{
	static bool lastKeyState[kMaxMacros] = {0};

	if (s_LNEventFlags & kLNEventMask_OnKey)
	{
		if (s_inputEventClear & kLNEventMask_OnKey)
		{
			s_inputEventClear &= ~kLNEventMask_OnKey;
			for (auto onKeyClr = s_LNOnKeyEvents->BeginOp(); onKeyClr; ++onKeyClr)
				if (onKeyClr().Empty()) onKeyClr.Remove(s_LNOnKeyEvents);
			if (s_LNOnKeyEvents->Empty())
				s_LNEventFlags &= ~kLNEventMask_OnKey;
		}
		for (auto onKey = s_LNOnKeyEvents->BeginCp(); onKey; ++onKey)
		{
			UInt32 key = onKey.Key();
			bool currKeyState = g_DIHookCtrl->IsKeyPressedRaw(key);
			if (lastKeyState[key] == currKeyState) continue;
			if (currKeyState) onKey().onDown.InvokeEvents(key);
			else onKey().onUp.InvokeEvents(key);
			lastKeyState[key] = currKeyState;
		}
	}

	if (s_controllerReady)
	{
		if (s_LNEventFlags & kLNEventMask_OnButton)
		{
			static UInt16 lastButtonState = 0;
			UInt16 currButtonState = s_gamePad.wButtons, changes = currButtonState ^ lastButtonState;
			if (changes)
			{
				UInt16 cmprMask = changes & currButtonState;
				if (cmprMask)
					for (auto data = s_LNEvents[kLNEventID_OnButtonDown]->BeginCp(); data; ++data)
						if (UInt32 outMask = cmprMask & data().typeID)
							CallFunction(data().callback, nullptr, 1, outMask);
				if (cmprMask = changes & lastButtonState)
					for (auto data = s_LNEvents[kLNEventID_OnButtonUp]->BeginCp(); data; ++data)
						if (UInt32 outMask = cmprMask & data().typeID)
							CallFunction(data().callback, nullptr, 1, outMask);
				lastButtonState = currButtonState;
			}
		}

		if (s_LNEventFlags & kLNEventMask_OnTrigger)
		{
			static bool lastLTriggerState = 0, lastRTriggerState = 0;
			bool currTriggerState = s_gamePad.bLeftTrigger != 0;
			if (lastLTriggerState != currTriggerState)
			{
				s_LNOnTriggerEvents[currTriggerState]->InvokeEvents(0);
				lastLTriggerState = currTriggerState;
			}
			currTriggerState = s_gamePad.bRightTrigger != 0;
			if (lastRTriggerState != currTriggerState)
			{
				s_LNOnTriggerEvents[currTriggerState + 2]->InvokeEvents(1);
				lastRTriggerState = currTriggerState;
			}
		}
	}

	if (g_interfaceManager->currentMode > 1) return;

	static TESObjectCELL *lastCell = nullptr;
	TESObjectCELL *currCell = g_thePlayer->parentCell;
	if (!currCell) return;

	bool gameLoaded = s_gameLoadFlagLN;
	s_gameLoadFlagLN = false;

	TESObjectREFR *evalRefr;
	TESForm *evalBase;

	if (lastCell != currCell)
	{
		if (!gameLoaded)
		{
			if (s_LNEventFlags & kLNEventMask_OnCellExit)
				for (auto data = s_LNEvents[kLNEventID_OnCellExit]->BeginCp(); data; ++data)
					if (data().EvalFilter(nullptr, lastCell))
						CallFunction(data().callback, nullptr, 1, lastCell);
			if (s_LNEventFlags & kLNEventMask_OnCellEnter)
				for (auto data = s_LNEvents[kLNEventID_OnCellEnter]->BeginCp(); data; ++data)
					if (data().EvalFilter(nullptr, currCell))
						CallFunction(data().callback, nullptr, 1, currCell);
		}
		lastCell = currCell;
	}

	static TESObjectREFR *lastGrabbed = nullptr;
	static UInt32 lastGrabID = 0;
	TESObjectREFR *currGrabbed = g_thePlayer->grabbedRef;

	if (lastGrabbed != currGrabbed)
	{
		if (!gameLoaded)
		{
			if (lastGrabbed && (s_LNEventFlags & kLNEventMask_OnPlayerRelease))
			{
				if (lastGrabbed == LookupFormByRefID(lastGrabID))
				{
					evalRefr = lastGrabbed;
					evalBase = lastGrabbed->baseForm;
					for (auto data = s_LNEvents[kLNEventID_OnPlayerRelease]->BeginCp(); data; ++data)
						if (data().EvalFilter(evalRefr, evalBase)) CallFunction(data().callback, nullptr, 1, lastGrabbed);
				}
			}
			if (currGrabbed && (s_LNEventFlags & kLNEventMask_OnPlayerGrab))
			{
				evalBase = currGrabbed->baseForm;
				for (auto data = s_LNEvents[kLNEventID_OnPlayerGrab]->BeginCp(); data; ++data)
					if (data().EvalFilter(currGrabbed, evalBase)) CallFunction(data().callback, nullptr, 1, currGrabbed);
			}
		}
		lastGrabbed = currGrabbed;
		if (currGrabbed) lastGrabID = currGrabbed->refID;
	}

	static TESObjectREFR *lastCrosshair = nullptr;
	static UInt32 lastCrshrID = 0;
	TESObjectREFR *currCrosshair = g_interfaceManager->crosshairRef;

	if (lastCrosshair != currCrosshair)
	{
		if (!gameLoaded)
		{
			if (lastCrosshair && (s_LNEventFlags & kLNEventMask_OnCrosshairOff))
			{
				if (lastCrosshair == LookupFormByRefID(lastCrshrID))
				{
					evalRefr = lastCrosshair;
					evalBase = lastCrosshair->GetBaseForm();
					for (auto data = s_LNEvents[kLNEventID_OnCrosshairOff]->BeginCp(); data; ++data)
						if (data().EvalFilter(evalRefr, evalBase)) CallFunction(data().callback, nullptr, 1, lastCrosshair);
				}
			}
			if (currCrosshair && (s_LNEventFlags & kLNEventMask_OnCrosshairOn))
			{
				evalBase = currCrosshair->GetBaseForm();
				for (auto data = s_LNEvents[kLNEventID_OnCrosshairOn]->BeginCp(); data; ++data)
					if (data().EvalFilter(currCrosshair, evalBase)) CallFunction(data().callback, nullptr, 1, currCrosshair);
			}
		}
		lastCrosshair = currCrosshair;
		if (currCrosshair) lastCrshrID = currCrosshair->refID;
	}
	
	static bool lastCtrlState[MAX_CONTROL_BIND] = {0};

	if (s_LNEventFlags & kLNEventMask_OnControl)
	{
		if (s_inputEventClear & kLNEventMask_OnControl)
		{
			s_inputEventClear &= ~kLNEventMask_OnControl;
			for (auto onCtrlClr = s_LNOnControlEvents->BeginOp(); onCtrlClr; ++onCtrlClr)
				if (onCtrlClr().Empty()) onCtrlClr.Remove(s_LNOnControlEvents);
			if (s_LNOnControlEvents->Empty())
				s_LNEventFlags &= ~kLNEventMask_OnControl;
		}
		for (auto onCtrl = s_LNOnControlEvents->BeginCp(); onCtrl; ++onCtrl)
		{
			UInt32 ctrl = onCtrl.Key();
			bool currCtrlState = IsControlPressedRaw(ctrl);
			if (lastCtrlState[ctrl] == currCtrlState) continue;
			if (currCtrlState) onCtrl().onDown.InvokeEvents(ctrl);
			else onCtrl().onUp.InvokeEvents(ctrl);
			lastCtrlState[ctrl] = currCtrlState;
		}
	}
}