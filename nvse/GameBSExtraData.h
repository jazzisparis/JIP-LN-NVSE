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

struct NPCPerksInfo;

// 020
struct BaseExtraList
{
	virtual void	Destroy(bool doFree);

	BSExtraData		*m_data;					// 04
	UInt8			m_presenceBitfield[0x13];	// 08 - if a bit is set, then the extralist should contain that extradata
	UInt8			jipRefFlags5F;				// 1B	0x5F in TESObjectREFR
	NPCPerksInfo	*perksInfo;					// 1C	JIP only!

	bool HasType(UInt32 type) const;
	BSExtraData *GetByType(UInt32 xType) const;
	__forceinline BSExtraData *AddExtra(BSExtraData *toAdd)
	{
		return ThisCall<BSExtraData*>(0x40FF60, this, toAdd);
	}
	__forceinline void RemoveExtra(BSExtraData *toRemove, bool doFree)
	{
		ThisCall(0x410020, this, toRemove, doFree);
	}
	__forceinline void RemoveByType(UInt8 xType)
	{
		ThisCall(0x410140, this, xType);
	}
	__forceinline void RemoveAll(bool doFree)
	{
		ThisCall(0x40FAE0, this, doFree);
	}
	__forceinline void CopyFrom(const BaseExtraList *sourceList)
	{
		ThisCall(0x411EC0, this, sourceList);
	}
	void DebugDump() const;
	char GetExtraFactionRank(TESFaction *faction) const;
	SInt32 GetCount() const;
};

struct ExtraDataList : public BaseExtraList
{
	ExtraDataList *CreateCopy();
	static ExtraDataList* __stdcall Create(BSExtraData *xBSData = NULL);
};
STATIC_ASSERT(sizeof(ExtraDataList) == 0x020);