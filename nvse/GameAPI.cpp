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
	return *bEchoConsole != 0;
}

void SetConsoleEcho(bool doEcho)
{
	*bEchoConsole = doEcho ? 1 : 0;
}

ConsoleManager * ConsoleManager::GetSingleton(void)
{
	return (ConsoleManager *)ConsoleManager_GetSingleton(true);
}

void Console_Print(const char * fmt, ...)
{
	ConsoleManager	* mgr = ConsoleManager::GetSingleton();
	if(mgr)
	{
		va_list	args;

		va_start(args, fmt);

		CALL_MEMBER_FN(mgr, Print)(fmt, args);

		va_end(args);
	}
}

TESSaveLoadGame * TESSaveLoadGame::Get()
{
	return (TESSaveLoadGame *)0x011DE45C;
}

SaveGameManager* SaveGameManager::GetSingleton()
{
	return *(SaveGameManager**)0x11DE134;
}

UInt32 ScriptEventList::ResetAllVariables()
{
	if (!m_vars) return 0;
	ListNode<ScriptVar> *varIter = m_vars->Head();
	ScriptVar *scriptVar;
	UInt32 numVars = 0;
	do
	{
		scriptVar = varIter->data;
		if (scriptVar)
		{
			scriptVar->data.num = 0;
			numVars++;
		}
	}
	while (varIter = varIter->next);
	return numVars;
}

ScriptVar *ScriptEventList::GetVariable(UInt32 id)
{
	if (m_vars)
	{
		ListNode<ScriptVar> *varIter = m_vars->Head();
		ScriptVar *scriptVar;
		do
		{
			scriptVar = varIter->data;
			if (scriptVar && (scriptVar->id == id))
				return scriptVar;
		}
		while (varIter = varIter->next);
	}
	return NULL;
}
