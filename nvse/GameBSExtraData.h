#pragma once

// Added to remove a cyclic dependency between GameForms.h and GameExtraData.h

class ExtraHealth;
class ExtraWeaponModFlags;

// 0C
class BSExtraData
{
public:
	BSExtraData();
	~BSExtraData();

	virtual void	Destroy(bool doFree);
	virtual bool	IsDifferentType(BSExtraData *compareTo);

	static BSExtraData* Create(UInt8 xType, UInt32 size, UInt32 vtbl);

	UInt8			type;		// 04
	UInt8			pad05[3];	// 05
	BSExtraData		*next;		// 08
};

// 020
struct BaseExtraList
{
	virtual void	Destroy(bool doFree);

	BSExtraData		*m_data;					// 04
	UInt8			m_presenceBitfield[0x15];	// 08 - if a bit is set, then the extralist should contain that extradata
	UInt8			flags;						// 1D	0x61 in TESObjectREFR
	UInt8			byte1E;						// 1E
	UInt8			byte1F;						// 1F

	bool HasType(UInt32 type) const;
	bool MarkScriptEvent(UInt32 eventMask, TESForm *eventTarget);
	void DebugDump() const;
	bool IsWorn() const;
	char GetExtraFactionRank(TESFaction *faction) const;
	SInt32 GetCount() const;
};

struct ExtraDataList : public BaseExtraList
{
	ExtraDataList *CreateCopy();
	static ExtraDataList* __stdcall Create(BSExtraData *xBSData = NULL);
};
STATIC_ASSERT(sizeof(ExtraDataList) == 0x020);

typedef BSExtraData* (__thiscall *_GetExtraData)(const BaseExtraList*, UInt8);
extern const _GetExtraData GetExtraData;

typedef BSExtraData* (__thiscall *_AddExtraData)(BaseExtraList*, BSExtraData*);
extern const _AddExtraData AddExtraData;

typedef void (__thiscall *_RemoveExtraData)(BaseExtraList*, BSExtraData*, bool);
extern const _RemoveExtraData RemoveExtraData;

typedef void (__thiscall *_RemoveExtraType)(BaseExtraList*, UInt8);
extern const _RemoveExtraType RemoveExtraType;

typedef void (__thiscall *_ClearExtraDataList)(BaseExtraList*, bool);
extern const _ClearExtraDataList ClearExtraDataList;

typedef void (__thiscall *_CopyExtraDataList)(BaseExtraList*, const BaseExtraList*);
extern const _CopyExtraDataList CopyExtraDataList;