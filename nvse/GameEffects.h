#pragma once

// 48
class ActiveEffect
{
public:
	/*000*/virtual void		Destroy(bool doFree);
	/*004*/virtual ActiveEffect	*Clone();
	/*008*/virtual void		Unk_02();
	/*00C*/virtual void		Unk_03(float arg);
	/*010*/virtual void		SaveGame(UInt32 arg);
	/*014*/virtual void		LoadGame(UInt32 arg);
	/*018*/virtual void		Unk_06(UInt32 arg);
	/*01C*/virtual void		Unk_07(UInt32 arg);
	/*020*/virtual void		Unk_08(UInt32 arg);
	/*024*/virtual void		Unk_09(UInt32 arg);
	/*028*/virtual void		Unk_0A(UInt32 arg);
	/*02C*/virtual void		CopyTo(ActiveEffect *_target);
	/*030*/virtual bool		Unk_0C(UInt32 arg);
	/*034*/virtual bool		Unk_0D(UInt32 arg);
	/*038*/virtual bool		ResetCasterIfSame(MagicCaster *_caster);
	/*03C*/virtual bool		Unk_0F();
	/*040*/virtual void		Unk_10(UInt32 arg);
	/*044*/virtual void		CopyFrom(ActiveEffect *from);
	/*048*/virtual bool		Unk_12(UInt32 arg);
	/*04C*/virtual bool		Unk_13(MagicTarget *_target);
	/*050*/virtual void		Unk_14();
	/*054*/virtual void		Unk_15();
	/*058*/virtual void		Unk_16();

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
	TESForm			*enchantObject;		// 3C	Object responsible for effect
	tList<void>		*list40;			// 40
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
	virtual void	Unk_19(Actor *actor, float modifier, UInt32 avCode);

	UInt32		actorVal;
};

// 50
class ScriptEffect : public ActiveEffect
{
public:
	Script			*script;	// 48
	ScriptLocals	*eventList;	// 4C
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
	NiPointLight	*ptLight;	// 48
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

// 18
class BSTempEffect : public NiObject
{
public:
	virtual void	Unk_23(void);
	virtual void	Unk_24(void);
	virtual void	Unk_25(void);
	virtual void	Unk_26(void);
	virtual void	Unk_27(void);
	virtual void	Unk_28(void);
	virtual void	Unk_29(void);
	virtual void	Unk_2A(void);
	virtual void	Unk_2B(void);
	virtual void	Unk_2C(void);
	virtual void	Unk_2D(void);
	virtual void	Unk_2E(void);
	virtual void	Unk_2F(void);
	virtual void	Unk_30(void);

	float			lifetime;	// 08
	TESObjectCELL	*cell;		// 0C
	float			age;		// 10
	bool			initialized;// 14
	UInt8			pad15[3];	// 15
};

// 28
class MagicHitEffect : public BSTempEffect
{
public:
	virtual void	Unk_31(void);
	virtual void	Unk_32(void);
	virtual void	Unk_33(void);
	virtual void	Unk_34(void);
	virtual void	Unk_35(void);
	virtual void	Unk_36(void);
	virtual void	Unk_37(void);
	virtual void	Unk_38(void);

	ActiveEffect	*activeEffect;	// 18
	TESObjectREFR	*target;		// 1C
	float			unk20;			// 20
	UInt8			flags;			// 24	1 - Stop
	UInt8			pad25[3];		// 25
};

// 6C
class MagicShaderHitEffect : public MagicHitEffect
{
public:
	UInt32									unk28[2];		// 28
	TESEffectShader							*effectShader;	// 30
	float									flt34;			// 34
	BSSimpleArray<ParticleShaderProperty*>	shaderProps;	// 38
	NiNode									*shaderNode;	// 48
	UInt32									unk4C;			// 4C
	BSSimpleArray<NiAVObject*>				objects;		// 50	Seen BSFadeNode
	float									flt60;			// 60
	float									flt64;			// 64
	NiProperty								*prop68;		// 68	Seen 0x10AE0C8
};
static_assert(sizeof(MagicShaderHitEffect) == 0x6C);