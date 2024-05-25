#include "nvse/GameAPI.h"

// arg1 = 1, ignored if canCreateNew is false, passed to 'init' function if a new object is created
typedef void * (* _GetSingleton)(bool canCreateNew);

const _ExtractArgs ExtractArgs = (_ExtractArgs)0x005ACCB0;

const _CreateFormInstance CreateFormInstance = (_CreateFormInstance)0x00465110;

const _GetSingleton ConsoleManager_GetSingleton = (_GetSingleton)0x0071B160;
bool * bEchoConsole = (bool*)0x011F158C;

const _QueueUIMessage QueueUIMessage = (_QueueUIMessage)0x007052F0;

const _ShowMessageBox ShowMessageBox = (_ShowMessageBox)0x00703E80;
const _ShowMessageBox_Callback ShowMessageBox_Callback = (_ShowMessageBox_Callback)0x005B4A70;
const _ShowMessageBox_pScriptRefID ShowMessageBox_pScriptRefID = (_ShowMessageBox_pScriptRefID)0x011CAC64;
const _ShowMessageBox_button ShowMessageBox_button = (_ShowMessageBox_button)0x0118C684;

bool GetConsoleEcho()
{
	return *bEchoConsole;
}

void SetConsoleEcho(bool doEcho)
{
	*bEchoConsole = doEcho;
}

ConsoleManager * ConsoleManager::GetSingleton(void)
{
	return (ConsoleManager *)ConsoleManager_GetSingleton(true);
}

void Console_Print(const char *fmt, ...)
{
	va_list	args;
	va_start(args, fmt);
	ThisCall(0x71D0A0, g_consoleManager, fmt, args);
	va_end(args);
}

void ConsoleManager::Clear()
{
	ThisCall(0x71E070, &printedLines);
	unk024 = 0;
	ThisCall(0x71D410, this);
}

SaveGameManager* SaveGameManager::GetSingleton()
{
	return *(SaveGameManager**)0x11DE134;
}

UInt32 ScriptLocals::ResetAllVariables()
{
	if (!m_vars) return 0;
	auto varIter = m_vars->Head();
	ScriptVar *scriptVar;
	UInt32 numVars = 0;
	do
	{
		scriptVar = varIter->data;
		if (scriptVar)
		{
			scriptVar->data = 0.0;
			numVars++;
		}
	}
	while (varIter = varIter->next);
	return numVars;
}

__declspec(naked) ScriptVar* __fastcall ScriptLocals::GetVariable(UInt32 id) const
{
	__asm
	{
		mov		eax, [ecx+0xC]
		ALIGN 16
	iterHead:
		test	eax, eax
		jz		notFound
		mov		ecx, [eax]
		mov		eax, [eax+4]
		test	ecx, ecx
		jz		iterHead
		cmp		[ecx], edx
		jnz		iterHead
		mov		eax, ecx
	notFound:
		retn
	}
}

ScriptLocals *ScriptLocals::CreateCopy()
{
	ScriptLocals *pEventList = Game_HeapAlloc<ScriptLocals>();
	pEventList->m_script = m_script;
	pEventList->m_flags = m_flags;
	pEventList->m_eventList = nullptr;
	pEventList->m_vars = nullptr;
	pEventList->m_effScrFlags = nullptr;
	if (m_eventList)
	{
		auto eventList = Game_HeapAlloc<tList<Event>::Node>();
		eventList->data = nullptr;
		eventList->next = nullptr;
		pEventList->m_eventList = (EventList*)eventList;
		auto evtIter = m_eventList->Head();
		Event *pEvent;
		do
		{
			if (!evtIter->data) continue;
			pEvent = Game_HeapAlloc<Event>();
			*pEvent = *evtIter->data;
			if (eventList->data)
				eventList = eventList->Append(pEvent);
			else eventList->data = pEvent;
		}
		while (evtIter = evtIter->next);
	}
	if (m_vars)
	{
		auto pVars = Game_HeapAlloc<tList<ScriptVar>::Node>();
		pVars->data = nullptr;
		pVars->next = nullptr;
		pEventList->m_vars = (VarList*)pVars;
		auto varIter = m_vars->Head();
		ScriptVar *pVar;
		do
		{
			if (!varIter->data) continue;
			pVar = Game_HeapAlloc<ScriptVar>();
			*pVar = *varIter->data;
			if (pVars->data)
				pVars = pVars->Append(pVar);
			else pVars->data = pVar;
		}
		while (varIter = varIter->next);
	}
	if (m_effScrFlags)
	{
		pEventList->m_effScrFlags = Game_HeapAlloc<EffectScriptFlags>();
		*pEventList->m_effScrFlags = *m_effScrFlags;
	}
	return pEventList;
}

BGSSaveLoadGame *g_BGSSaveLoadGame;

UInt32 *NiTPtrMap<UInt32>::Lookup(UInt32 key) const;
void NiTPtrMap<UInt32>::Insert(UInt32 key, UInt32 value);

__declspec(naked) UInt32 __fastcall BGSSaveLoadGame::EncodeRefID(UInt32 *pRefID)
{
	__asm
	{
		mov		eax, [edx]
		cmp		eax, 0xFF000000
		jnb		isCreated
		mov		ecx, [ecx+8]
		push	esi
		push	edi
		mov		esi, ecx
		mov		edi, edx
		push	eax
		call	NiTPtrMap<UInt32>::Lookup
		test	eax, eax
		jnz		found
		mov		eax, [esi+0x20]
		inc		dword ptr [esi+0x20]
		mov		edx, [edi]
		push	eax
		push	edx
		push	eax
		push	eax
		push	edx
		mov		ecx, esi
		call	NiTPtrMap<UInt32>::Insert
		lea		ecx, [esi+0x10]
		call	NiTPtrMap<UInt32>::Insert
		pop		eax
	found:
		mov		[edi], eax
		pop		edi
		pop		esi
	isCreated:
		retn
	}
}

__declspec(naked) UInt32 __fastcall BGSSaveLoadGame::DecodeRefID(UInt32 *pRefID)
{
	__asm
	{
		mov		eax, [edx]
		test	eax, eax
		js		isCreated
		push	edx
		push	eax
		mov		ecx, [ecx+8]
		add		ecx, 0x10
		call	NiTPtrMap<UInt32>::Lookup
		pop		ecx
		mov		[ecx], eax
	isCreated:
		retn
	}
}