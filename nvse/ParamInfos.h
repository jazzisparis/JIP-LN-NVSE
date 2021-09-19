#pragma once

ParamInfo kParams_OneInt[] =
{
	{"Integer", kParamType_Integer, 0}
};

ParamInfo kParams_TwoInts[] =
{
	{"Integer", kParamType_Integer, 0},
	{"Integer", kParamType_Integer, 0}
};

ParamInfo kParams_OneOptionalInt[] =
{
	{"Integer", kParamType_Integer, 1}
};

ParamInfo kParams_OneInt_OneOptionalInt[] =
{
	{"Integer", kParamType_Integer, 0},
	{"Integer", kParamType_Integer, 1}
};

ParamInfo kParams_OneFloat[] =
{
	{"Float", kParamType_Float, 0}
};

ParamInfo kParams_OneString[] =
{
	{"String", kParamType_String, 0}
};

ParamInfo kParams_OneString_OneFloat[] =
{
	{"String", kParamType_String, 0},
	{"Float", kParamType_Float, 0}
};

ParamInfo kParams_TwoFloats[] =
{
	{"Float", kParamType_Float, 0},
	{"Float", kParamType_Float, 0}
};

ParamInfo kParams_OneObjectID[] =
{
	{"Object ID", kParamType_ObjectID, 0}
};

ParamInfo kParams_OneOptionalObjectID[] =
{
	{"Object ID", kParamType_ObjectID, 1}
};

ParamInfo kParams_OneObjectID_OneInt[] =
{
	{"Object ID", kParamType_ObjectID, 0},
	{"Integer", kParamType_Integer, 0}
};

 #define FORMAT_STRING_PARAMS 	\
	{"format string",	kParamType_String, 0}, \
	{"variable",		kParamType_Float, 1}, \
	{"variable",		kParamType_Float, 1}, \
	{"variable",		kParamType_Float, 1}, \
	{"variable",		kParamType_Float, 1}, \
	{"variable",		kParamType_Float, 1}, \
	{"variable",		kParamType_Float, 1}, \
	{"variable",		kParamType_Float, 1}, \
	{"variable",		kParamType_Float, 1}, \
	{"variable",		kParamType_Float, 1}, \
	{"variable",		kParamType_Float, 1}, \
	{"variable",		kParamType_Float, 1}, \
	{"variable",		kParamType_Float, 1}, \
	{"variable",		kParamType_Float, 1}, \
	{"variable",		kParamType_Float, 1}, \
	{"variable",		kParamType_Float, 1}, \
	{"variable",		kParamType_Float, 1}, \
	{"variable",		kParamType_Float, 1}, \
	{"variable",		kParamType_Float, 1}, \
	{"variable",		kParamType_Float, 1}, \
	{"variable",		kParamType_Float, 1} 

ParamInfo kParams_FormatString[] =
{
	FORMAT_STRING_PARAMS
};

ParamInfo kParams_OneActor[] =
{
	{"Actor", kParamType_Actor, 0}
};

ParamInfo kParams_OneAxis[] = 
{
	{"Axis", kParamType_Axis, 0}
};

ParamInfo kParams_FormList[] =
{
	{"List", kParamType_FormList, 0}
};

ParamInfo kParams_TwoStrings[] =
{
	{"String", kParamType_String, 0},
	{"String", kParamType_String, 0}
};

 ParamInfo kParams_OneOptionalObject[] =
 {
	 {"TESObject", kParamType_TESObject, 1}
 };
 
ParamInfo kParams_OneInt_OneOptionalObject[] =
{
	{"Integer", kParamType_Integer, 0},
	{"TESObject", kParamType_TESObject, 1}
};

ParamInfo kParams_OneQuest[] =
{
	{"Quest", kParamType_Quest, 0}
};

ParamInfo kParams_OneOptionalObjectRef[] =
{
	{"Object Ref", kParamType_ObjectRef, 1}
};

ParamInfo kParams_OneForm_OneInt[] =
{
	{"Form", kParamType_AnyForm, 0},
	{"Integer", kParamType_Integer, 0}
};

ParamInfo kParams_OneForm[] =
{
	{"Form", kParamType_AnyForm, 0}
};

ParamInfo kParams_OneForm_OneFloat[] =
{
	{"Form", kParamType_AnyForm, 0},
	{"Float", kParamType_Float, 0}
};

ParamInfo kParams_OneOptionalForm[] =
{
	{"Form", kParamType_AnyForm, 1}
};

ParamInfo kParams_OneObjectID_TwoOptionalInts[] =
{
	{"Object ID", kParamType_ObjectID, 0},
	{"Integer", kParamType_Integer, 1},
	{"Integer", kParamType_Integer, 1}
};

ParamInfo kParams_OneOptionalActorBase[] =
{
	{"Actor Base", kParamType_ActorBase, 1}
};

ParamInfo kParams_OneInt_OneOptionalActorBase[] =
{
	{"Integer", kParamType_Integer, 0},
	{"Actor Base", kParamType_ActorBase, 1}
};

ParamInfo kParams_OneItemOrList_OneInt_OneOptionalFloat_OneOptionalInt[] =
{
	{"Item/List", kParamType_InvObjOrFormList, 0},
	{"Integer", kParamType_Integer, 0},
	{"Float", kParamType_Float, 1},
	{"Integer", kParamType_Integer, 1}
};

ParamInfo kParams_OneString_OneFormatString[] =
{
	{"String", kParamType_String, 0},
	FORMAT_STRING_PARAMS
};

ParamInfo kParams_OneForm_OneInt_OneFloat[] =
{
	{"Form", kParamType_AnyForm, 0},
	{"Integer", kParamType_Integer, 0},
	{"Float", kParamType_Float, 0}
};

ParamInfo kParams_OneForm_TwoInts[] =
{
	{"Form", kParamType_AnyForm, 0},
	{"Integer", kParamType_Integer, 0},
	{"Integer", kParamType_Integer, 0}
};

ParamInfo kParams_TwoForms[] =
{
	{"Form", kParamType_AnyForm, 0},
	{"Form", kParamType_AnyForm, 0}
};

ParamInfo kParams_OneForm_OneInt_OneOptionalForm[] =
{
	{"Form", kParamType_AnyForm, 0},
	{"Integer", kParamType_Integer, 0},
	{"Form", kParamType_AnyForm, 1}
};

ParamInfo kParams_OneString_OneOptionalQuest[] =
{
	{"String", kParamType_String, 0},
	{"Quest", kParamType_Quest, 1}
};

ParamInfo kParams_OneQuest_OneFloat[] =
{
	{"Quest", kParamType_Quest, 0},
	{"Float", kParamType_Float, 0}
};

ParamInfo kParams_OneAIPackage_OneInt[] =
{
	{"Package", kParamType_AIPackage, 0},
	{"Integer", kParamType_Integer, 0}
};

ParamInfo kParams_OneAIPackage_TwoInts[] =
{
	{"Package", kParamType_AIPackage, 0},
	{"Integer", kParamType_Integer, 0},
	{"Integer", kParamType_Integer, 0}
};

ParamInfo kParams_OneGlobal[] =
{
	{"Global", kParamType_Global, 0}
};

ParamInfo kParams_OneGlobal_OneFloat[] =
{
	{"Global", kParamType_Global, 0},
	{"Float", kParamType_Float, 0}
};

ParamInfo kParams_OneFaction[] =
{
	{"Faction", kParamType_Faction, 0}
};

ParamInfo kParams_OneFaction_OneOptionalReputation[] =
{
	{"Faction", kParamType_Faction, 0},
	{"Reputation", kParamType_Reputation, 1}
};

ParamInfo kParams_OneAxis_OneOptionalForm[] =
{
	{"Axis", kParamType_Axis, 0},
	{"Form", kParamType_AnyForm, 1}
};

ParamInfo kParams_TwoDoubles_ThreeOptionalDoubles[] =
{
	{"Double", kParamType_Double, 0},
	{"Double", kParamType_Double, 0},
	{"Double", kParamType_Double, 1},
	{"Double", kParamType_Double, 1},
	{"Double", kParamType_Double, 1}
};

ParamInfo kParams_OneCombatStyle_OneInt[] =
{
	{"Combat Style", kParamType_CombatStyle, 0},
	{"Integer", kParamType_Integer, 0}
};

ParamInfo kParams_OneCombatStyle_OneInt_OneFloat[] =
{
	{"Combat Style", kParamType_CombatStyle, 0},
	{"Integer", kParamType_Integer, 0},
	{"Float", kParamType_Float, 0}
};

ParamInfo kParams_OneCombatStyle_TwoInts[] =
{
	{"Combat Style", kParamType_CombatStyle, 0},
	{"Integer", kParamType_Integer, 0},
	{"Integer", kParamType_Integer, 0}
};

ParamInfo kParams_OneString_TwoOptionalInts[] =
{
	{"String", kParamType_String, 0},
	{"Integer", kParamType_Integer, 1},
	{"Integer", kParamType_Integer, 1}
};

ParamInfo kParams_OneString_TwoInts[] =
{
	{"String", kParamType_String, 0},
	{"Integer", kParamType_Integer, 0},
	{"Integer", kParamType_Integer, 0}
};

ParamInfo kParams_OneString_OneInt_OneFormatString[] =
{
	{"String", kParamType_String, 0},
	{"Integer", kParamType_Integer, 0},
	FORMAT_STRING_PARAMS
};

ParamInfo kParams_OneClass_OneInt[] =
{
	{"Class", kParamType_Class, 0},
	{"Integer", kParamType_Integer, 0}
};

ParamInfo kParams_OneClass_TwoInts[] =
{
	{"Class", kParamType_Class, 0},
	{"Integer", kParamType_Integer, 0},
	{"Integer", kParamType_Integer, 0}
};

ParamInfo kParams_OneSound_OneInt[] =
{
	{"Sound", kParamType_Sound, 0},
	{"Integer", kParamType_Integer, 0}
};

ParamInfo kParams_OneSound_OneInt_OneFloat[] =
{
	{"Sound", kParamType_Sound, 0},
	{"Integer", kParamType_Integer, 0},
	{"Float", kParamType_Float, 0}
};

ParamInfo kParams_OneSound_TwoInts[] =
{
	{"Sound", kParamType_Sound, 0},
	{"Integer", kParamType_Integer, 0},
	{"Integer", kParamType_Integer, 0}
};

ParamInfo kParams_OneForm_OneInt_OneOptionalSound[] =
{
	{"Form", kParamType_AnyForm, 0},
	{"Integer", kParamType_Integer, 0},
	{"Sound", kParamType_Sound, 1}
};

ParamInfo kParams_OneForm_OneInt_OneOptionalObjectID[] =
{
	{"Form", kParamType_AnyForm, 0},
	{"Integer", kParamType_Integer, 0},
	{"Object ID", kParamType_ObjectID, 1}
};

ParamInfo kParams_OneWeatherID_OneInt[] =
{
	{"Weather ID", kParamType_WeatherID, 0},
	{"Integer", kParamType_Integer, 0}
};

ParamInfo kParams_OneWeatherID_OneInt_OneOptionalImageSpaceMod[] =
{
	{"Weather ID", kParamType_WeatherID, 0},
	{"Integer", kParamType_Integer, 0},
	{"Image Space Mod", kParamType_ImageSpaceModifier, 1}
};

ParamInfo kParams_OneWeatherID_OneInt_OneString[] =
{
	{"Weather ID", kParamType_WeatherID, 0},
	{"Integer", kParamType_Integer, 0},
	{"String", kParamType_String, 0}
};

ParamInfo kParams_OneWeatherID[] =
{
	{"Weather ID", kParamType_WeatherID, 0}
};

ParamInfo kParams_OneWeatherID_OneString[] =
{
	{"Weather ID", kParamType_WeatherID, 0},
	{"String", kParamType_String, 0}
};

ParamInfo kParams_OneWeatherID_OneInt_OneDouble[] =
{
	{"Weather ID", kParamType_WeatherID, 0},
	{"Integer", kParamType_Integer, 0},
	{"Double", kParamType_Double, 0}
};

ParamInfo kParams_OneWeatherID_TwoInts_OneOptionalInt[] =
{
	{"Weather ID", kParamType_WeatherID, 0},
	{"Integer", kParamType_Integer, 0},
	{"Integer", kParamType_Integer, 0},
	{"Integer", kParamType_Integer, 1}
};

ParamInfo kParams_OneWeatherID_ThreeInts_OneOptionalInt[] =
{
	{"Weather ID", kParamType_WeatherID, 0},
	{"Integer", kParamType_Integer, 0},
	{"Integer", kParamType_Integer, 0},
	{"Integer", kParamType_Integer, 0},
	{"Integer", kParamType_Integer, 1}
};

ParamInfo kParams_OneForm_OneInt_OneString[] =
{
	{"Form", kParamType_AnyForm, 0},
	{"Integer", kParamType_Integer, 0},
	{"String", kParamType_String, 0}
};

ParamInfo kParams_OneForm_OneString[] =
{
	{"Form", kParamType_AnyForm, 0},
	{"String", kParamType_String, 0}
};

ParamInfo kParams_OneForm_OneWeatherID_OneInt_OneOptionalGlobal[] =
{
	{"Form", kParamType_AnyForm, 0},
	{"Weather ID", kParamType_WeatherID, 0},
	{"Integer", kParamType_Integer, 0},
	{"Global", kParamType_Global, 1}
};

ParamInfo kParams_OneForm_OneWeatherID[] =
{
	{"Form", kParamType_AnyForm, 0},
	{"Weather ID", kParamType_WeatherID, 0}
};

ParamInfo kParams_OneAxis_OneFloat[] =
{
	{"Axis", kParamType_Axis, 0},
	{"Float", kParamType_Float, 0}
};

ParamInfo kParams_OneForm_ThreeInts[] =
{
	{"Form", kParamType_AnyForm, 0},
	{"Integer", kParamType_Integer, 0},
	{"Integer", kParamType_Integer, 0},
	{"Integer", kParamType_Integer, 0}
};

ParamInfo kParams_OneItemOrList_OneContainer_TwoOptionalInts[] =
{
	{"Item/List", kParamType_InvObjOrFormList, 0},
	{"Container", kParamType_Container, 0},
	{"Integer", kParamType_Integer, 1},
	{"Integer", kParamType_Integer, 1}
};

ParamInfo kParams_OneInt_OneFloat_OneOptionalActorBase[] =
{
	{"Integer", kParamType_Integer, 0},
	{"Float", kParamType_Float, 0},
	{"Actor Base", kParamType_ActorBase, 1}
};

ParamInfo kParams_OneMagicItem_OneInt_OneMagicEffect[] =
{
	{"Magic Item", kParamType_MagicItem, 0},
	{"Integer", kParamType_Integer, 0},
	{"Magic Effect", kParamType_MagicEffect, 0}
};

ParamInfo kParams_OneOptionalQuest[] =
{
	{"Quest", kParamType_Quest, 1}
};

ParamInfo kParams_OneString_OneOptionalForm[] =
{
	{"String", kParamType_String, 0},
	{"Form", kParamType_AnyForm, 1}
};

ParamInfo kParams_OneString_OneOptionalInt_OneOptionalForm[] =
{
	{"String", kParamType_String, 0},
	{"Integer", kParamType_Integer, 1},
	{"Form", kParamType_AnyForm, 1}
};

ParamInfo kParams_OneString_OneDouble_OneOptionalInt_OneOptionalForm[] =
{
	{"String", kParamType_String, 0},
	{"Double", kParamType_Double, 0},
	{"Integer", kParamType_Integer, 1},
	{"Form", kParamType_AnyForm, 1}
};

ParamInfo kParams_OneString_OneForm_OneOptionalInt_OneOptionalForm[] =
{
	{"String", kParamType_String, 0},
	{"Form", kParamType_AnyForm, 0},
	{"Integer", kParamType_Integer, 1},
	{"Form", kParamType_AnyForm, 1}
};

ParamInfo kParams_TwoStrings_OneOptionalInt_OneOptionalForm[] =
{
	{"String", kParamType_String, 0},
	{"String", kParamType_String, 0},
	{"Integer", kParamType_Integer, 1},
	{"Form", kParamType_AnyForm, 1}
};

ParamInfo kParams_OneInt_OneOptionalForm[] =
{
	{"Integer", kParamType_Integer, 0},
	{"Form", kParamType_AnyForm, 1}
};

ParamInfo kParams_OneString_OneInt_OneOptionalForm[] =
{
	{"String", kParamType_String, 0},
	{"Integer", kParamType_Integer, 0},
	{"Form", kParamType_AnyForm, 1}
};

ParamInfo kParams_OneString_OneDouble_OneOptionalForm[] =
{
	{"String", kParamType_String, 0},
	{"Double", kParamType_Double, 0},
	{"Form", kParamType_AnyForm, 1}
};

ParamInfo kParams_OneString_OneForm_OneOptionalForm[] =
{
	{"String", kParamType_String, 0},
	{"Form", kParamType_AnyForm, 0},
	{"Form", kParamType_AnyForm, 1}
};

ParamInfo kParams_TwoStrings_OneOptionalForm[] =
{
	{"String", kParamType_String, 0},
	{"String", kParamType_String, 0},
	{"Form", kParamType_AnyForm, 1}
};

ParamInfo kParams_FourInts[] =
{
	{"Integer", kParamType_Integer, 0},
	{"Integer", kParamType_Integer, 0},
	{"Integer", kParamType_Integer, 0},
	{"Integer", kParamType_Integer, 0}
};

ParamInfo kParams_OneForm_TwoInts_OneFloat[] =
{
	{"Form", kParamType_AnyForm, 0},
	{"Integer", kParamType_Integer, 0},
	{"Integer", kParamType_Integer, 0},
	{"Float", kParamType_Float, 0}
};

ParamInfo kParams_OneForm_TwoInts_OneOptionalForm[] =
{
	{"Form", kParamType_AnyForm, 0},
	{"Integer", kParamType_Integer, 0},
	{"Integer", kParamType_Integer, 0},
	{"Form", kParamType_AnyForm, 1}
};

ParamInfo kParams_OneForm_OneOptionalForm[] =
{
	{"Form", kParamType_AnyForm, 0},
	{"Form", kParamType_AnyForm, 1}
};

ParamInfo kParams_OneSound[] =
{
	{"Sound", kParamType_Sound, 0}
};

ParamInfo kParams_OneSound_OneString[] =
{
	{"Sound", kParamType_Sound, 0},
	{"String", kParamType_String, 0}
};

ParamInfo kParams_OneWorldspace[] =
{
	{"Worldspace", kParamType_WorldSpace, 0}
};

ParamInfo kParams_OneWorldspace_OneOptionalForm[] =
{
	{"Worldspace", kParamType_WorldSpace, 0},
	{"Form", kParamType_AnyForm, 1}
};

ParamInfo kParams_OneForm_OneOptionalActorBase[] =
{
	{"Form", kParamType_AnyForm, 0},
	{"Actor Base", kParamType_ActorBase, 1}
};

ParamInfo kParams_OneInt_OneFloat[] =
{
	{"Integer", kParamType_Integer, 0},
	{"Float", kParamType_Float, 0}
};

ParamInfo kParams_OneInt_OneFormatString[] =
{
	{"Integer", kParamType_Integer, 0},
	FORMAT_STRING_PARAMS
};

ParamInfo kParams_TwoInts_OneOptionalInt[] =
{
	{"Integer", kParamType_Integer, 0},
	{"Integer", kParamType_Integer, 0},
	{"Integer", kParamType_Integer, 1}
};

ParamInfo kParams_OneObjectRef_OneOptionalInt[] =
{
	{"Object Ref", kParamType_ObjectRef, 0},
	{"Integer", kParamType_Integer, 1}
};

ParamInfo kParams_OneList_TwoOptionalInts[] =
{
	{"List", kParamType_FormList, 0},
	{"Integer", kParamType_Integer, 1},
	{"Integer", kParamType_Integer, 1}
};

ParamInfo kParams_OneFaction_OneOptionalInt[] =
{
	{"Faction", kParamType_Faction, 0},
	{"Integer", kParamType_Integer, 1}
};

ParamInfo kParams_OneQuest_OneInt[] =
{
	{"Quest", kParamType_Quest, 0},
	{"Integer", kParamType_Integer, 0}
};

ParamInfo kParams_OneWorldspace_OneInt[] =
{
	{"Worldspace", kParamType_WorldSpace, 0},
	{"Integer", kParamType_Integer, 0}
};

ParamInfo kParams_OneWorldspace_TwoInts[] =
{
	{"Worldspace", kParamType_WorldSpace, 0},
	{"Integer", kParamType_Integer, 0},
	{"Integer", kParamType_Integer, 0}
};

ParamInfo kParams_OneString_OneInt_OneOptionalActorBase[] =
{
	{"String", kParamType_String, 0},
	{"Integer", kParamType_Integer, 0},
	{"Actor Base", kParamType_ActorBase, 1}
};

ParamInfo kParams_OneString_OneOptionalActorBase[] =
{
	{"String", kParamType_String, 0},
	{"Actor Base", kParamType_ActorBase, 1}
};

ParamInfo kParams_OneObjectRef[] =
{
	{"Object Ref", kParamType_ObjectRef, 0}
};

ParamInfo kParams_OneInt_ThreeFloats[] =
{
	{"Integer", kParamType_Integer, 0},
	{"Float", kParamType_Float, 0},
	{"Float", kParamType_Float, 0},
	{"Float", kParamType_Float, 0}
};

ParamInfo kParams_OneMagicItem_OneEffect_TwoInts_ThreeOptionalInts[] =
{
	{"Magic Item", kParamType_MagicItem, 0},
	{"Magic Effect", kParamType_MagicEffect, 0},
	{"Integer", kParamType_Integer, 0},
	{"Integer", kParamType_Integer, 0},
	{"Integer", kParamType_Integer, 1},
	{"Integer", kParamType_Integer, 1},
	{"Integer", kParamType_Integer, 1}
};

ParamInfo kParams_OneItemOrList_TwoOptionalInts[] =
{
	{"Item/List", kParamType_InvObjOrFormList, 0},
	{"Integer", kParamType_Integer, 1},
	{"Integer", kParamType_Integer, 1}
};

ParamInfo kParams_TwoOptionalInts[] =
{
	{"Integer", kParamType_Integer, 1},
	{"Integer", kParamType_Integer, 1}
};

ParamInfo kParams_OneActorValue_OneFloat_OneOptionalActorBase[] =
{
	{"Actor Value", kParamType_ActorValue, 0},
	{"Float", kParamType_Float, 0},
	{"Actor Base", kParamType_ActorBase, 1}
};

ParamInfo kParams_OneOptionalCell[] =
{
	{"Cell", kParamType_Cell, 1}
};

ParamInfo kParams_OneFloat_OneFormatString[] =
{
	{"Float", kParamType_Float, 0},
	FORMAT_STRING_PARAMS
};

ParamInfo kParams_OneForm_OneFormatString[] =
{
	{"Form", kParamType_AnyForm, 0},
	FORMAT_STRING_PARAMS
};

ParamInfo kParams_OneInt_OneString[] =
{
	{"Integer", kParamType_Integer, 0},
	{"String", kParamType_String, 0}
};

ParamInfo kParams_OneForm_TwoFloats_OneFormatString[] =
{
	{"Form", kParamType_AnyForm, 0},
	{"Float", kParamType_Float, 0},
	{"Float", kParamType_Float, 0},
	FORMAT_STRING_PARAMS
};

ParamInfo kParams_OneActor_OneTopic_TwoOptionalInts[] =
{
	{"Actor", kParamType_Actor, 0},
	{"Topic", kParamType_Topic, 0},
	{"Integer", kParamType_Integer, 1},
	{"Integer", kParamType_Integer, 1}
};

ParamInfo kParams_OneTopic[] =
{
	{"Topic", kParamType_Topic, 0}
};

ParamInfo kParams_OneForm_ThreeFloats[] =
{
	{"Form", kParamType_AnyForm, 0},
	{"Float", kParamType_Float, 0},
	{"Float", kParamType_Float, 0},
	{"Float", kParamType_Float, 0}
};

ParamInfo kParams_FourOptionalInts_OneOptionalList[] =
{
	{"Integer", kParamType_Integer, 1},
	{"Integer", kParamType_Integer, 1},
	{"Integer", kParamType_Integer, 1},
	{"Integer", kParamType_Integer, 1},
	{"List", kParamType_FormList, 1}
};

ParamInfo kParams_OneInt_OneOptionalActor[] =
{
	{"Integer", kParamType_Integer, 0},
	{"Actor", kParamType_Actor, 1}
};

ParamInfo kParams_OneAIPackage[] =
{
	{"Package", kParamType_AIPackage, 0}
};

ParamInfo kParams_OneObjectID_OneActorValue[] =
{
	{"Object ID", kParamType_ObjectID, 0},
	{"Actor Value", kParamType_ActorValue, 0}
};

ParamInfo kParams_OneMessage[] =
{
	{"Message", kParamType_Message, 0}
};

ParamInfo kParams_OneMessage_OneInt[2] =
{
	{"Message", kParamType_Message, 0},
	{"Integer", kParamType_Integer, 0},
};

ParamInfo kParams_OneSound_OneOptionalObjectRef[] =
{
	{"Sound", kParamType_Sound, 0},
	{"Object Ref", kParamType_ObjectRef, 1}
};

ParamInfo kParams_OneObjectRef_TwoStrings_OneOtionalInt[] =
{
	{"Object Ref", kParamType_ObjectRef, 0},
	{"String", kParamType_String, 0},
	{"String", kParamType_String, 0},
	{"Integer", kParamType_Integer, 1}
};

ParamInfo kParams_OneString_OneInt[] =
{
	{"String", kParamType_String, 0},
	{"Integer", kParamType_Integer, 0}
};

ParamInfo kParams_OneString_OneInt_OneFloat[] =
{
	{"String", kParamType_String, 0},
	{"Integer", kParamType_Integer, 0},
	{"Float", kParamType_Float, 0}
};

ParamInfo kParams_OneForm_OneInt_OneForm_OneInt[] =
{
	{"Form", kParamType_AnyForm, 0},
	{"Integer", kParamType_Integer, 0},
	{"Form", kParamType_AnyForm, 0},
	{"Integer", kParamType_Integer, 0}
};

ParamInfo kParams_OneForm_OneInt_OneForm_OneAnimGroup[] =
{
	{"Form", kParamType_AnyForm, 0},
	{"Integer", kParamType_Integer, 0},
	{"Form", kParamType_AnyForm, 0},
	{"Anim Group", kParamType_AnimationGroup, 0}
};

ParamInfo kParams_OneForm_OneInt_OneForm[] =
{
	{"Form", kParamType_AnyForm, 0},
	{"Integer", kParamType_Integer, 0},
	{"Form", kParamType_AnyForm, 0}
};

ParamInfo kParams_OneActorValue_OneOptionalActorBase[] =
{
	{"Actor Value", kParamType_ActorValue, 0},
	{"Actor Base", kParamType_ActorBase, 1}
};

ParamInfo kParams_ThreeFloats[] =
{
	{"Float", kParamType_Float, 0},
	{"Float", kParamType_Float, 0},
	{"Float", kParamType_Float, 0}
};

ParamInfo kParams_OneOptionalAxis[] =
{
	{"Axis", kParamType_Axis, 1}
};

ParamInfo kParams_OnePerk_OneInt[] =
{
	{"Perk", kParamType_Perk, 0},
	{"Integer", kParamType_Integer, 0}
};

ParamInfo kParams_OnePerk_TwoInts[] =
{
	{"Perk", kParamType_Perk, 0},
	{"Integer", kParamType_Integer, 0},
	{"Integer", kParamType_Integer, 0}
};

ParamInfo kParams_OneList_OneInt_TwoOptionalInts[] =
{
	{"List", kParamType_FormList, 0},
	{"Integer", kParamType_Integer, 0},
	{"Integer", kParamType_Integer, 1},
	{"Integer", kParamType_Integer, 1}
};

ParamInfo kParams_OneString_OneOptionalInt[] =
{
	{"String", kParamType_String, 0},
	{"Integer", kParamType_Integer, 1}
};

ParamInfo kParams_ThreeForms[] =
{
	{"Form", kParamType_AnyForm, 0},
	{"Form", kParamType_AnyForm, 0},
	{"Form", kParamType_AnyForm, 0}
};

ParamInfo kParams_TwoForms_TwoInts_OneFloat[] =
{
	{"Form", kParamType_AnyForm, 0},
	{"Form", kParamType_AnyForm, 0},
	{"Integer", kParamType_Integer, 0},
	{"Integer", kParamType_Integer, 0},
	{"Float", kParamType_Float, 0}
};

ParamInfo kParams_OneDouble_OneOptionalInt[] =
{
	{"Double", kParamType_Double, 0},
	{"Integer", kParamType_Integer, 1}
};

ParamInfo kParams_OneForm_OneInt_OneFloat_OneOptionalForm[] =
{
	{"Form", kParamType_AnyForm, 0},
	{"Integer", kParamType_Integer, 0},
	{"Float", kParamType_Float, 0},
	{"Form", kParamType_AnyForm, 1}
};

ParamInfo kParams_TwoForms_OneOptionalForm[] =
{
	{"Form", kParamType_AnyForm, 0},
	{"Form", kParamType_AnyForm, 0},
	{"Form", kParamType_AnyForm, 1}
};

ParamInfo kParams_OneInt_TwoOptionalInts[] =
{
	{"Integer", kParamType_Integer, 0},
	{"Integer", kParamType_Integer, 1},
	{"Integer", kParamType_Integer, 1}
};

ParamInfo kParams_OneObjectID_OneOptionalFloat_ThreeOptionalInts[] =
{
	{"Object ID", kParamType_ObjectID, 0},
	{"Float", kParamType_Float, 1},
	{"Integer", kParamType_Integer, 1},
	{"Integer", kParamType_Integer, 1},
	{"Integer", kParamType_Integer, 1}
};

ParamInfo kParams_OneFloat_OneOptionalForm[] =
{
	{"Float", kParamType_Float, 0},
	{"Form", kParamType_AnyForm, 1}
};

ParamInfo kParams_OneForm_OneInt_ThreeFloat[] =
{
	{"Form", kParamType_AnyForm, 0},
	{"Integer", kParamType_Integer, 0},
	{"Float", kParamType_Float, 0},
	{"Float", kParamType_Float, 0},
	{"Float", kParamType_Float, 0}
};

ParamInfo kParams_OneInt_TwoOptionalInts_OneOptionalCell[] =
{
	{"Integer", kParamType_Integer, 0},
	{"Integer", kParamType_Integer, 1},
	{"Integer", kParamType_Integer, 1},
	{"Cell", kParamType_Cell, 1}
};

ParamInfo kParams_OneInt_OneForm_OneOptionalFloat_OneOptionalInt[] =
{
	{"Integer", kParamType_Integer, 0},
	{"Form", kParamType_AnyForm, 0},
	{"Float", kParamType_Float, 1},
	{"Integer", kParamType_Integer, 1}
};

ParamInfo kParams_OneForm_OneOptionalInt[] =
{
	{"Form", kParamType_AnyForm, 0},
	{"Integer", kParamType_Integer, 1}
};

ParamInfo kParams_OneForm_OneInt_OneOptionalInt[] =
{
	{"Form", kParamType_AnyForm, 0},
	{"Integer", kParamType_Integer, 0},
	{"Integer", kParamType_Integer, 1}
};

ParamInfo kParams_TwoForms_OneInt[] =
{
	{"Form", kParamType_AnyForm, 0},
	{"Form", kParamType_AnyForm, 0},
	{"Integer", kParamType_Integer, 0}
};

ParamInfo kParams_FormCondition[] =
{
	{"Form", kParamType_AnyForm, 0},
	{"Integer", kParamType_Integer, 0},
	{"Form", kParamType_AnyForm, 0},
	{"Integer", kParamType_Integer, 0},
	{"Float", kParamType_Float, 0}
};

ParamInfo kParams_OneString_OneOptionalString[] =
{
	{"String", kParamType_String, 0},
	{"String", kParamType_String, 1}
};

ParamInfo kParams_OneString_OneDouble_OneOptionalString[] =
{
	{"String", kParamType_String, 0},
	{"Double", kParamType_Double, 0},
	{"String", kParamType_String, 1}
};

ParamInfo kParams_TwoString_OneOptionalString[] =
{
	{"String", kParamType_String, 0},
	{"String", kParamType_String, 0},
	{"String", kParamType_String, 1}
};

ParamInfo kParams_OneString_OneInt_OneOptionalString[] =
{
	{"String", kParamType_String, 0},
	{"Integer", kParamType_Integer, 0},
	{"String", kParamType_String, 1}
};

ParamInfo kParams_OneOptionalString[] =
{
	{"String", kParamType_String, 1}
};

ParamInfo kParams_OneInt_OneOptionalObjectRef[] =
{
	{"Integer", kParamType_Integer, 0},
	{"Object Ref", kParamType_ObjectRef, 1}
};

ParamInfo kParams_OneActorBase[] =
{
	{"Actor Base", kParamType_ActorBase, 0}
};

ParamInfo kParams_OneFloat_OneOptionalCell[] =
{
	{"Float", kParamType_Float, 0},
	{"Cell", kParamType_Cell, 1}
};

ParamInfo kParams_OneActorBase_OneInt[] =
{
	{"Actor Base", kParamType_ActorBase, 0},
	{"Integer", kParamType_Integer, 0}
};

ParamInfo kParams_OneActorBase_TwoInts[] =
{
	{"Actor Base", kParamType_ActorBase, 0},
	{"Integer", kParamType_Integer, 0},
	{"Integer", kParamType_Integer, 0}
};

ParamInfo kParams_OneCell[] =
{
	{"Cell", kParamType_Cell, 0}
};

ParamInfo kParams_OneCell_OneOptionalForm[] =
{
	{"Cell", kParamType_Cell, 0},
	{"Form", kParamType_AnyForm, 1}
};

ParamInfo kParams_OneCell_OneInt[] =
{
	{"Cell", kParamType_Cell, 0},
	{"Integer", kParamType_Integer, 0}
};

ParamInfo kParams_OneCell_TwoInts[] =
{
	{"Cell", kParamType_Cell, 0},
	{"Integer", kParamType_Integer, 0},
	{"Integer", kParamType_Integer, 0}
};

ParamInfo kParams_TwoInts_OneOptionalActorBase[] =
{
	{"Integer", kParamType_Integer, 0},
	{"Integer", kParamType_Integer, 0},
	{"Actor Base", kParamType_ActorBase, 1}
};

ParamInfo kParams_OneFaction_OneInt_OneOptionalActorBase[] =
{
	{"Faction", kParamType_Faction, 0},
	{"Integer", kParamType_Integer, 0},
	{"Actor Base", kParamType_ActorBase, 1}
};

ParamInfo kParams_OneImageSpace_OneInt[] =
{
	{"Image Space", kParamType_ImageSpace, 0},
	{"Integer", kParamType_Integer, 0}
};

ParamInfo kParams_OneImageSpace_OneInt_OneFloat[] =
{
	{"Image Space", kParamType_ImageSpace, 0},
	{"Integer", kParamType_Integer, 0},
	{"Float", kParamType_Float, 0}
};

ParamInfo kParams_OneIMOD_OneInt[] =
{
	{"IMOD", kParamType_ImageSpaceModifier, 0},
	{"Integer", kParamType_Integer, 0}
};

ParamInfo kParams_OneIMOD_OneInt_OneFloat[] =
{
	{"IMOD", kParamType_ImageSpaceModifier, 0},
	{"Integer", kParamType_Integer, 0},
	{"Float", kParamType_Float, 0}
};

ParamInfo kParams_OneForm_OneInt_TwoOptionalInts[] =
{
	{"Form", kParamType_AnyForm, 0},
	{"Integer", kParamType_Integer, 0},
	{"Integer", kParamType_Integer, 1},
	{"Integer", kParamType_Integer, 1}
};

ParamInfo kParams_TwoStrings_OneFloat[] =
{
	{"String", kParamType_String, 0},
	{"String", kParamType_String, 0},
	{"Float", kParamType_Float, 0}
};

ParamInfo kParams_OneGlobal_OneOptionalForm[] =
{
	{"Global", kParamType_Global, 0},
	{"Form", kParamType_AnyForm, 1}
};

ParamInfo kParams_OneCell_OneString[] =
{
	{"Cell", kParamType_Cell, 0},
	{"String", kParamType_String, 0}
};

ParamInfo kParams_TwoOptionalInts_OneOptionalContainer[] =
{
	{"Integer", kParamType_Integer, 1},
	{"Integer", kParamType_Integer, 1},
	{"Container", kParamType_Container, 1}
};

ParamInfo kParams_OneForm_OneString_OneInt[] =
{
	{"Form", kParamType_AnyForm, 0},
	{"String", kParamType_String, 0},
	{"Integer", kParamType_Integer, 0}
};

ParamInfo kParams_OneMagicItem[] =
{
	{"Magic Item", kParamType_MagicItem, 0}
};

ParamInfo kParams_FourOptionalFloats[] =
{
	{"Float", kParamType_Float, 1},
	{"Float", kParamType_Float, 1},
	{"Float", kParamType_Float, 1},
	{"Float", kParamType_Float, 1}
};

ParamInfo kParams_OneInt_OneFloat_TwoOptionalForms[] =
{
	{"Integer", kParamType_Integer, 0},
	{"Float", kParamType_Float, 0},
	{"Form", kParamType_AnyForm, 1},
	{"Form", kParamType_AnyForm, 1}
};

ParamInfo kParams_OneForm_OneInt_OneQuest_OneInt_OneOptionalInt[] =
{
	{"Form", kParamType_AnyForm, 0},
	{"Integer", kParamType_Integer, 0},
	{"Quest", kParamType_Quest, 0},
	{"Integer", kParamType_Integer, 0},
	{"Integer", kParamType_Integer, 1}
};

ParamInfo kParams_OneForm_OneInt_ThreeOptionalForms[] =
{
	{"Form", kParamType_AnyForm, 0},
	{"Integer", kParamType_Integer, 0},
	{"Form", kParamType_AnyForm, 1},
	{"Form", kParamType_AnyForm, 1},
	{"Form", kParamType_AnyForm, 1}
};

ParamInfo kParams_OneObjectID_OneInt_OneFormatString[] =
{
	{"Object ID", kParamType_ObjectID, 0},
	{"Integer", kParamType_Integer, 0},
	FORMAT_STRING_PARAMS
};

ParamInfo kParams_OneOptionalObjectRef_TwoOptionalInts[] =
{
	{"Object Ref", kParamType_ObjectRef, 1},
	{"Integer", kParamType_Integer, 1},
	{"Integer", kParamType_Integer, 1}
};

ParamInfo kParams_OneObjectRef_OneFloat[] =
{
	{"Object Ref", kParamType_ObjectRef, 0},
	{"Float", kParamType_Float, 0}
};

ParamInfo kParams_OneQuest_TwoInts_OneObjectRef[] =
{
	{"Quest", kParamType_Quest, 0},
	{"Integer", kParamType_Integer, 0},
	{"Integer", kParamType_Integer, 0},
	{"Object Ref", kParamType_ObjectRef, 0}
};

ParamInfo kParams_OneQuest_OneInt_OneObjectRef[] =
{
	{"Quest", kParamType_Quest, 0},
	{"Integer", kParamType_Integer, 0},
	{"Object Ref", kParamType_ObjectRef, 0}
};

ParamInfo kParams_OneQuest_OneInt_OneFormatString[] =
{
	{"Quest", kParamType_Quest, 0},
	{"Integer", kParamType_Integer, 0},
	FORMAT_STRING_PARAMS
};

ParamInfo kParams_OneObjectID_OneOptionalObjectID[] =
{
	{"Object ID", kParamType_ObjectID, 0},
	{"Object ID", kParamType_ObjectID, 1}
};

ParamInfo kParams_OneForm_OneOptionalString[] =
{
	{"Form", kParamType_AnyForm, 0},
	{"String", kParamType_String, 1}
};

ParamInfo kParams_OneSpellItem_OneOptionalActorBase[] =
{
	{"Spell Item", kParamType_SpellItem, 0},
	{"Actor Base", kParamType_ActorBase, 1}
};

ParamInfo kParams_OneMagicItem_OneOptionalActor[] =
{
	{"Magic Item", kParamType_MagicItem, 0},
	{"Actor", kParamType_Actor, 1}
};

ParamInfo kParams_ThreeOptionalInts[] =
{
	{"Integer", kParamType_Integer, 1},
	{"Integer", kParamType_Integer, 1},
	{"Integer", kParamType_Integer, 1}
};

ParamInfo kParams_OneAIPackage_ThreeInts[] =
{
	{"Package", kParamType_AIPackage, 0},
	{"Integer", kParamType_Integer, 0},
	{"Integer", kParamType_Integer, 0},
	{"Integer", kParamType_Integer, 0}
};

ParamInfo kParams_OneForm_OneInt_OneDouble[] =
{
	{"Form", kParamType_AnyForm, 0},
	{"Integer", kParamType_Integer, 0},
	{"Double", kParamType_Double, 0}
};

ParamInfo kParams_OneDouble[] =
{
	{"Double", kParamType_Double, 0}
};

ParamInfo kParams_FourFloats_OneOptionalObjectRef[] =
{
	{"Float", kParamType_Float, 0},
	{"Float", kParamType_Float, 0},
	{"Float", kParamType_Float, 0},
	{"Float", kParamType_Float, 0},
	{"Object Ref", kParamType_ObjectRef, 1}
};

ParamInfo kParams_OneActorBase_OneOptionalString[] =
{
	{"Actor Base", kParamType_ActorBase, 0},
	{"String", kParamType_String, 1}
};

ParamInfo kParams_OneActorBase_OneString[] =
{
	{"Actor Base", kParamType_ActorBase, 0},
	{"String", kParamType_String, 0}
};

ParamInfo kParams_OneActorBase_OneForm[] =
{
	{"Actor Base", kParamType_ActorBase, 0},
	{"Form", kParamType_AnyForm, 0}
};

ParamInfo kParams_OneString_ThreeFloats_OneOptionalInt[] =
{
	{"String", kParamType_String, 0},
	{"Float", kParamType_Float, 0},
	{"Float", kParamType_Float, 0},
	{"Float", kParamType_Float, 0},
	{"Integer", kParamType_Integer, 1}
};

ParamInfo kParams_OneString_ThreeFloats_TwoOptionalInts[] =
{
	{"String", kParamType_String, 0},
	{"Float", kParamType_Float, 0},
	{"Float", kParamType_Float, 0},
	{"Float", kParamType_Float, 0},
	{"Integer", kParamType_Integer, 1},
	{"Integer", kParamType_Integer, 1}
};

ParamInfo kParams_OneString_OneOptionalAxis[] =
{
	{"String", kParamType_String, 0},
	{"Axis", kParamType_Axis, 1}
};

ParamInfo kParams_OneString_OneAxis_OneFloat[] =
{
	{"String", kParamType_String, 0},
	{"Axis", kParamType_Axis, 0},
	{"Float", kParamType_Float, 0}
};

ParamInfo kParams_OneForm_OneInt_OneForm_ThreeFloats[] =
{
	{"Form", kParamType_AnyForm, 0},
	{"Integer", kParamType_Integer, 0},
	{"Form", kParamType_AnyForm, 0},
	{"Float", kParamType_Float, 0},
	{"Float", kParamType_Float, 0},
	{"Float", kParamType_Float, 0}
};

ParamInfo kParams_OneActorValue_OneOptionalInt[] =
{
	{"Actor Value", kParamType_ActorValue, 0},
	{"Integer", kParamType_Integer, 1}
};

ParamInfo kParams_ThreeGlobals_OneOptionalFloat_OneOptionalInt_OneOptionalString[] =
{
	{"Global", kParamType_Global, 0},
	{"Global", kParamType_Global, 0},
	{"Global", kParamType_Global, 0},
	{"Float", kParamType_Float, 1},
	{"Integer", kParamType_Integer, 1},
	{"String", kParamType_String, 1}
};

ParamInfo kParams_OneObjectRef_OneString_ThreeOptionalFloats[] =
{
	{"Object Ref", kParamType_ObjectRef, 0},
	{"String", kParamType_String, 0},
	{"Float", kParamType_Float, 1},
	{"Float", kParamType_Float, 1},
	{"Float", kParamType_Float, 1}
};

ParamInfo kParams_OneObjectID_OneOptionalActorBase[] =
{
	{"Object ID", kParamType_ObjectID, 0},
	{"Actor Base", kParamType_ActorBase, 1}
};

ParamInfo kParams_TwoStrings_OneInt[] =
{
	{"String", kParamType_String, 0},
	{"String", kParamType_String, 0},
	{"Integer", kParamType_Integer, 0}
};

ParamInfo kParams_OneString_OneForm_ThreeOptionalFloats[] =
{
	{"String", kParamType_String, 0},
	{"Form", kParamType_AnyForm, 0},
	{"Float", kParamType_Float, 1},
	{"Float", kParamType_Float, 1},
	{"Float", kParamType_Float, 1}
};

ParamInfo kParams_OneEffect[] =
{
	{"Magic Effect", kParamType_MagicEffect, 0}
};

ParamInfo kParams_OneQuest_TwoInts[] =
{
	{"Quest", kParamType_Quest, 0},
	{"Integer", kParamType_Integer, 0},
	{"Integer", kParamType_Integer, 0}
};

ParamInfo kParams_OneFloat_ThreeOptionalFloats_OneOptionalInt[] =
{
	{"Float", kParamType_Float, 0},
	{"Float", kParamType_Float, 1},
	{"Float", kParamType_Float, 1},
	{"Float", kParamType_Float, 1},
	{"Integer", kParamType_Integer, 1}
};

ParamInfo kParams_OneString_OneOptionalString_OneOptionalInt[] =
{
	{"String", kParamType_String, 0},
	{"String", kParamType_String, 1},
	{"Integer", kParamType_Integer, 1}
};

ParamInfo kParams_OneObjectRef_OneFloat_OneOptionalInt[] =
{
	{"Object Ref", kParamType_ObjectRef, 0},
	{"Float", kParamType_Float, 0},
	{"Integer", kParamType_Integer, 1}
};

ParamInfo kParams_TwoDoubles[] =
{
	{"Double", kParamType_Double, 0},
	{"Double", kParamType_Double, 0}
};

ParamInfo kParams_OneObjectID_OneInt_OneFloat[] =
{
	{"Object ID", kParamType_ObjectID, 0},
	{"Integer", kParamType_Integer, 0},
	{"Float", kParamType_Float, 0}
};

ParamInfo kParams_TwoFloats_ThreeOptionalInts[] =
{
	{"Float", kParamType_Float, 0},
	{"Float", kParamType_Float, 0},
	{"Integer", kParamType_Integer, 1},
	{"Integer", kParamType_Integer, 1},
	{"Integer", kParamType_Integer, 1}
};

ParamInfo kParams_OneNote[] =
{
	{"Note", kParamType_Note, 0}
};

ParamInfo kParams_OneForm_OneOptionalSpellItem[] =
{
	{"Form", kParamType_AnyForm, 0},
	{"Spell Item", kParamType_SpellItem, 1}
};

ParamInfo kParams_OneString_TwoFloats[] =
{
	{"String", kParamType_String, 0},
	{"Float", kParamType_Float, 0},
	{"Float", kParamType_Float, 0}
};

ParamInfo kParams_OneActorBase_OneOptionalActorBase[] =
{
	{"Actor Base", kParamType_ActorBase, 0},
	{"Actor Base", kParamType_ActorBase, 1}
};

ParamInfo kParams_TwoFloats_OneOptionalObjectRef_OneOptionalInt[] =
{
	{"Float", kParamType_Float, 0},
	{"Float", kParamType_Float, 0},
	{"Object Ref", kParamType_ObjectRef, 1},
	{"Integer", kParamType_Integer, 1}
};

ParamInfo kParams_OneString_ThreeOptionalFloats_OneOptionalInt[] =
{
	{"String", kParamType_String, 0},
	{"Float", kParamType_Float, 1},
	{"Float", kParamType_Float, 1},
	{"Float", kParamType_Float, 1},
	{"Integer", kParamType_Integer, 1}
};

ParamInfo kParams_OneString_ThreeFloats[] =
{
	{"String", kParamType_String, 0},
	{"Float", kParamType_Float, 0},
	{"Float", kParamType_Float, 0},
	{"Float", kParamType_Float, 0}
};

ParamInfo kParams_OneForm_OneInt_OneFormatString[] =
{
	{"Form", kParamType_AnyForm, 0},
	{"Integer", kParamType_Integer, 0},
	FORMAT_STRING_PARAMS
};

ParamInfo kParams_OneOptionalObjectRef_OneOptionalInt_OneOptionalFloat_OneOptionalString[] =
{
	{"Object Ref", kParamType_ObjectRef, 1},
	{"Integer", kParamType_Integer, 1},
	{"Float", kParamType_Float, 1},
	{"String", kParamType_String, 1}
};

ParamInfo kParams_OneScriptVar[] =
{
	{"Variable", kParamType_ScriptVariable, 0}
};

ParamInfo kParams_OneOptionalInt_OneOptionalString[] =
{
	{"Integer", kParamType_Integer, 1},
	{"String", kParamType_String, 1}
};

ParamInfo kParams_OneObjectID_OneOptionalString[] =
{
	{"Object ID", kParamType_ObjectID, 0},
	{"String", kParamType_String, 1}
};

ParamInfo kParams_OneString_OneDouble[] =
{
	{"String", kParamType_String, 0},
	{"Double", kParamType_Double, 0}
};

ParamInfo kParams_TwoScriptVars[] =
{
	{"Variable", kParamType_ScriptVariable, 0},
	{"Variable", kParamType_ScriptVariable, 0}
};

ParamInfo kParams_OneString_OneFloat_OneOptionalInt[] =
{
	{"String", kParamType_String, 0},
	{"Float", kParamType_Float, 0},
	{"Integer", kParamType_Integer, 1}
};

ParamInfo kParams_OneString_OneInt_OneOptionalInt[] =
{
	{"String", kParamType_String, 0},
	{"Integer", kParamType_Integer, 0},
	{"Integer", kParamType_Integer, 1}
};

ParamInfo kParams_OneString_TwoInts_OneOptionalInt[] =
{
	{"String", kParamType_String, 0},
	{"Integer", kParamType_Integer, 0},
	{"Integer", kParamType_Integer, 0},
	{"Integer", kParamType_Integer, 1}
};

ParamInfo kParams_OneOptionalFloat_OneOptionalInt[] =
{
	{"Float", kParamType_Float, 1},
	{"Integer", kParamType_Integer, 1}
};

ParamInfo kParams_OneOptionalString_OneOptionalInt[] =
{
	{"String", kParamType_String, 1},
	{"Integer", kParamType_Integer, 1}
};

ParamInfo kParams_ThreeScriptVars[] =
{
	{"Variable", kParamType_ScriptVariable, 0},
	{"Variable", kParamType_ScriptVariable, 0},
	{"Variable", kParamType_ScriptVariable, 0}
};

ParamInfo kParams_OneString_TwoInts_OneFloat[] =
{
	{"String", kParamType_String, 0},
	{"Integer", kParamType_Integer, 0},
	{"Integer", kParamType_Integer, 0},
	{"Float", kParamType_Float, 0}
};

ParamInfo kParams_TwoStrings_OneDouble_OneOptionalInt[] =
{
	{"String", kParamType_String, 0},
	{"String", kParamType_String, 0},
	{"Double", kParamType_Double, 0},
	{"Integer", kParamType_Integer, 1}
};

ParamInfo kParams_TwoStrings_OneOptionalInt[] =
{
	{"String", kParamType_String, 0},
	{"String", kParamType_String, 0},
	{"Integer", kParamType_Integer, 1}
};

ParamInfo kParams_OnePerk[] =
{
	{"Perk", kParamType_Perk, 0}
};

ParamInfo kParams_OneChallenge[] =
{
	{"Challenge", kParamType_Challenge, 0}
};

ParamInfo kParams_OneChallenge_OneInt[] =
{
	{"Challenge", kParamType_Challenge, 0},
	{"Integer", kParamType_Integer, 0}
};

ParamInfo kParams_OneChallenge_OneForm[] =
{
	{"Challenge", kParamType_Challenge, 0},
	{"Form", kParamType_AnyForm, 0}
};

ParamInfo kParams_OneCasino[] =
{
	{"Casino", kParamType_Casino, 0}
};

ParamInfo kParams_OneCasino_OneInt[] =
{
	{"Casino", kParamType_Casino, 0},
	{"Integer", kParamType_Integer, 0}
};

ParamInfo kParams_OneCasino_OneFloat[] =
{
	{"Casino", kParamType_Casino, 0},
	{"Float", kParamType_Float, 0}
};

ParamInfo kParams_FiveFloats_ThreeScriptVars_OneOptionalInt[] =
{
	{"Float", kParamType_Float, 0},
	{"Float", kParamType_Float, 0},
	{"Float", kParamType_Float, 0},
	{"Float", kParamType_Float, 0},
	{"Float", kParamType_Float, 0},
	{"Variable", kParamType_ScriptVariable, 0},
	{"Variable", kParamType_ScriptVariable, 0},
	{"Variable", kParamType_ScriptVariable, 0},
	{"Integer", kParamType_Integer, 1}
};

ParamInfo kParams_ThreeFloats_OneOptionalInt[] =
{
	{"Float", kParamType_Float, 0},
	{"Float", kParamType_Float, 0},
	{"Float", kParamType_Float, 0},
	{"Integer", kParamType_Integer, 1}
};

ParamInfo kParams_ThreeFloats_ThreeScriptVars[] =
{
	{"Float", kParamType_Float, 0},
	{"Float", kParamType_Float, 0},
	{"Float", kParamType_Float, 0},
	{"Variable", kParamType_ScriptVariable, 0},
	{"Variable", kParamType_ScriptVariable, 0},
	{"Variable", kParamType_ScriptVariable, 0}
};