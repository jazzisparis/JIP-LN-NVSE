#pragma once

// Added to remove a cyclic dependency between GameForms.h and GameExtraData.h

// 0C
class BSExtraData
{
public:
	virtual void	Destroy(bool doFree) = 0;
	virtual bool	Differs(BSExtraData *compareTo) = 0;

	UInt8			type;		// 04
	UInt8			pad05[3];	// 05
	BSExtraData		*next;		// 08
};

struct NPCPerksInfo;

// 020
struct BaseExtraList
{
	virtual void	Destroy(bool doFree);

	BSExtraData			*m_data;					// 04
	UInt8				m_presenceBitfield[0x13];	// 08 - if a bit is set, then the extralist should contain that extradata
	UInt8				jipRefFlags5F;				// 1B	0x5F in TESObjectREFR
	union											// 1C	JIP only!
	{
		NPCPerksInfo	*perksInfo;
		TESAmmo			*ammo;		//	Stored for projectile refs
	};

	bool HasType(UInt32 type) const;
	void __fastcall SetTypePresent(UInt32 type, bool present);
	BSExtraData *GetByType(UInt32 xType) const;
	__forceinline BSExtraData *AddExtra(BSExtraData *toAdd)
	{
		return ThisCall<BSExtraData*>(ADDR_AddExtraData, this, toAdd);
	}
	__forceinline void RemoveExtra(BSExtraData *toRemove, bool doFree)
	{
		ThisCall(0x410020, this, toRemove, doFree);
	}
	__forceinline void RemoveByType(UInt8 xType)
	{
		ThisCall(ADDR_RemoveExtraType, this, xType);
	}
	__forceinline void RemoveAll(bool doFree)
	{
		ThisCall(0x40FAE0, this, doFree);
	}
	__forceinline void CopyFrom(const BaseExtraList *sourceList, bool bCopyAndRemove)
	{
		ThisCall(0x412490, this, sourceList, bCopyAndRemove);
	}
	void DebugDump() const;
	char __fastcall GetExtraFactionRank(TESFaction *faction) const;
	SInt32 GetCount() const;
};

class ExtraCount;
class ExtraJIP;

struct ExtraDataList : public BaseExtraList
{
	ExtraDataList *CreateCopy(bool bCopyAndRemove = false);
	static ExtraDataList *Create();

	ExtraCount *AddExtraCount(SInt32 count);
	ExtraJIP *AddExtraJIP(UINT _key = 0);
};
static_assert(sizeof(ExtraDataList) == 0x20);