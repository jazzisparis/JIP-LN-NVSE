#pragma once

constexpr ParamInfo kParams_OneInt[] =
{
	{kParamType_Integer}
};

constexpr ParamInfo kParams_TwoInts[] =
{
	{kParamType_Integer},
	{kParamType_Integer}
};

constexpr ParamInfo kParams_OneOptionalInt[] =
{
	{kParamType_Integer, 1}
};

constexpr ParamInfo kParams_OneInt_OneOptionalInt[] =
{
	{kParamType_Integer},
	{kParamType_Integer, 1}
};

constexpr ParamInfo kParams_OneFloat[] =
{
	{kParamType_Float}
};

constexpr ParamInfo kParams_OneString[] =
{
	{kParamType_String}
};

constexpr ParamInfo kParams_OneString_OneFloat[] =
{
	{kParamType_String},
	{kParamType_Float}
};

constexpr ParamInfo kParams_TwoFloats[] =
{
	{kParamType_Float},
	{kParamType_Float}
};

constexpr ParamInfo kParams_OneObjectID[] =
{
	{kParamType_ObjectID}
};

constexpr ParamInfo kParams_OneOptionalObjectID[] =
{
	{kParamType_ObjectID, 1}
};

constexpr ParamInfo kParams_OneObjectID_OneInt[] =
{
	{kParamType_ObjectID},
	{kParamType_Integer}
};

#define FORMAT_STRING_PARAMS 	\
	{kParamType_String}, \
	{kParamType_Float, 1}, \
	{kParamType_Float, 1}, \
	{kParamType_Float, 1}, \
	{kParamType_Float, 1}, \
	{kParamType_Float, 1}, \
	{kParamType_Float, 1}, \
	{kParamType_Float, 1}, \
	{kParamType_Float, 1}, \
	{kParamType_Float, 1}, \
	{kParamType_Float, 1}, \
	{kParamType_Float, 1}, \
	{kParamType_Float, 1}, \
	{kParamType_Float, 1}, \
	{kParamType_Float, 1}, \
	{kParamType_Float, 1}, \
	{kParamType_Float, 1}, \
	{kParamType_Float, 1}, \
	{kParamType_Float, 1}, \
	{kParamType_Float, 1}, \
	{kParamType_Float, 1} 

constexpr ParamInfo kParams_FormatString[] =
{
	FORMAT_STRING_PARAMS
};

constexpr ParamInfo kParams_OneActor[] =
{
	{kParamType_Actor}
};

constexpr ParamInfo kParams_OneAxis[] =
{
	{kParamType_Axis}
};

constexpr ParamInfo kParams_FormList[] =
{
	{kParamType_FormList}
};

constexpr ParamInfo kParams_TwoStrings[] =
{
	{kParamType_String},
	{kParamType_String}
};

constexpr ParamInfo kParams_OneBoundObject[] =
{
	{kParamType_BoundObject}
};

constexpr ParamInfo kParams_OneOptionalBoundObject[] =
{
	{kParamType_BoundObject, 1}
};

constexpr ParamInfo kParams_OneBoundObject_OneOptionalBoundObject[] =
{
	{kParamType_BoundObject},
	{kParamType_BoundObject, 1}
};

constexpr ParamInfo kParams_OneBoundObject_OneInt[] =
{
	{kParamType_BoundObject},
	{kParamType_Integer}
};
 
constexpr ParamInfo kParams_OneInt_OneOptionalBoundObject[] =
{
	{kParamType_Integer},
	{kParamType_BoundObject, 1}
};

constexpr ParamInfo kParams_OneQuest[] =
{
	{kParamType_Quest}
};

constexpr ParamInfo kParams_OneOptionalObjectRef[] =
{
	{kParamType_ObjectRef, 1}
};

constexpr ParamInfo kParams_OneForm_OneInt[] =
{
	{kParamType_AnyForm},
	{kParamType_Integer}
};

constexpr ParamInfo kParams_OneForm[] =
{
	{kParamType_AnyForm}
};

constexpr ParamInfo kParams_OneForm_OneFloat[] =
{
	{kParamType_AnyForm},
	{kParamType_Float}
};

constexpr ParamInfo kParams_OneOptionalForm[] =
{
	{kParamType_AnyForm, 1}
};

constexpr ParamInfo kParams_OneObjectID_TwoOptionalInts[] =
{
	{kParamType_ObjectID},
	{kParamType_Integer, 1},
	{kParamType_Integer, 1}
};

constexpr ParamInfo kParams_OneOptionalActorBase[] =
{
	{kParamType_ActorBase, 1}
};

constexpr ParamInfo kParams_OneInt_OneOptionalActorBase[] =
{
	{kParamType_Integer},
	{kParamType_ActorBase, 1}
};

constexpr ParamInfo kParams_OneItemOrList_OneOptInt_OneOptFloat_TwoOptInts[] =
{
	{kParamType_InvObjOrFormList},
	{kParamType_Integer, 1},
	{kParamType_Float, 1},
	{kParamType_Integer, 1},
	{kParamType_Integer, 1}
};

constexpr ParamInfo kParams_OneString_OneFormatString[] =
{
	{kParamType_String},
	FORMAT_STRING_PARAMS
};

constexpr ParamInfo kParams_OneForm_OneInt_OneFloat[] =
{
	{kParamType_AnyForm},
	{kParamType_Integer},
	{kParamType_Float}
};

constexpr ParamInfo kParams_OneForm_TwoInts[] =
{
	{kParamType_AnyForm},
	{kParamType_Integer},
	{kParamType_Integer}
};

constexpr ParamInfo kParams_TwoForms[] =
{
	{kParamType_AnyForm},
	{kParamType_AnyForm}
};

constexpr ParamInfo kParams_OneForm_OneInt_OneOptionalForm[] =
{
	{kParamType_AnyForm},
	{kParamType_Integer},
	{kParamType_AnyForm, 1}
};

constexpr ParamInfo kParams_OneString_OneOptionalQuest[] =
{
	{kParamType_String},
	{kParamType_Quest, 1}
};

constexpr ParamInfo kParams_OneQuest_OneFloat[] =
{
	{kParamType_Quest},
	{kParamType_Float}
};

constexpr ParamInfo kParams_OneAIPackage_OneInt[] =
{
	{kParamType_AIPackage},
	{kParamType_Integer}
};

constexpr ParamInfo kParams_OneAIPackage_TwoInts[] =
{
	{kParamType_AIPackage},
	{kParamType_Integer},
	{kParamType_Integer}
};

constexpr ParamInfo kParams_OneGlobal[] =
{
	{kParamType_Global}
};

constexpr ParamInfo kParams_OneGlobal_OneFloat[] =
{
	{kParamType_Global},
	{kParamType_Float}
};

constexpr ParamInfo kParams_OneFaction[] =
{
	{kParamType_Faction}
};

constexpr ParamInfo kParams_OneFaction_OneOptionalReputation[] =
{
	{kParamType_Faction},
	{kParamType_Reputation, 1}
};

constexpr ParamInfo kParams_OneAxis_OneOptionalBoundObject[] =
{
	{kParamType_Axis},
	{kParamType_BoundObject, 1}
};

constexpr ParamInfo kParams_TwoDoubles_ThreeOptionalDoubles[] =
{
	{kParamType_Double},
	{kParamType_Double},
	{kParamType_Double, 1},
	{kParamType_Double, 1},
	{kParamType_Double, 1}
};

constexpr ParamInfo kParams_OneCombatStyle_OneInt[] =
{
	{kParamType_CombatStyle},
	{kParamType_Integer}
};

constexpr ParamInfo kParams_OneCombatStyle_OneInt_OneFloat[] =
{
	{kParamType_CombatStyle},
	{kParamType_Integer},
	{kParamType_Float}
};

constexpr ParamInfo kParams_OneCombatStyle_TwoInts[] =
{
	{kParamType_CombatStyle},
	{kParamType_Integer},
	{kParamType_Integer}
};

constexpr ParamInfo kParams_OneString_TwoOptionalInts[] =
{
	{kParamType_String},
	{kParamType_Integer, 1},
	{kParamType_Integer, 1}
};

constexpr ParamInfo kParams_OneString_TwoInts[] =
{
	{kParamType_String},
	{kParamType_Integer},
	{kParamType_Integer}
};

constexpr ParamInfo kParams_OneString_OneInt_OneFormatString[] =
{
	{kParamType_String},
	{kParamType_Integer},
	FORMAT_STRING_PARAMS
};

constexpr ParamInfo kParams_OneClass_OneInt[] =
{
	{kParamType_Class},
	{kParamType_Integer}
};

constexpr ParamInfo kParams_OneClass_TwoInts[] =
{
	{kParamType_Class},
	{kParamType_Integer},
	{kParamType_Integer}
};

constexpr ParamInfo kParams_OneSound_OneInt[] =
{
	{kParamType_Sound},
	{kParamType_Integer}
};

constexpr ParamInfo kParams_OneSound_OneInt_OneFloat[] =
{
	{kParamType_Sound},
	{kParamType_Integer},
	{kParamType_Float}
};

constexpr ParamInfo kParams_OneSound_TwoInts[] =
{
	{kParamType_Sound},
	{kParamType_Integer},
	{kParamType_Integer}
};

constexpr ParamInfo kParams_OneForm_OneInt_OneOptionalSound[] =
{
	{kParamType_AnyForm},
	{kParamType_Integer},
	{kParamType_Sound, 1}
};

constexpr ParamInfo kParams_OneForm_OneInt_OneOptionalObjectID[] =
{
	{kParamType_AnyForm},
	{kParamType_Integer},
	{kParamType_ObjectID, 1}
};

constexpr ParamInfo kParams_OneWeatherID_OneInt[] =
{
	{kParamType_WeatherID},
	{kParamType_Integer}
};

constexpr ParamInfo kParams_OneWeatherID_OneInt_OneOptionalImageSpaceMod[] =
{
	{kParamType_WeatherID},
	{kParamType_Integer},
	{kParamType_ImageSpaceModifier, 1}
};

constexpr ParamInfo kParams_OneWeatherID_OneInt_OneString[] =
{
	{kParamType_WeatherID},
	{kParamType_Integer},
	{kParamType_String}
};

constexpr ParamInfo kParams_OneWeatherID[] =
{
	{kParamType_WeatherID}
};

constexpr ParamInfo kParams_OneWeatherID_OneString[] =
{
	{kParamType_WeatherID},
	{kParamType_String}
};

constexpr ParamInfo kParams_OneWeatherID_OneInt_OneDouble[] =
{
	{kParamType_WeatherID},
	{kParamType_Integer},
	{kParamType_Double}
};

constexpr ParamInfo kParams_OneWeatherID_TwoInts_OneOptionalInt[] =
{
	{kParamType_WeatherID},
	{kParamType_Integer},
	{kParamType_Integer},
	{kParamType_Integer, 1}
};

constexpr ParamInfo kParams_OneWeatherID_ThreeInts_OneOptionalInt[] =
{
	{kParamType_WeatherID},
	{kParamType_Integer},
	{kParamType_Integer},
	{kParamType_Integer},
	{kParamType_Integer, 1}
};

constexpr ParamInfo kParams_OneForm_OneInt_OneString[] =
{
	{kParamType_AnyForm},
	{kParamType_Integer},
	{kParamType_String}
};

constexpr ParamInfo kParams_OneForm_OneString[] =
{
	{kParamType_AnyForm},
	{kParamType_String}
};

constexpr ParamInfo kParams_OneForm_OneWeatherID_OneInt_OneOptionalGlobal[] =
{
	{kParamType_AnyForm},
	{kParamType_WeatherID},
	{kParamType_Integer},
	{kParamType_Global, 1}
};

constexpr ParamInfo kParams_OneForm_OneWeatherID[] =
{
	{kParamType_AnyForm},
	{kParamType_WeatherID}
};

constexpr ParamInfo kParams_OneAxis_OneFloat[] =
{
	{kParamType_Axis},
	{kParamType_Float}
};

constexpr ParamInfo kParams_OneForm_ThreeInts[] =
{
	{kParamType_AnyForm},
	{kParamType_Integer},
	{kParamType_Integer},
	{kParamType_Integer}
};

constexpr ParamInfo kParams_OneItemOrList_OneContainer_TwoOptionalInts[] =
{
	{kParamType_InvObjOrFormList},
	{kParamType_Container},
	{kParamType_Integer, 1},
	{kParamType_Integer, 1}
};

constexpr ParamInfo kParams_OneInt_OneFloat_OneOptionalActorBase[] =
{
	{kParamType_Integer},
	{kParamType_Float},
	{kParamType_ActorBase, 1}
};

constexpr ParamInfo kParams_OneMagicItem_OneInt[] =
{
	{kParamType_MagicItem},
	{kParamType_Integer}
};

constexpr ParamInfo kParams_OneMagicItem_TwoInts[] =
{
	{kParamType_MagicItem},
	{kParamType_Integer},
	{kParamType_Integer}
};

constexpr ParamInfo kParams_OneMagicItem_ThreeInts[] =
{
	{kParamType_MagicItem},
	{kParamType_Integer},
	{kParamType_Integer},
	{kParamType_Integer}
};

constexpr ParamInfo kParams_OneMagicItem_OneInt_OneMagicEffect[] =
{
	{kParamType_MagicItem},
	{kParamType_Integer},
	{kParamType_MagicEffect}
};

constexpr ParamInfo kParams_OneOptionalQuest[] =
{
	{kParamType_Quest, 1}
};

constexpr ParamInfo kParams_OneString_OneOptionalForm[] =
{
	{kParamType_String},
	{kParamType_AnyForm, 1}
};

constexpr ParamInfo kParams_OneString_OneOptionalInt_OneOptionalForm[] =
{
	{kParamType_String},
	{kParamType_Integer, 1},
	{kParamType_AnyForm, 1}
};

constexpr ParamInfo kParams_OneString_OneDouble_OneOptionalInt_OneOptionalForm[] =
{
	{kParamType_String},
	{kParamType_Double},
	{kParamType_Integer, 1},
	{kParamType_AnyForm, 1}
};

constexpr ParamInfo kParams_OneString_OneForm_OneOptionalInt_OneOptionalForm[] =
{
	{kParamType_String},
	{kParamType_AnyForm},
	{kParamType_Integer, 1},
	{kParamType_AnyForm, 1}
};

constexpr ParamInfo kParams_TwoStrings_OneOptionalInt_OneOptionalForm[] =
{
	{kParamType_String},
	{kParamType_String},
	{kParamType_Integer, 1},
	{kParamType_AnyForm, 1}
};

constexpr ParamInfo kParams_OneInt_OneOptionalForm[] =
{
	{kParamType_Integer},
	{kParamType_AnyForm, 1}
};

constexpr ParamInfo kParams_OneString_OneInt_OneOptionalForm[] =
{
	{kParamType_String},
	{kParamType_Integer},
	{kParamType_AnyForm, 1}
};

constexpr ParamInfo kParams_OneString_OneDouble_OneOptionalForm[] =
{
	{kParamType_String},
	{kParamType_Double},
	{kParamType_AnyForm, 1}
};

constexpr ParamInfo kParams_OneString_OneForm_OneOptionalForm[] =
{
	{kParamType_String},
	{kParamType_AnyForm},
	{kParamType_AnyForm, 1}
};

constexpr ParamInfo kParams_TwoStrings_OneOptionalForm[] =
{
	{kParamType_String},
	{kParamType_String},
	{kParamType_AnyForm, 1}
};

constexpr ParamInfo kParams_FourInts[] =
{
	{kParamType_Integer},
	{kParamType_Integer},
	{kParamType_Integer},
	{kParamType_Integer}
};

constexpr ParamInfo kParams_OneForm_TwoInts_OneFloat[] =
{
	{kParamType_AnyForm},
	{kParamType_Integer},
	{kParamType_Integer},
	{kParamType_Float}
};

constexpr ParamInfo kParams_OneForm_TwoInts_OneOptionalForm[] =
{
	{kParamType_AnyForm},
	{kParamType_Integer},
	{kParamType_Integer},
	{kParamType_AnyForm, 1}
};

constexpr ParamInfo kParams_OneForm_OneOptionalForm[] =
{
	{kParamType_AnyForm},
	{kParamType_AnyForm, 1}
};

constexpr ParamInfo kParams_OneSound[] =
{
	{kParamType_Sound}
};

constexpr ParamInfo kParams_OneSound_OneString[] =
{
	{kParamType_Sound},
	{kParamType_String}
};

constexpr ParamInfo kParams_OneWorldspace[] =
{
	{kParamType_WorldSpace}
};

constexpr ParamInfo kParams_OneWorldspace_OneOptionalForm[] =
{
	{kParamType_WorldSpace},
	{kParamType_AnyForm, 1}
};

constexpr ParamInfo kParams_OneForm_OneOptionalActorBase[] =
{
	{kParamType_AnyForm},
	{kParamType_ActorBase, 1}
};

constexpr ParamInfo kParams_OneInt_OneFloat[] =
{
	{kParamType_Integer},
	{kParamType_Float}
};

constexpr ParamInfo kParams_OneInt_OneFormatString[] =
{
	{kParamType_Integer},
	FORMAT_STRING_PARAMS
};

constexpr ParamInfo kParams_TwoInts_OneOptionalInt[] =
{
	{kParamType_Integer},
	{kParamType_Integer},
	{kParamType_Integer, 1}
};

constexpr ParamInfo kParams_OneObjectRef_OneOptionalInt[] =
{
	{kParamType_ObjectRef},
	{kParamType_Integer, 1}
};

constexpr ParamInfo kParams_OneList_TwoOptionalInts[] =
{
	{kParamType_FormList},
	{kParamType_Integer, 1},
	{kParamType_Integer, 1}
};

constexpr ParamInfo kParams_OneFaction_OneOptionalInt[] =
{
	{kParamType_Faction},
	{kParamType_Integer, 1}
};

constexpr ParamInfo kParams_OneQuest_OneInt[] =
{
	{kParamType_Quest},
	{kParamType_Integer}
};

constexpr ParamInfo kParams_OneWorldspace_OneInt[] =
{
	{kParamType_WorldSpace},
	{kParamType_Integer}
};

constexpr ParamInfo kParams_OneWorldspace_TwoInts[] =
{
	{kParamType_WorldSpace},
	{kParamType_Integer},
	{kParamType_Integer}
};

constexpr ParamInfo kParams_OneString_OneInt_OneOptionalActorBase[] =
{
	{kParamType_String},
	{kParamType_Integer},
	{kParamType_ActorBase, 1}
};

constexpr ParamInfo kParams_OneString_OneOptionalActorBase[] =
{
	{kParamType_String},
	{kParamType_ActorBase, 1}
};

constexpr ParamInfo kParams_OneObjectRef[] =
{
	{kParamType_ObjectRef}
};

constexpr ParamInfo kParams_OneInt_ThreeFloats[] =
{
	{kParamType_Integer},
	{kParamType_Float},
	{kParamType_Float},
	{kParamType_Float}
};

constexpr ParamInfo kParams_OneMagicItem_OneEffect_TwoInts_ThreeOptionalInts[] =
{
	{kParamType_MagicItem},
	{kParamType_MagicEffect},
	{kParamType_Integer},
	{kParamType_Integer},
	{kParamType_Integer, 1},
	{kParamType_Integer, 1},
	{kParamType_Integer, 1}
};

constexpr ParamInfo kParams_OneItemOrList_TwoOptionalInts[] =
{
	{kParamType_InvObjOrFormList},
	{kParamType_Integer, 1},
	{kParamType_Integer, 1}
};

constexpr ParamInfo kParams_TwoOptionalInts[] =
{
	{kParamType_Integer, 1},
	{kParamType_Integer, 1}
};

constexpr ParamInfo kParams_OneActorValue_OneFloat_OneOptionalActorBase[] =
{
	{kParamType_ActorValue},
	{kParamType_Float},
	{kParamType_ActorBase, 1}
};

constexpr ParamInfo kParams_OneFloat_OneFormatString[] =
{
	{kParamType_Float},
	FORMAT_STRING_PARAMS
};

constexpr ParamInfo kParams_OneForm_OneFormatString[] =
{
	{kParamType_AnyForm},
	FORMAT_STRING_PARAMS
};

constexpr ParamInfo kParams_OneInt_OneString[] =
{
	{kParamType_Integer},
	{kParamType_String}
};

constexpr ParamInfo kParams_OneForm_TwoFloats_OneFormatString[] =
{
	{kParamType_AnyForm},
	{kParamType_Float},
	{kParamType_Float},
	FORMAT_STRING_PARAMS
};

constexpr ParamInfo kParams_OneActor_OneTopic_TwoOptionalInts[] =
{
	{kParamType_Actor},
	{kParamType_Topic},
	{kParamType_Integer, 1},
	{kParamType_Integer, 1}
};

constexpr ParamInfo kParams_OneTopic[] =
{
	{kParamType_Topic}
};

constexpr ParamInfo kParams_OneForm_ThreeFloats[] =
{
	{kParamType_AnyForm},
	{kParamType_Float},
	{kParamType_Float},
	{kParamType_Float}
};

constexpr ParamInfo kParams_FourOptionalInts_OneOptionalList[] =
{
	{kParamType_Integer, 1},
	{kParamType_Integer, 1},
	{kParamType_Integer, 1},
	{kParamType_Integer, 1},
	{kParamType_FormList, 1}
};

constexpr ParamInfo kParams_OneInt_OneOptionalActor[] =
{
	{kParamType_Integer},
	{kParamType_Actor, 1}
};

constexpr ParamInfo kParams_OneObjectID_OneActorValue[] =
{
	{kParamType_ObjectID},
	{kParamType_ActorValue}
};

constexpr ParamInfo kParams_OneMessage[] =
{
	{kParamType_Message}
};

constexpr ParamInfo kParams_OneMessage_OneInt[2] =
{
	{kParamType_Message},
	{kParamType_Integer},
};

constexpr ParamInfo kParams_OneSound_OneOptionalObjectRef[] =
{
	{kParamType_Sound},
	{kParamType_ObjectRef, 1}
};

constexpr ParamInfo kParams_OneObjectRef_TwoStrings_OneOtionalInt[] =
{
	{kParamType_ObjectRef},
	{kParamType_String},
	{kParamType_String},
	{kParamType_Integer, 1}
};

constexpr ParamInfo kParams_OneString_OneInt[] =
{
	{kParamType_String},
	{kParamType_Integer}
};

constexpr ParamInfo kParams_OneString_OneInt_OneFloat[] =
{
	{kParamType_String},
	{kParamType_Integer},
	{kParamType_Float}
};

constexpr ParamInfo kParams_OneForm_OneInt_OneForm_OneInt[] =
{
	{kParamType_AnyForm},
	{kParamType_Integer},
	{kParamType_AnyForm},
	{kParamType_Integer}
};

constexpr ParamInfo kParams_OneForm_OneInt_OneForm_OneAnimGroup[] =
{
	{kParamType_AnyForm},
	{kParamType_Integer},
	{kParamType_AnyForm},
	{kParamType_AnimationGroup}
};

constexpr ParamInfo kParams_OneForm_OneInt_OneForm[] =
{
	{kParamType_AnyForm},
	{kParamType_Integer},
	{kParamType_AnyForm}
};

constexpr ParamInfo kParams_OneActorValue_OneOptionalActorBase[] =
{
	{kParamType_ActorValue},
	{kParamType_ActorBase, 1}
};

constexpr ParamInfo kParams_ThreeFloats[] =
{
	{kParamType_Float},
	{kParamType_Float},
	{kParamType_Float}
};

constexpr ParamInfo kParams_OneOptionalAxis[] =
{
	{kParamType_Axis, 1}
};

constexpr ParamInfo kParams_OnePerk_OneInt[] =
{
	{kParamType_Perk},
	{kParamType_Integer}
};

constexpr ParamInfo kParams_OnePerk_TwoInts[] =
{
	{kParamType_Perk},
	{kParamType_Integer},
	{kParamType_Integer}
};

constexpr ParamInfo kParams_OneList_OneInt_TwoOptionalInts[] =
{
	{kParamType_FormList},
	{kParamType_Integer},
	{kParamType_Integer, 1},
	{kParamType_Integer, 1}
};

constexpr ParamInfo kParams_OneString_OneOptionalInt[] =
{
	{kParamType_String},
	{kParamType_Integer, 1}
};

constexpr ParamInfo kParams_ThreeForms[] =
{
	{kParamType_AnyForm},
	{kParamType_AnyForm},
	{kParamType_AnyForm}
};

constexpr ParamInfo kParams_TwoForms_TwoInts_OneFloat[] =
{
	{kParamType_AnyForm},
	{kParamType_AnyForm},
	{kParamType_Integer},
	{kParamType_Integer},
	{kParamType_Float}
};

constexpr ParamInfo kParams_OneForm_OneInt_OneFloat_OneOptionalForm[] =
{
	{kParamType_AnyForm},
	{kParamType_Integer},
	{kParamType_Float},
	{kParamType_AnyForm, 1}
};

constexpr ParamInfo kParams_OneInt_TwoOptionalInts[] =
{
	{kParamType_Integer},
	{kParamType_Integer, 1},
	{kParamType_Integer, 1}
};

constexpr ParamInfo kParams_OneObjectID_OneOptionalFloat_ThreeOptionalInts[] =
{
	{kParamType_ObjectID},
	{kParamType_Float, 1},
	{kParamType_Integer, 1},
	{kParamType_Integer, 1},
	{kParamType_Integer, 1}
};

constexpr ParamInfo kParams_OneFloat_OneOptionalForm[] =
{
	{kParamType_Float},
	{kParamType_AnyForm, 1}
};

constexpr ParamInfo kParams_OneInt_OneForm_OneOptionalFloat_OneOptionalInt[] =
{
	{kParamType_Integer},
	{kParamType_AnyForm},
	{kParamType_Float, 1},
	{kParamType_Integer, 1}
};

constexpr ParamInfo kParams_OneForm_OneOptionalInt[] =
{
	{kParamType_AnyForm},
	{kParamType_Integer, 1}
};

constexpr ParamInfo kParams_OneForm_OneInt_OneOptionalInt[] =
{
	{kParamType_AnyForm},
	{kParamType_Integer},
	{kParamType_Integer, 1}
};

constexpr ParamInfo kParams_TwoForms_OneInt[] =
{
	{kParamType_AnyForm},
	{kParamType_AnyForm},
	{kParamType_Integer}
};

constexpr ParamInfo kParams_FormCondition[] =
{
	{kParamType_AnyForm},
	{kParamType_Integer},
	{kParamType_AnyForm},
	{kParamType_Integer},
	{kParamType_Float}
};

constexpr ParamInfo kParams_OneString_OneOptionalString[] =
{
	{kParamType_String},
	{kParamType_String, 1}
};

constexpr ParamInfo kParams_OneString_OneDouble_OneOptionalString[] =
{
	{kParamType_String},
	{kParamType_Double},
	{kParamType_String, 1}
};

constexpr ParamInfo kParams_TwoString_OneOptionalString[] =
{
	{kParamType_String},
	{kParamType_String},
	{kParamType_String, 1}
};

constexpr ParamInfo kParams_OneString_OneInt_OneOptionalString[] =
{
	{kParamType_String},
	{kParamType_Integer},
	{kParamType_String, 1}
};

constexpr ParamInfo kParams_OneOptionalString[] =
{
	{kParamType_String, 1}
};

constexpr ParamInfo kParams_OneInt_OneOptionalObjectRef[] =
{
	{kParamType_Integer},
	{kParamType_ObjectRef, 1}
};

constexpr ParamInfo kParams_OneActorBase[] =
{
	{kParamType_ActorBase}
};

constexpr ParamInfo kParams_OneActorBase_OneInt[] =
{
	{kParamType_ActorBase},
	{kParamType_Integer}
};

constexpr ParamInfo kParams_OneActorBase_TwoInts[] =
{
	{kParamType_ActorBase},
	{kParamType_Integer},
	{kParamType_Integer}
};

constexpr ParamInfo kParams_TwoInts_OneOptionalActorBase[] =
{
	{kParamType_Integer},
	{kParamType_Integer},
	{kParamType_ActorBase, 1}
};

constexpr ParamInfo kParams_OneFaction_OneInt_OneOptionalActorBase[] =
{
	{kParamType_Faction},
	{kParamType_Integer},
	{kParamType_ActorBase, 1}
};

constexpr ParamInfo kParams_OneImageSpace_OneInt[] =
{
	{kParamType_ImageSpace},
	{kParamType_Integer}
};

constexpr ParamInfo kParams_OneImageSpace_OneInt_OneFloat[] =
{
	{kParamType_ImageSpace},
	{kParamType_Integer},
	{kParamType_Float}
};

constexpr ParamInfo kParams_OneIMOD_OneInt[] =
{
	{kParamType_ImageSpaceModifier},
	{kParamType_Integer}
};

constexpr ParamInfo kParams_OneIMOD_OneInt_OneFloat[] =
{
	{kParamType_ImageSpaceModifier},
	{kParamType_Integer},
	{kParamType_Float}
};

constexpr ParamInfo kParams_OneForm_ThreeOptionalInts[] =
{
	{kParamType_AnyForm},
	{kParamType_Integer, 1},
	{kParamType_Integer, 1},
	{kParamType_Integer, 1}
};

constexpr ParamInfo kParams_OneGlobal_OneOptionalForm[] =
{
	{kParamType_Global},
	{kParamType_AnyForm, 1}
};

constexpr ParamInfo kParams_TwoOptionalInts_OneOptionalContainer[] =
{
	{kParamType_Integer, 1},
	{kParamType_Integer, 1},
	{kParamType_Container, 1}
};

constexpr ParamInfo kParams_OneForm_OneString_OneInt[] =
{
	{kParamType_AnyForm},
	{kParamType_String},
	{kParamType_Integer}
};

constexpr ParamInfo kParams_OneMagicItem[] =
{
	{kParamType_MagicItem}
};

constexpr ParamInfo kParams_FourOptionalFloats_OneOptionalInt[] =
{
	{kParamType_Float, 1},
	{kParamType_Float, 1},
	{kParamType_Float, 1},
	{kParamType_Float, 1},
	{kParamType_Integer, 1}
};

constexpr ParamInfo kParams_OneInt_OneFloat_TwoOptionalForms[] =
{
	{kParamType_Integer},
	{kParamType_Float},
	{kParamType_AnyForm, 1},
	{kParamType_AnyForm, 1}
};

constexpr ParamInfo kParams_OneForm_OneInt_OneQuest_OneInt_OneOptionalInt[] =
{
	{kParamType_AnyForm},
	{kParamType_Integer},
	{kParamType_Quest},
	{kParamType_Integer},
	{kParamType_Integer, 1}
};

constexpr ParamInfo kParams_OneForm_OneInt_ThreeOptionalForms[] =
{
	{kParamType_AnyForm},
	{kParamType_Integer},
	{kParamType_AnyForm, 1},
	{kParamType_AnyForm, 1},
	{kParamType_AnyForm, 1}
};

constexpr ParamInfo kParams_OneObjectID_OneInt_OneFormatString[] =
{
	{kParamType_ObjectID},
	{kParamType_Integer},
	FORMAT_STRING_PARAMS
};

constexpr ParamInfo kParams_OneOptionalObjectRef_TwoOptionalInts[] =
{
	{kParamType_ObjectRef, 1},
	{kParamType_Integer, 1},
	{kParamType_Integer, 1}
};

constexpr ParamInfo kParams_OneObjectRef_OneFloat[] =
{
	{kParamType_ObjectRef},
	{kParamType_Float}
};

constexpr ParamInfo kParams_OneQuest_TwoInts_OneObjectRef[] =
{
	{kParamType_Quest},
	{kParamType_Integer},
	{kParamType_Integer},
	{kParamType_ObjectRef}
};

constexpr ParamInfo kParams_OneQuest_OneInt_OneObjectRef[] =
{
	{kParamType_Quest},
	{kParamType_Integer},
	{kParamType_ObjectRef}
};

constexpr ParamInfo kParams_OneQuest_OneInt_OneFormatString[] =
{
	{kParamType_Quest},
	{kParamType_Integer},
	FORMAT_STRING_PARAMS
};

constexpr ParamInfo kParams_OneObjectID_OneOptionalObjectID[] =
{
	{kParamType_ObjectID},
	{kParamType_ObjectID, 1}
};

constexpr ParamInfo kParams_OneForm_OneOptionalString[] =
{
	{kParamType_AnyForm},
	{kParamType_String, 1}
};

constexpr ParamInfo kParams_OneSpellItem_OneOptionalActorBase[] =
{
	{kParamType_SpellItem},
	{kParamType_ActorBase, 1}
};

constexpr ParamInfo kParams_OneMagicItem_OneOptionalActor[] =
{
	{kParamType_MagicItem},
	{kParamType_Actor, 1}
};

constexpr ParamInfo kParams_ThreeOptionalInts[] =
{
	{kParamType_Integer, 1},
	{kParamType_Integer, 1},
	{kParamType_Integer, 1}
};

constexpr ParamInfo kParams_OneAIPackage_ThreeInts[] =
{
	{kParamType_AIPackage},
	{kParamType_Integer},
	{kParamType_Integer},
	{kParamType_Integer}
};

constexpr ParamInfo kParams_OneForm_OneInt_OneDouble[] =
{
	{kParamType_AnyForm},
	{kParamType_Integer},
	{kParamType_Double}
};

constexpr ParamInfo kParams_OneDouble[] =
{
	{kParamType_Double}
};

constexpr ParamInfo kParams_FourFloats_OneOptionalObjectRef[] =
{
	{kParamType_Float},
	{kParamType_Float},
	{kParamType_Float},
	{kParamType_Float},
	{kParamType_ObjectRef, 1}
};

constexpr ParamInfo kParams_OneActorBase_OneOptionalString[] =
{
	{kParamType_ActorBase},
	{kParamType_String, 1}
};

constexpr ParamInfo kParams_OneActorBase_OneString[] =
{
	{kParamType_ActorBase},
	{kParamType_String}
};

constexpr ParamInfo kParams_OneActorBase_OneForm[] =
{
	{kParamType_ActorBase},
	{kParamType_AnyForm}
};

constexpr ParamInfo kParams_OneString_ThreeFloats_OneOptionalInt[] =
{
	{kParamType_String},
	{kParamType_Float},
	{kParamType_Float},
	{kParamType_Float},
	{kParamType_Integer, 1}
};

constexpr ParamInfo kParams_OneString_ThreeFloats_TwoOptionalInts[] =
{
	{kParamType_String},
	{kParamType_Float},
	{kParamType_Float},
	{kParamType_Float},
	{kParamType_Integer, 1},
	{kParamType_Integer, 1}
};

constexpr ParamInfo kParams_OneString_OneOptionalAxis[] =
{
	{kParamType_String},
	{kParamType_Axis, 1}
};

constexpr ParamInfo kParams_OneString_OneAxis_OneFloat[] =
{
	{kParamType_String},
	{kParamType_Axis},
	{kParamType_Float}
};

constexpr ParamInfo kParams_OneForm_OneInt_OneForm_ThreeFloats[] =
{
	{kParamType_AnyForm},
	{kParamType_Integer},
	{kParamType_AnyForm},
	{kParamType_Float},
	{kParamType_Float},
	{kParamType_Float}
};

constexpr ParamInfo kParams_OneActorValue_OneOptionalInt[] =
{
	{kParamType_ActorValue},
	{kParamType_Integer, 1}
};

constexpr ParamInfo kParams_ThreeGlobals_OneOptFloat_OneOptInt_OneOptString_OneOptFloat[] =
{
	{kParamType_Global},
	{kParamType_Global},
	{kParamType_Global},
	{kParamType_Float, 1},
	{kParamType_Integer, 1},
	{kParamType_String, 1},
	{kParamType_Float, 1}
};

constexpr ParamInfo kParams_OneObjectRef_OneString_ThreeOptionalFloats[] =
{
	{kParamType_ObjectRef},
	{kParamType_String},
	{kParamType_Float, 1},
	{kParamType_Float, 1},
	{kParamType_Float, 1}
};

constexpr ParamInfo kParams_OneObjectID_OneOptionalActorBase[] =
{
	{kParamType_ObjectID},
	{kParamType_ActorBase, 1}
};

constexpr ParamInfo kParams_OneString_OneForm_ThreeOptionalFloats[] =
{
	{kParamType_String},
	{kParamType_AnyForm},
	{kParamType_Float, 1},
	{kParamType_Float, 1},
	{kParamType_Float, 1}
};

constexpr ParamInfo kParams_OneEffect[] =
{
	{kParamType_MagicEffect}
};

constexpr ParamInfo kParams_OneQuest_TwoInts[] =
{
	{kParamType_Quest},
	{kParamType_Integer},
	{kParamType_Integer}
};

constexpr ParamInfo kParams_OneFloat_ThreeOptionalFloats_OneOptionalInt[] =
{
	{kParamType_Float},
	{kParamType_Float, 1},
	{kParamType_Float, 1},
	{kParamType_Float, 1},
	{kParamType_Integer, 1}
};

constexpr ParamInfo kParams_OneString_OneOptionalString_OneOptionalInt[] =
{
	{kParamType_String},
	{kParamType_String, 1},
	{kParamType_Integer, 1}
};

constexpr ParamInfo kParams_OneObjectRef_OneFloat_OneOptionalInt[] =
{
	{kParamType_ObjectRef},
	{kParamType_Float},
	{kParamType_Integer, 1}
};

constexpr ParamInfo kParams_OneObjectID_OneInt_OneFloat[] =
{
	{kParamType_ObjectID},
	{kParamType_Integer},
	{kParamType_Float}
};

constexpr ParamInfo kParams_TwoFloats_ThreeOptionalInts[] =
{
	{kParamType_Float},
	{kParamType_Float},
	{kParamType_Integer, 1},
	{kParamType_Integer, 1},
	{kParamType_Integer, 1}
};

constexpr ParamInfo kParams_OneNote[] =
{
	{kParamType_Note}
};

constexpr ParamInfo kParams_OneForm_OneOptionalSpellItem[] =
{
	{kParamType_AnyForm},
	{kParamType_SpellItem, 1}
};

constexpr ParamInfo kParams_TwoFloats_OneOptionalObjectRef_OneOptionalInt[] =
{
	{kParamType_Float},
	{kParamType_Float},
	{kParamType_ObjectRef, 1},
	{kParamType_Integer, 1}
};

constexpr ParamInfo kParams_OneString_ThreeOptionalFloats_OneOptionalInt[] =
{
	{kParamType_String},
	{kParamType_Float, 1},
	{kParamType_Float, 1},
	{kParamType_Float, 1},
	{kParamType_Integer, 1}
};

constexpr ParamInfo kParams_OneForm_OneInt_OneFormatString[] =
{
	{kParamType_AnyForm},
	{kParamType_Integer},
	FORMAT_STRING_PARAMS
};

constexpr ParamInfo kParams_OneOptionalObjectRef_OneOptionalInt_OneOptionalFloat_OneOptionalString[] =
{
	{kParamType_ObjectRef, 1},
	{kParamType_Integer, 1},
	{kParamType_Float, 1},
	{kParamType_String, 1}
};

constexpr ParamInfo kParams_OneOptionalInt_OneOptionalString[] =
{
	{kParamType_Integer, 1},
	{kParamType_String, 1}
};

constexpr ParamInfo kParams_OneObjectID_OneOptionalString[] =
{
	{kParamType_ObjectID},
	{kParamType_String, 1}
};

constexpr ParamInfo kParams_OneString_OneDouble[] =
{
	{kParamType_String},
	{kParamType_Double}
};

constexpr ParamInfo kParams_TwoScriptVars[] =
{
	{kParamType_ScriptVariable},
	{kParamType_ScriptVariable}
};

constexpr ParamInfo kParams_OneString_OneFloat_OneOptionalInt[] =
{
	{kParamType_String},
	{kParamType_Float},
	{kParamType_Integer, 1}
};

constexpr ParamInfo kParams_OneString_OneInt_OneOptionalInt[] =
{
	{kParamType_String},
	{kParamType_Integer},
	{kParamType_Integer, 1}
};

constexpr ParamInfo kParams_OneString_TwoInts_OneOptionalInt[] =
{
	{kParamType_String},
	{kParamType_Integer},
	{kParamType_Integer},
	{kParamType_Integer, 1}
};

constexpr ParamInfo kParams_OneOptionalFloat_OneOptionalInt[] =
{
	{kParamType_Float, 1},
	{kParamType_Integer, 1}
};

constexpr ParamInfo kParams_OneOptionalString_TwoOptionalInts[] =
{
	{kParamType_String, 1},
	{kParamType_Integer, 1},
	{kParamType_Integer, 1}
};

constexpr ParamInfo kParams_ThreeScriptVars[] =
{
	{kParamType_ScriptVariable},
	{kParamType_ScriptVariable},
	{kParamType_ScriptVariable}
};

constexpr ParamInfo kParams_ThreeScriptVars_OneOptionalInt[] =
{
	{kParamType_ScriptVariable},
	{kParamType_ScriptVariable},
	{kParamType_ScriptVariable},
	{kParamType_Integer, 1}
};

constexpr ParamInfo kParams_OneString_TwoInts_OneFloat[] =
{
	{kParamType_String},
	{kParamType_Integer},
	{kParamType_Integer},
	{kParamType_Float}
};

constexpr ParamInfo kParams_TwoStrings_OneFloat_ThreeOptionalInts[] =
{
	{kParamType_String},
	{kParamType_String},
	{kParamType_Float},
	{kParamType_Integer, 1},
	{kParamType_Integer, 1},
	{kParamType_Integer, 1}
};

constexpr ParamInfo kParams_TwoStrings_OneOptionalInt[] =
{
	{kParamType_String},
	{kParamType_String},
	{kParamType_Integer, 1}
};

constexpr ParamInfo kParams_OnePerk[] =
{
	{kParamType_Perk}
};

constexpr ParamInfo kParams_OneChallenge[] =
{
	{kParamType_Challenge}
};

constexpr ParamInfo kParams_OneChallenge_OneInt[] =
{
	{kParamType_Challenge},
	{kParamType_Integer}
};

constexpr ParamInfo kParams_OneChallenge_OneForm[] =
{
	{kParamType_Challenge},
	{kParamType_AnyForm}
};

constexpr ParamInfo kParams_OneCasino[] =
{
	{kParamType_Casino}
};

constexpr ParamInfo kParams_OneCasino_OneInt[] =
{
	{kParamType_Casino},
	{kParamType_Integer}
};

constexpr ParamInfo kParams_OneCasino_OneFloat[] =
{
	{kParamType_Casino},
	{kParamType_Float}
};

constexpr ParamInfo kParams_FiveFloats_ThreeScriptVars_OneOptionalInt_OneOptionalFloat[] =
{
	{kParamType_Float},
	{kParamType_Float},
	{kParamType_Float},
	{kParamType_Float},
	{kParamType_Float},
	{kParamType_ScriptVariable},
	{kParamType_ScriptVariable},
	{kParamType_ScriptVariable},
	{kParamType_Integer, 1},
	{kParamType_Float, 1}
};

constexpr ParamInfo kParams_ThreeFloats_OneOptionalInt[] =
{
	{kParamType_Float},
	{kParamType_Float},
	{kParamType_Float},
	{kParamType_Integer, 1}
};

constexpr ParamInfo kParams_ThreeFloats_ThreeScriptVars[] =
{
	{kParamType_Float},
	{kParamType_Float},
	{kParamType_Float},
	{kParamType_ScriptVariable},
	{kParamType_ScriptVariable},
	{kParamType_ScriptVariable}
};

constexpr ParamInfo kParams_OneIdleForm[] =
{
	{kParamType_IdleForm}
};

constexpr ParamInfo kParams_OneOptionalIdleForm[] =
{
	{kParamType_IdleForm, 1}
};

constexpr ParamInfo kParams_OneOptionalInt_OneOptionalFloat[] =
{
	{kParamType_Integer, 1},
	{kParamType_Float, 1}
};

constexpr ParamInfo kParams_OneString_ThreeScriptVars_TwoOptionalInts[] =
{
	{kParamType_String},
	{kParamType_ScriptVariable},
	{kParamType_ScriptVariable},
	{kParamType_ScriptVariable},
	{kParamType_Integer, 1},
	{kParamType_Integer, 1}
};

constexpr ParamInfo kParams_OneString_ThreeScriptVars_OneOptionalInt[] =
{
	{kParamType_String},
	{kParamType_ScriptVariable},
	{kParamType_ScriptVariable},
	{kParamType_ScriptVariable},
	{kParamType_Integer, 1}
};

constexpr ParamInfo kParams_OneString_SixFloats_OneOptionalFloat[] =
{
	{kParamType_String},
	{kParamType_Float},
	{kParamType_Float},
	{kParamType_Float},
	{kParamType_Float},
	{kParamType_Float},
	{kParamType_Float},
	{kParamType_Float, 1}
};

constexpr ParamInfo kParams_TwoStrings_FourFloats[] =
{
	{kParamType_String},
	{kParamType_String},
	{kParamType_Float},
	{kParamType_Float},
	{kParamType_Float},
	{kParamType_Float}
};

constexpr ParamInfo kParams_TwoStrings_FiveFloats[] =
{
	{kParamType_String},
	{kParamType_String},
	{kParamType_Float},
	{kParamType_Float},
	{kParamType_Float},
	{kParamType_Float},
	{kParamType_Float}
};

constexpr ParamInfo kParams_OneOptionalFloat[] =
{
	{kParamType_Float, 1}
};

constexpr ParamInfo kParams_ThreeScriptVars_OneOptionalInt_OneOptionalFloat[] =
{
	{kParamType_ScriptVariable},
	{kParamType_ScriptVariable},
	{kParamType_ScriptVariable},
	{kParamType_Integer, 1},
	{kParamType_Float, 1}
};

constexpr ParamInfo kParams_NineFloats_TwoScriptVars[] =
{
	{kParamType_Float},
	{kParamType_Float},
	{kParamType_Float},
	{kParamType_Float},
	{kParamType_Float},
	{kParamType_Float},
	{kParamType_Float},
	{kParamType_Float},
	{kParamType_Float},
	{kParamType_ScriptVariable},
	{kParamType_ScriptVariable}
};

constexpr ParamInfo kParams_SixFloats_TwoScriptVars[] =
{
	{kParamType_Float},
	{kParamType_Float},
	{kParamType_Float},
	{kParamType_Float},
	{kParamType_Float},
	{kParamType_Float},
	{kParamType_ScriptVariable},
	{kParamType_ScriptVariable}
};

constexpr ParamInfo kParams_SixFloats_OneOptionalInt[] =
{
	{kParamType_Float},
	{kParamType_Float},
	{kParamType_Float},
	{kParamType_Float},
	{kParamType_Float},
	{kParamType_Float},
	{kParamType_Integer, 1}
};

constexpr ParamInfo kParams_OneString_OneInt_OneFloat_ThreeScriptVars[] =
{
	{kParamType_String},
	{kParamType_Integer},
	{kParamType_Float},
	{kParamType_ScriptVariable},
	{kParamType_ScriptVariable},
	{kParamType_ScriptVariable}
};

constexpr ParamInfo kParams_OneString_OneInt_OneOptionalString_OneOptionalInt[] =
{
	{kParamType_String},
	{kParamType_Integer},
	{kParamType_String, 1},
	{kParamType_Integer, 1}
};

constexpr ParamInfo kParams_OneFloat_OneOptionalInt[] =
{
	{kParamType_Float},
	{kParamType_Integer, 1}
};

constexpr ParamInfo kParams_ThreeScriptVars_ThreeOptionalScriptVars[] =
{
	{kParamType_ScriptVariable},
	{kParamType_ScriptVariable},
	{kParamType_ScriptVariable},
	{kParamType_ScriptVariable, 1},
	{kParamType_ScriptVariable, 1},
	{kParamType_ScriptVariable, 1}
};

constexpr ParamInfo kParams_NVSE_OneNum_OneOptionalNum[] =
{
	{kNVSEParamType_Number},
	{kNVSEParamType_Number, 1}
};

constexpr ParamInfo kParams_NVSE_OneOptionalNum_OneOptionalBasicType[] =
{
	{kNVSEParamType_Number, 1},
	{kNVSEParamType_BasicType, 1}
};

constexpr ParamInfo kParams_OneInt_ThreeOptionalInts[] =
{
	{kParamType_Integer},
	{kParamType_Integer, 1},
	{kParamType_Integer, 1},
	{kParamType_Integer, 1}
};

constexpr ParamInfo kParams_OneOptionalInt_OneOptionalFloat_OneOptionalInt[] =
{
	{kParamType_Integer, 1},
	{kParamType_Float, 1},
	{kParamType_Integer, 1}
};