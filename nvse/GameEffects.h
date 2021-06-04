#pragma once

// 48
class ActiveEffect
{
public:
	virtual void	Destroy(bool doFree);
	virtual ActiveEffect	*Clone(void);
	virtual void	Unk_02(UInt32 arg);
	virtual void	Unk_03(UInt32 arg);
	virtual void	SaveGame(UInt32 arg);
	virtual void	LoadGame(UInt32 arg);
	virtual void	Unk_06(UInt32 arg);
	virtual void	Unk_07(UInt32 arg);
	virtual void	Unk_08(UInt32 arg);
	virtual bool	UnregisterCaster(MagicCaster *_caster);	// returns 1 and clears caster if it matches the parameter, else returns 0
	virtual bool	Unk_0A(void);
	virtual void	CopyTo(ActiveEffect *_target);
	virtual bool	Unk_0C(UInt32 arg);
	virtual bool	Unk_0D(UInt32 arg);
	virtual void	Unk_0E(UInt32 arg);		// update/add effect?
	virtual void	Terminate(void);	// update/add effect?
	virtual void	Unk_10(UInt32 arg);
	virtual void	Unk_11(UInt32 arg);
	virtual void	Unk_12(UInt32 arg);
	virtual void	Unk_13(UInt32 arg);
	virtual void	Unk_14(void);
	virtual void	Unk_15(void);
	virtual void	Unk_16(void);

	float			timeElapsed;		// 04
	MagicItem		*magicItem;			// 08
	EffectItem		*effectItem;		// 0C
	bool			bActive;			// 10
	bool			bApplied;			// 11
	bool			byte12;				// 12
	bool			bTerminated;		// 13 set to 1 when effect is to be removed
	UInt8			byte14;				// 14
	UInt8			pad15[3];			// 15
	UInt32			flags;				// 18
	float			magnitude;			// 1C - adjusted based on target?
	float			duration;			// 20 - adjusted based on target?
	MagicTarget		*target;			// 24
	MagicCaster		*caster;			// 28
	UInt32			spellType;			// 2C e.g. SpellItem::kType_Ability
	Sound			sound;				// 30
	MagicItemForm	*enchantObject;		// 3C enchanted obj responsible for effect
	TESForm			*data;				// 40 - in ScriptEffect this is a Script *
	UInt32			unk44;				// 44

	__forceinline void Remove(bool immediate)
	{
		ThisCall(0x804210, this, immediate);
	}
};

// 4C
class ValueModifierEffect : public ActiveEffect
{
public:
	virtual void	SetActorValueID(UInt32 avID);
	virtual UInt32	GetActorValueID();
	virtual void	Unk_19(void);

	UInt32		actorVal;
};

// 50
class ScriptEffect : public ActiveEffect
{
public:
	UInt32		unk48[2];	// 48
};

// 48
class DispelEffect : public ActiveEffect
{
public:
	virtual void	Unk_17(void);
};

// 50
class CureEffect : public ActiveEffect
{
public:
	virtual void	Unk_17(void);

	UInt32		unk48[2];	// 48
};

// 5C
class AbsorbEffect : public ValueModifierEffect
{
public:
	virtual void	Unk_1A(void);

	UInt32		unk4C[4];	// 4C
};

// 50
class ShieldEffect : public ValueModifierEffect
{
public:
	virtual void	Unk_1A(void);

	UInt32		unk4C;	// 4C
};

// 4C
class CalmEffect : public ValueModifierEffect
{
public:
	virtual void	Unk_1A(void);
};

// 4C
class DemoralizeEffect : public ActiveEffect
{
public:
	virtual void	Unk_17(void);

	UInt32		unk48;	// 48
};

// 50
class FrenzyEffect : public ValueModifierEffect
{
public:
	virtual void	Unk_1A(void);

	UInt32		unk4C;	// 4C
};

// 48
class CommandEffect : public ActiveEffect
{
public:
	virtual void	Unk_17(void);
};

// 48
class CommandCreatureEffect : public CommandEffect
{
public:
};

// 48
class CommandHumanoidEffect : public CommandEffect
{
public:
};

// 4C
class InvisibilityEffect : public ValueModifierEffect
{
public:
	virtual void	Unk_1A(void);
};

// 4C
class ChameleonEffect : public ValueModifierEffect
{
public:
	virtual void	Unk_1A(void);
};

// 4C
class LightEffect : public ActiveEffect
{
public:
	UInt32		unk48;	// 48
};

// 4C
class DarknessEffect : public ValueModifierEffect
{
public:
	virtual void	Unk_1A(void);
};

// 4C
class NightEyeEffect : public ValueModifierEffect
{
public:
	virtual void	Unk_1A(void);
};

// 48
class LockEffect : public ActiveEffect
{
public:
	virtual void	Unk_17(void);
};

// 48
class OpenEffect : public ActiveEffect
{
public:
	virtual void	Unk_17(void);
};

// 4C
class AssociatedItemEffect : public ActiveEffect
{
public:
	virtual void	Unk_17(void);

	TESObject	*item;	// 48 - creature, armor, weapon
};

// AC
class BoundItemEffect : public AssociatedItemEffect
{
public:
	UInt32		unk48[24];	// 4C
};

// 74
class SummonCreatureEffect : public AssociatedItemEffect
{
public:
	UInt32		unk48[10];	// 4C
};

// 4C
class DetectLifeEffect : public ValueModifierEffect
{
public:
	virtual void	Unk_1A(void);
};

// 60
class TelekinesisEffect : public ValueModifierEffect
{
public:
	virtual void	Unk_1A(void);

	UInt32		unk4C[5];	// 4C
};

// 4C
class DisintegrateArmorEffect : public ActiveEffect
{
public:
	UInt32		unk48;	// 48
};

// 48
class DisintegrateWeaponEffect : public ActiveEffect
{
public:
};

// 4C
class ParalysisEffect : public ValueModifierEffect
{
public:
	virtual void	Unk_1A(void);
};

// 70
class ReanimateEffect : public ActiveEffect
{
public:
	UInt32		unk48[10];	// 48
};

// 4C
class TurnUndeadEffect : public ActiveEffect
{
public:
	virtual void	Unk_17(void);

	UInt32		unk48;	// 48
};

// 50
class SunDamageEffect : public ActiveEffect
{
public:
	virtual void	Unk_17(void);

	UInt32		unk48[2];	// 48
};

// 48
class VampirismEffect : public ActiveEffect
{
public:
	virtual void	Unk_17(void);
};

// 4C
class ConcussionEffect : public ActiveEffect
{
public:
	float	unk48;		//  48
};

// 50
class ValueAndConditionsEffect : public ValueModifierEffect
{
public:
	virtual void	Unk_1A(void);

	UInt32		unk4C;	// 4C
};

// 50
class LimbConditionEffect : public ValueModifierEffect
{
public:
	virtual void	Unk_1A(void);

	UInt32		unk4C;	// 4C
};