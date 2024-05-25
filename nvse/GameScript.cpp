#include "nvse/GameScript.h"

__declspec(naked) void SuppressConsoleOutput()
{
	__asm
	{
		mov		eax, fs:0x2C
		mov		edx, g_TLSIndex
		mov		eax, [eax+edx*4]
		mov		[eax+0x268], 0
		retn
	}
}

TESForm* __stdcall LookupFormByRefID(UInt32 refID);
void Script::RefVariable::Resolve(ScriptLocals *eventList)
{
	if (varIdx && eventList)
		if (ScriptVar *var = eventList->GetVariable(varIdx))
			form = LookupFormByRefID(var->data.refID);
}

bool Script::Compile(const char *scrName)
{
	ScriptBuffer scrBuffer;
	scrBuffer.scriptText = text;
	scrBuffer.runtimeMode = 0;
	scrBuffer.scriptName.Set(scrName);
	scrBuffer.partialScript = true;
	scrBuffer.currScript = this;
	return StdCall<bool>(0x5AEB90, this, &scrBuffer);
}

bool Script::Init(char *scrText, const char *scrName)
{
	Constructor();
	MarkAsTemporary();
	text = scrText;
	bool success = Compile(scrName) && info.dataLength;
	text = nullptr;
	return success;
}

Script *Script::Create(char *scrText, const char *scrName)
{
	Script *pScript = Game_HeapAlloc<Script>();
	if (pScript->Init(scrText, scrName))
	{
		pScript->SetEditorID(scrName);
		return pScript;
	}
	pScript->Destroy(1);
	return nullptr;
}

class ScriptVarFinder
{
	const char	*m_varName;

public:
	ScriptVarFinder(const char *varName) : m_varName(varName) {}

	bool Accept(VariableInfo *varInfo)
	{
		return !StrCompareCI(varInfo->name.m_data, m_varName);
	}
};

VariableInfo *Script::GetVariableByName(const char *varName) const
{
	auto varIter = varList.Head();
	do
	{
		if (VariableInfo *varInfo = varIter->data; varInfo && !StrCompareCI(varInfo->name.m_data, varName))
			return varInfo;
	}
	while (varIter = varIter->next);
	return NULL;
}

Script::RefVariable	*Script::GetVariable(UInt32 reqIdx) const
{
	UInt32 idx = 1;	// yes, really starts at 1
	if (reqIdx)
	{
		auto varIter = refList.Head();
		do
		{
			if (idx == reqIdx)
				return varIter->data;
			idx++;
		}
		while (varIter = varIter->next);
	}
	return NULL;
}

VariableInfo *Script::GetVariableInfo(UInt32 idx) const
{
	auto varIter = varList.Head();
	do
	{
		if (VariableInfo *varInfo = varIter->data; varInfo && (varInfo->idx == idx))
			return varInfo;
	}
	while (varIter = varIter->next);
	return NULL;
}

UInt32 Script::AddVariable(TESForm *form)
{
	RefVariable	*refVar = Game_HeapAlloc<RefVariable>();
	refVar->name.Set("");
	refVar->form = form;
	refVar->varIdx = 0;

	UInt32 resultIdx = refList.Append(refVar) + 1;
	info.numRefs = resultIdx + 1;
	return resultIdx;
}

UInt32 Script::RefVarList::GetIndex(RefVariable *refVar) const
{
	UInt32 idx = 0;
	auto varIter = Head();
	do
	{
		idx++;
		if (varIter->data == refVar)
			return idx;
	}
	while (varIter = varIter->next);
	return 0;
}

UInt32 Script::GetDataLength() const
{
	UInt8 *dataPtr = data;
	if (*(UInt32*)dataPtr != 0x1D) return 0;
	dataPtr += 4;
	while (*(UInt16*)dataPtr == 0x10)
	{
		dataPtr += *(UInt16*)(dataPtr + 2) + *(UInt16*)(dataPtr + 6);
		if (*(UInt32*)dataPtr != 0x11) return 0;
		dataPtr += 4;
	}
	return dataPtr - data;
}