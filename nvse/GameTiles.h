#pragma once

//	Tile			
//		TileRect		3C
//			TileMenu	40
//		TileImage		48
//		TileText		4C
//		Tile3D			50

enum TileTypeID
{
	kTileType_rect =		0x385,
	kTileType_image =		0x386,
	kTileType_text =		0x387,
	kTileType_nif =			0x388,
	kTileType_3D =			kTileType_nif,
	kTileType_menu =		0x389,
	kTileType_hotrect =		0x38A,
	kTileType_window =		0x38B,
	kTileType_radial =		0x38C,
	kTileType_template =	0x3E7
};

enum TileValueID
{
	kTileValue_x =					0xFA1,
	kTileValue_y =					0xFA2,
	kTileValue_visible =			0xFA3,
	kTileValue_class =				0xFA4,
	kTileValue_clipwindow =			0xFA6,
	kTileValue_stackingtype =		0xFA7,
	kTileValue_locus =				0xFA8,
	kTileValue_alpha =				0xFA9,
	kTileValue_id =					0xFAA,
	kTileValue_disablefade =		0xFAB,
	kTileValue_listindex =			0xFAC,
	kTileValue_depth =				0xFAD,
	kTileValue_clips =				0xFAE,
	kTileValue_target =				0xFAF,
	kTileValue_height =				0xFB0,
	kTileValue_width =				0xFB1,
	kTileValue_red =				0xFB2,
	kTileValue_green =				0xFB3,
	kTileValue_blue =				0xFB4,
	kTileValue_tile =				0xFB5,
	kTileValue_childcount =			0xFB6,
	kTileValue_child_count =		kTileValue_childcount,
	kTileValue_justify =			0xFB7,
	kTileValue_zoom =				0xFB8,
	kTileValue_font =				0xFB9,
	kTileValue_wrapwidth =			0xFBA,
	kTileValue_wraplimit =			0xFBB,
	kTileValue_wraplines =			0xFBC,
	kTileValue_pagenum =			0xFBD,
	kTileValue_ishtml =				0xFBE,
	kTileValue_cropoffsety =		0xFBF,
	kTileValue_cropy =				kTileValue_cropoffsety,
	kTileValue_cropoffsetx =		0xFC0,
	kTileValue_cropx =				kTileValue_cropoffsetx,
	kTileValue_menufade =			0xFC1,
	kTileValue_explorefade =		0xFC2,
	kTileValue_mouseover =			0xFC3,
	kTileValue_string =				0xFC4,
	kTileValue_shiftclicked =		0xFC5,
	kTileValue_clicked =			0xFC7,
	kTileValue_clicksound =			0xFCB,
	kTileValue_filename =			0xFCC,
	kTileValue_filewidth =			0xFCD,
	kTileValue_fileheight =			0xFCE,
	kTileValue_repeatvertical =		0xFCF,
	kTileValue_repeathorizontal =	0xFD0,
	kTileValue_animation =			0xFD2,
	kTileValue_linecount =			0xFD4,
	kTileValue_pagecount =			0xFD5,
	kTileValue_xdefault =			0xFD6,
	kTileValue_xup =				0xFD7,
	kTileValue_xdown =				0xFD8,
	kTileValue_xleft =				0xFD9,
	kTileValue_xright =				0xFDA,
	kTileValue_xbuttona =			0xFDD,
	kTileValue_xbuttonb =			0xFDE,
	kTileValue_xbuttonx =			0xFDF,
	kTileValue_xbuttony =			0xFE0,
	kTileValue_xbuttonlt =			0xFE1,
	kTileValue_xbuttonrt =			0xFE2,
	kTileValue_xbuttonlb =			0xFE3,
	kTileValue_xbuttonrb =			0xFE4,
	kTileValue_xbuttonstart =		0xFE7,
	kTileValue_mouseoversound =		0xFE8,
	kTileValue_draggable =			0xFE9,
	kTileValue_dragstartx =			0xFEA,
	kTileValue_dragstarty =			0xFEB,
	kTileValue_dragoffsetx =		0xFEC,
	kTileValue_dragoffsety =		0xFED,
	kTileValue_dragdeltax =			0xFEE,
	kTileValue_dragdeltay =			0xFEF,
	kTileValue_dragx =				0xFF0,
	kTileValue_dragy =				0xFF1,
	kTileValue_wheelable =			0xFF2,
	kTileValue_wheelmoved =			0xFF3,
	kTileValue_systemcolor =		0xFF4,
	kTileValue_brightness =			0xFF5,
	kTileValue_linegap =			0xFF7,
	kTileValue_resolutionconverter =0xFF8,
	kTileValue_texatlas =			0xFF9,
	kTileValue_rotateangle =		0xFFA,
	kTileValue_rotateaxisx =		0xFFB,
	kTileValue_rotateaxisy =		0xFFC,

	kTileValue_user0 =				0x1004,
	kTileValue_user1 =				0x1005,
	kTileValue_user2 =				0x1006,
	kTileValue_user3 =				0x1007,
	kTileValue_user4 =				0x1008,
	kTileValue_user5 =				0x1009,
	kTileValue_user6 =				0x100A,
	kTileValue_user7 =				0x100B,
	kTileValue_user8 =				0x100C,
	kTileValue_user9 =				0x100D,
	kTileValue_user10 =				0x100E,
	kTileValue_user11 =				0x100F,
	kTileValue_user12 =				0x1010,
	kTileValue_user13 =				0x1011,
	kTileValue_user14 =				0x1012,
	kTileValue_user15 =				0x1013,
	kTileValue_user16 =				0x1014,

	kTileValue_max =				0x1015
};

// 38+
class Tile
{
public:
	/*000*/virtual Tile		*Destroy(bool doFree);
	/*004*/virtual void		Init(Tile *parent, const char *name, Tile *replacedChild);
	/*008*/virtual NiNode	*CalcNode(void);
	/*00C*/virtual UInt32	GetType(void);		// returns one of kTileValue_XXX
	/*010*/virtual const char	*GetTypeStr(void);	// 4-byte id
	/*014*/virtual bool		Unk_05(float arg0, float arg1);	// RadialTile only
	/*018*/virtual Tile		*UpdateField(UInt32 valueID, float floatValue, const char *strValue);
	/*01C*/virtual void		ClearTextures();
	/*020*/virtual TileShaderProperty	*GetShaderProperty();
	/*024*/virtual void		SetAlphaAndOverlayColor(NiNode *niNode, float alpha, NiColorAlpha *color);

	struct Value;

	enum ActionType
	{
		kAction_copy = 0x7D0,
		kAction_add,
		kAction_sub,
		kAction_mul,
		kAction_div,
		kAction_min,
		kAction_max,
		kAction_mod,
		kAction_floor,
		kAction_ceil,
		kAction_abs,
		kAction_round,
		kAction_gt,
		kAction_gte,
		kAction_eq,
		kAction_neq,
		kAction_lt,
		kAction_lte,
		kAction_and,
		kAction_or,
		kAction_not,
		kAction_onlyif,
		kAction_onlyifnot,
		kAction_ref,
		kAction_begin,
		kAction_end,
		//	UIO
		kAction_neg,
		kAction_recipr,
		kAction_land,
		kAction_lor,
		kAction_shl,
		kAction_shr,
		kAction_bt,
		kAction_sqrt,
		kAction_pow,
		kAction_sin,
		kAction_cos,
		kAction_tan,
		kAction_asin,
		kAction_acos,
		kAction_atan,
		kAction_log,
		kAction_rand
	};

	// 0C
	class Action
	{
	public:
		virtual float	GetFloat();
		virtual Value	*GetValue();

		UInt32		type;		// 04
		Action		*next;		// 08
	};

	// 10
	class RefValueAction : public Action
	{
	public:
		Value		*tileVal;	// 0C
	};

	// 10
	class FloatAction : public Action
	{
	public:
		float		value;		// 0C
	};

	// 14
	struct Value
	{
		UInt32		id;			// 00
		Tile		*parent;	// 04
		float		num;		// 08
		char		*str;		// 0C
		Action		*action;	// 10

		/*__forceinline void SetFloat(float fltVal, bool bPropagate = 0)
		{
			ThisCall(ADDR_TileValSetFloat, this, fltVal, bPropagate);
		}
		__forceinline void SetString(const char *strVal, bool bPropagate = 0)
		{
			ThisCall(0xA0A300, this, strVal, bPropagate);
		}*/

		void __vectorcall SetFloat(float value);
		void __fastcall SetBool(bool value);
		void SetString(const char *strVal);

		__forceinline void Refresh(bool forceRefreshReactions = false)
		{
			ThisCall(0xA09410, this, forceRefreshReactions);
		}
		__forceinline void AddRefValueAction(UInt32 _action, Tile *ref, UInt32 value)
		{
			ThisCall(0xA09130, this, _action, ref, value);
		}
		__forceinline void AddFloatAction(UInt32 _action, float value)
		{
			ThisCall(0xA09080, this, _action, value);
		}
		__forceinline void RemoveFromReactionMap()
		{
			ThisCall(0xA09200, this);
		}
	};

	enum TileFlags
	{
		kTileFlag_PositionOrRotation =	1,
		kTileFlag_Text =				2,
		kTileFlag_Visibility =			4,
		kTileFlag_ColorOrBrightness =	8,
		kTileFlag_Dimensions =			0x10,
		kTileFlag_ImageFilename =		0x20,
		kTileFlag_NifFilename =			0x40,
		kTileFlag_ClipWindow =			0x80,
		kTileFlag_Clips =				0x100,
		kTileFlag_Locus =				0x200,
		kTileFlag_All =					0x3FF
	};

	DList<Tile>					children;		// 04
	BSSimpleArray<Value*>		values;			// 10
	String						name;			// 20
	Tile						*parent;		// 28
	NiNode						*node;			// 2C
	UInt32						flags;			// 30
	UInt8						unk34;			// 34
	UInt8						unk35;			// 35
	UInt8						pad35[2];		// 36

	static UInt32 __fastcall TraitNameToID(const char *traitName);
	__forceinline static UInt32 TraitNameToIDAdd(const char *traitName)
	{
		return CdeclCall<UInt32>(0xA00940, traitName, 0xFFFFFFFF);
	}
	Value* __fastcall GetValue(UInt32 typeID) const;
	__forceinline Value *AddValue(UInt32 typeID)
	{
		return ThisCall<Value*>(0xA01000, this, typeID);
	}
	Value* __fastcall GetValueName(const char *valueName) const;
	float __vectorcall GetValueFloat(UInt32 id) const;
	DList<Tile>::Node* __fastcall GetNthChild(UInt32 index) const;
	Tile *GetChild(const char *childName);
	Tile *GetComponent(const char *componentTile, const char **trait);
	Tile *GetComponentTile(const char *componentTile);
	Value *GetComponentValue(const char *componentPath);
	__forceinline Tile *ReadXML(const char *xmlPath)
	{
		return ThisCall<Tile*>(0xA01B00, this, xmlPath);
	}
	void GetComponentFullName(char *resStr);
	void __vectorcall SetFloat(UInt32 id, float fltVal);
	void __fastcall SetBool(UInt32 id, bool value);
	void __fastcall SetString(UInt32 id, const char *strVal);
	__forceinline void StartGradualSetFloat(UInt32 id, float startVal, float endVal, float seconds, UInt32 changeMode = 0)
	{
		ThisCall(0xA07C60, this, id, startVal, endVal, seconds, changeMode);
	}
	__forceinline void EndGradualSetFloat(UInt32 id)
	{
		ThisCall(0xA07DC0, this, id);
	}
	__forceinline bool HasGradualSetFloat(UInt32 id)
	{
		return !ThisCall<bool>(0xA07FC0, this, id);
	}
	Menu *GetParentMenu() const;
	void DestroyAllChildren();
	void __fastcall PokeValue(UInt32 valueID);
	__forceinline void SetChangeFlag(UInt32 flagMask) {ThisCall(0xA07530, this, flagMask);}
	__forceinline float GetLocusAdjustedPosX() {return ThisCall<float>(0xA013D0, this);}
	__forceinline float GetLocusAdjustedPosY() {return ThisCall<float>(0xA01440, this);}
	__forceinline void ApplyRotationOnChildren() {ThisCall(0xA0B520, this);}
	__forceinline void PlaySound(UInt32 id) {ThisCall(0xA0B110, this, id);}

	void Dump();
};
typedef Tile::Value TileValue;

Tile* __fastcall GetTargetComponent(const char *componentPath, TileValue **value = NULL);

const char *TraitIDToName(UInt32 id);

// 1C
struct GradualSetFloat
{
	float		startValue;		// 00
	float		endValue;		// 04
	UInt32		startTimeMS;	// 08
	float		durationMS;		// 0C
	UInt32		valueID;		// 10
	Tile		*tile;			// 14
	UInt32		changeMode;		// 18	0-4, see 0xA081B5
	//	0		From start to end in duration
	//	1		From start to end to start, in duration, perpetually
	//	2		From start to end to start, in duration, 4 times
	//	3		From start to end to start, 7 times, in duration
	//	4		From start to end in duration/6, end for duration*2/3, from end to start in duration/6
	//	5**		From start to end, in duration, perpetually (suitable for image rotation)
};

// 3C
class TileRect : public Tile
{
public:
	UInt32	unk38;	// 38
};

// 40
class TileMenu : public TileRect
{
public:
	Menu	*menu;	// 3C
};

// 48
class TileImage : public Tile
{
public:
	float				flt38;			// 38
	NiTexture			*texture;		// 3C
	TileShaderProperty	*shaderProp;	// 40
	UInt8				byte44;			// 44
	UInt8				pad45[3];		// 45

	__forceinline void SetTexture(NiTexture *tex) {ThisCall(0xA20610, this, tex);}
	void __fastcall SetAlphaTexture(const char *ddsPath);
};

// 48
class RadialTile : public TileImage
{
public:
};

// 4C
class TileText : public Tile
{
public:
	UInt32			unk38;		// 38
	NiVector3		pos3C;		// 3C
	UInt8			byte48;		// 48
	UInt8			pad49[3];	// 49
};

// 50
class Tile3D : public Tile
{
public:
	NiControllerManager		*ctrlManager;	// 38
	BSAnimGroupSequence		*animSequence;	// 3C
	String					animStr;		// 40
	String					fileName;		// 48
};