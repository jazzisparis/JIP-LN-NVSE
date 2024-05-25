#pragma once

DEFINE_COMMAND_PLUGIN(CCCOnLoad, 0, nullptr);
DEFINE_COMMAND_PLUGIN(CCCSetFloat, 0, kParams_OneInt_OneFloat);
DEFINE_COMMAND_PLUGIN(CCCSetString, 0, kParams_OneInt_OneFormatString);
DEFINE_COMMAND_PLUGIN(CCCSetTrait, 0, kParams_TwoInts_OneOptionalInt);
DEFINE_COMMAND_PLUGIN(CCCGetDistance, 1, kParams_OneObjectRef_OneOptionalInt);
DEFINE_COMMAND_PLUGIN(CCCInFaction, 1, kParams_OneFaction_OneOptionalInt);
DEFINE_COMMAND_PLUGIN(CCCSetNCCS, 1, kParams_OneOptionalInt);
DEFINE_CMD_COND_PLUGIN(GetEncumbranceRate, 1, nullptr);
DEFINE_COMMAND_PLUGIN(CCCLoadNCCS, 0, nullptr);
DEFINE_COMMAND_PLUGIN(CCCSavedForm, 0, kParams_OneInt);
DEFINE_COMMAND_PLUGIN(CCCLocationName, 1, nullptr);
DEFINE_COMMAND_PLUGIN(CCCGetReputation, 1, nullptr);
DEFINE_COMMAND_PLUGIN(CCCSayTo, 1, kParams_OneActor_OneTopic_TwoOptionalInts);
DEFINE_COMMAND_PLUGIN(CCCRunResultScripts, 1, kParams_OneTopic);
DEFINE_COMMAND_PLUGIN(CCCSetFollowState, 1, kParams_OneOptionalInt);
DEFINE_COMMAND_PLUGIN(RefToPosStr, 1, nullptr);
DEFINE_COMMAND_PLUGIN(MoveToPosStr, 1, kParams_OneString);
DEFINE_COMMAND_PLUGIN(LockEquipment, 1, kParams_OneInt);
DEFINE_COMMAND_PLUGIN(CCCSetEquipped, 0, nullptr);
DEFINE_COMMAND_PLUGIN(CCCSMS, 0, kParams_OneOptionalObjectRef);
DEFINE_COMMAND_PLUGIN(CCCTaskPackageFlags, 0, kParams_OneAIPackage_ThreeInts);

UInt8 s_CCCModIdx = 0;
TempObject<UnorderedMap<const char*, const char*, 0x80, false>> s_avatarPaths;
TempObject<Map<char*, const char*>> s_avatarCommon(0x30);
bool s_UILoaded = false;
TileValue **s_UIelements = nullptr;
UInt32 s_savedForms[10] = {};
TESFaction *s_taskFaction;
const char kLoadError[] = "ERROR: Failed to load JIP CC&C UI components.";

bool Cmd_CCCOnLoad_Execute(COMMAND_ARGS)
{
	if (s_CCCModIdx) return true;

	s_CCCModIdx = scriptObj->modIndex;
	UInt32 index = scriptObj->modIndex << 24;
	TESGlobal *globVar = (TESGlobal*)LookupFormByRefID(index | 0x5000);
	if (!globVar || (globVar->data < 260))
	{
		globVar = (TESGlobal*)LookupFormByRefID(index | 0x8F6D);
		if (globVar) globVar->data = 1;
		ShowMessageBox("Please be advised:\n\nJIP CC&C versions older than 2.60 will NOT work with JIP LN NVSE.\n\nYou MUST update JIP CC&C to the latest version.", MSGBOX_ARGS);
		return false;
	}

	char *buffer = GetStrArgBuffer(), *dataPtr, *delim, *pathStr;
	LineIterator lineIter("Data\\Config\\ccc_avatars.ini", buffer);
	index = 0;
	while (lineIter)
	{
		dataPtr = *lineIter;
		++lineIter;
		delim = GetNextToken(dataPtr, '=');
		pathStr = (char*)malloc(StrLen(delim) + 28);
		memcpy(StrCopy(CPY_RET_END(pathStr, "jazzisparis\\ccc\\avatar_", 23), delim), ".dds", 5);
		if (index < 129) s_avatarPaths()[dataPtr] = pathStr;
		else s_avatarCommon()[dataPtr] = pathStr;
		index++;
	}

	for (DirectoryIterator iter("Data\\textures\\jazzisparis\\ccc\\avatar_*.dds"); iter; ++iter)
	{
		if (!iter.IsFile()) continue;
		delim = const_cast<char*>(*iter);
		index = StrLen(delim);
		pathStr = (char*)malloc(index + 17);
		StrCopy(CPY_RET_END(pathStr, "jazzisparis\\ccc\\", 16), delim);
		delim[index - 4] = 0;
		s_avatarPaths()[delim + 7] = pathStr;
	}

	s_taskFaction = (TESFaction*)LookupFormByRefID((s_CCCModIdx << 24) + 0x2D501);

	index = g_dataHandler->GetModIndex("Companion Core.esm");
	if (index < 0xFF)
	{
		index <<= 24;
		s_savedForms[0] = index | 0x812;
		s_savedForms[1] = index | 0x810;
		s_savedForms[2] = index | 0x950D;
		s_savedForms[3] = index | 0x5B3B4;
		TESPackage *package = (TESPackage*)LookupFormByRefID(index | 0x80E);
		if (package && package->location) package->location->radius = 0;
	}

	index = g_dataHandler->GetModIndex("NosCo Companion System.esm");
	if (index < 0xFF)
	{
		index <<= 24;
		s_savedForms[4] = index | 0xAE0;
		s_savedForms[5] = index | 0xAE2;
		s_savedForms[6] = index | 0x86A7;
		s_savedForms[7] = index | 0xAE5;
		s_savedForms[8] = index | 0xADD;
	}

	index = g_dataHandler->GetModIndex("EZ_CompanionNVSE.esm");
	if (index < 0xFF) s_savedForms[9] = (index << 24) | 0xDC3;
	
	Tile *tile = g_HUDMainMenu->tile->GetComponentTile("JIPCCCHUDDisplay");
	if (!tile)
	{
		Console_Print(kLoadError);
		return true;
	}
	if (!s_UIelements) s_UIelements = (TileValue**)calloc(0x77, 4);
	UInt32 traitID06 = Tile::TraitNameToID("_Size"), traitID07 = Tile::TraitNameToID("_Anchor"), traitID08 = Tile::TraitNameToID("_Selected"),
		traitID09 = Tile::TraitNameToID("_HealthFill"), traitID10 = Tile::TraitNameToID("_Distance"), traitID11 = Tile::TraitNameToID("_Action"),
		traitID17 = Tile::TraitNameToID("_CmdType"), traitID20 = Tile::TraitNameToID("_Value");
	DList<Tile>::Node *node0 = tile->children.Tail(), *node1, *node2;
	s_UIelements[0] = tile->GetValueName("_AltStyle");
	s_UIelements[1] = tile->GetValueName("_SelectMode");
	s_UIelements[2] = tile->GetValueName("_Adjust");
	tile = node0->data;
	s_UIelements[3] = tile->GetValueName("_TotalAvatars");
	s_UIelements[4] = tile->GetValueName("_NumHired");
	s_UIelements[5] = tile->GetValueName("_Layout");
	s_UIelements[6] = tile->GetValue(traitID06);
	s_UIelements[7] = tile->GetValue(traitID07);
	s_UIelements[8] = tile->GetValue(traitID08);
	s_UIelements[9] = tile->GetValue(kTileValue_visible);
	node1 = tile->children.Tail();
	for (index = 0; index < 10; index++)
	{
		tile = node1->data;
		node1 = node1->prev;
		s_UIelements[10 + index] = tile->GetValue(traitID09);
		s_UIelements[20 + index] = tile->GetValue(traitID10);
		s_UIelements[30 + index] = tile->GetValue(traitID11);
		s_UIelements[40 + index] = tile->GetNthChild(2)->data->GetValue(kTileValue_filename);
		s_UIelements[50 + index] = tile->GetNthChild(4)->data->GetValue(kTileValue_string);
	}
	node0 = node0->prev;
	tile = node0->data;
	s_UIelements[60] = tile->GetValue(traitID06);
	s_UIelements[61] = tile->GetValue(traitID08);
	s_UIelements[62] = tile->GetValue(traitID07);
	s_UIelements[63] = tile->GetValueName("_IsWaiting");
	s_UIelements[64] = tile->GetValueName("_IsFollowingLong");
	s_UIelements[65] = tile->GetValueName("_IsUsingMelee");
	s_UIelements[66] = tile->GetValueName("_IsAggressive");
	s_UIelements[67] = tile->GetValueName("_LabelsON");
	s_UIelements[68] = tile->GetValue(kTileValue_x);
	node1 = tile->GetNthChild(3);
	for (index = 69; index < 74; index++)
	{
		s_UIelements[index] = node1->data->GetValue(traitID17);
		node1 = node1->prev;
	}
	s_UIelements[74] = node1->data->GetValue(kTileValue_string);
	node0 = node0->prev;
	tile = node0->data;
	s_UIelements[75] = tile->GetValueName("_Menu");
	s_UIelements[76] = tile->GetValue(traitID08);
	s_UIelements[77] = tile->GetValueName("_HasHome");
	node1 = tile->GetNthChild(3);
	node2 = node1->data->children.Tail();
	for (index = 78; index < 81; index++)
	{
		tile = node2->data;
		node2 = node2->prev;
		s_UIelements[index] = tile->GetValue(traitID20);
	}
	node2 = node2->prev->prev->prev;
	s_UIelements[81] = node2->data->GetValue(kTileValue_string);
	node1 = node1->prev->prev;
	node2 = node1->data->children.Tail();
	s_UIelements[82] = node2->data->GetValue(kTileValue_string);
	node2 = node2->prev;
	s_UIelements[83] = node2->data->GetValue(kTileValue_string);
	node2 = node2->prev;
	for (index = 84; index < 110; index++)
	{
		s_UIelements[index] = node2->data->GetValue(traitID20);
		node2 = node2->prev;
	}
	s_UIelements[110] = node2->data->children.Tail()->data->GetValue(kTileValue_string);
	node0 = node0->prev;
	tile = node0->data;
	s_UIelements[111] = tile->GetValueName("_Targeting");
	s_UIelements[112] = tile->GetValue(kTileValue_string);
	node0 = node0->prev;
	s_UIelements[113] = node0->data->children.Tail()->data->GetValue(kTileValue_string);
	tile = tile->parent->parent;

	char compPath[] = "SneakMeter/y\0QuestWrap/QuestReminder/x\0XPWrap/x\0EH_Wrap/EnemyHealth/y\0QuestReminder/x\0XPMeter/x\0Info/y\0EnemyHealth/y\0InfoWrap/Info/y";
	s_UIelements[114] = tile->GetComponentValue(compPath);
	if (tile->GetValueName("_DUIF3")->num)
	{
		s_UIelements[115] = tile->GetComponentValue(compPath + 13);
		s_UIelements[116] = tile->GetComponentValue(compPath + 39);
		s_UIelements[118] = tile->GetComponentValue(compPath + 48);
	}
	else
	{
		s_UIelements[115] = tile->GetComponentValue(compPath + 70);
		s_UIelements[116] = tile->GetComponentValue(compPath + 86);
		s_UIelements[117] = tile->GetComponentValue(compPath + 96);
		s_UIelements[118] = tile->GetComponentValue(compPath + 103);
	}
	if (!s_UIelements[117]) s_UIelements[117] = tile->GetComponentValue(compPath + 117);

	for (index = 0; index < 119; index++)
	{
		if (!s_UIelements[index])
		{
			Console_Print(kLoadError);
			return true;
		}
	}
	s_UILoaded = true;

	return true;
}

bool Cmd_CCCSetFloat_Execute(COMMAND_ARGS)
{
	UInt32 trait;
	float value;
	if (s_UILoaded && ExtractArgsEx(EXTRACT_ARGS_EX, &trait, &value))
		s_UIelements[trait]->SetFloat(value);
	return true;
}

bool Cmd_CCCSetString_Execute(COMMAND_ARGS)
{
	UInt32 trait;
	char *buffer = GetStrArgBuffer();
	if (s_UILoaded && ExtractFormatStringArgs(1, buffer, EXTRACT_ARGS_EX, kCommandInfo_CCCSetString.numParams, &trait))
		s_UIelements[trait]->SetString(buffer);
	return true;
}

const char kAvatarMale[] = "jazzisparis\\ccc\\avatar_male.dds", kAvatarFemale[] = "jazzisparis\\ccc\\avatar_female.dds";
TempObject<UnorderedMap<UInt32, const char*, 0x20, false>> s_pathForID;

bool Cmd_CCCSetTrait_Execute(COMMAND_ARGS)
{
	SInt32 trait, child, value = 0;
	if (s_UILoaded && ExtractArgsEx(EXTRACT_ARGS_EX, &trait, &child, &value))
		if (trait == 40)
		{
			const char *pathStr = kAvatarMale;
			if (thisObj && IS_ACTOR(thisObj))
			{
				TESActorBase *actorBase = (TESActorBase*)thisObj->baseForm;
				const char *fullName = actorBase->fullName.name.m_data, **findID;
				if (!fullName || !*fullName) return true;
				if (s_pathForID->InsertKey(actorBase->refID, &findID))
				{
					const char *findName = s_avatarPaths->Get(fullName);
					if (!findName)
					{
						if IS_ID(actorBase, TESCreature)
						{
							for (auto iter = s_avatarCommon->Begin(); iter; ++iter)
							{
								if (strstr(fullName, iter.Key()))
								{
									pathStr = *iter;
									break;
								}
							}
						}
						else if (actorBase->baseData.flags & 1) pathStr = kAvatarFemale;
					}
					else pathStr = findName;
					*findID = pathStr;
				}
				else pathStr = *findID;
				s_UIelements[50 + child]->SetString(fullName);
			}
			s_UIelements[40 + child]->SetString(pathStr);
		}
		else if (!trait)
		{
			if (NOT_ACTOR(thisObj)) return true;
			Actor *actor = (Actor*)thisObj;
			if (actor->GetKnockedState() != 3)
			{
				s_UIelements[10 + child]->SetFloat(64.0F * actor->avOwner.GetActorValue(kAVCode_Health) / actor->avOwner.GetBaseActorValue(kAVCode_Health));
				s_UIelements[20 + child]->SetBool(g_thePlayer->GetDistance(actor) > 8192.0F);
			}
			else s_UIelements[10 + child]->SetFloat(-1.0F);
			value = actor->extraDataList.GetExtraFactionRank(s_taskFaction);
			if (value < 7)
			{
				PackageInfo *packageInfo = actor->GetPackageInfo();
				if (!packageInfo) return true;
				switch (packageInfo->package->type)
				{
					case 1:
					case 2:
					case 6:
					case 7:
						if (packageInfo->targetRef != g_thePlayer) return true;
						value = 1;
						break;
					case 5:
					case 9:
					case 13:
					case 14:
						value = 2;
						break;
					case 18:
						value = 5 + packageInfo->package->GetFleeCombat();
						break;
					case 10:
					case 22:
						value = 6;
						break;
					case 12:
						value = 13;
						break;
					default:
						return true;
				}
			}
			s_UIelements[30 + child]->SetFloat(value);
		}
		else s_UIelements[trait + child]->SetFloat(value);
	return true;
}

bool Cmd_CCCGetDistance_Execute(COMMAND_ARGS)
{
	TESObjectREFR *objRef;
	UInt32 axis = 3;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &objRef, &axis))
	{
		if (axis == 4)
			*result = abs(thisObj->position.z - objRef->position.z);
		else *result = Point2Distance(thisObj->position, objRef->position);
	}
	return true;
}

bool Cmd_CCCInFaction_Execute(COMMAND_ARGS)
{
	TESFaction *faction;
	UInt32 inBase = 0;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &faction, &inBase) && IS_ACTOR(thisObj) && 
		((inBase && (((TESActorBase*)thisObj->baseForm)->baseData.GetFactionRank(faction) >= 0)) || 
		(thisObj->extraDataList.GetExtraFactionRank(faction) >= 0)))
		*result = 1;
	return true;
}

TempObject<Set<TESActorBase*>> s_NCCSActors;

void SetNCCS(TESActorBase *actorBase, bool doSet)
{
	if (actorBase->scriptable.script)
	{
		SetScriptDisabled(actorBase->scriptable.script, doSet);
		SetScriptEventDisabled(actorBase, 0, true, doSet);
	}
}

bool Cmd_CCCSetNCCS_Execute(COMMAND_ARGS)
{
	UInt32 doSet = 1;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &doSet) && IS_ACTOR(thisObj))
	{
		TESActorBase *actorBase = (TESActorBase*)thisObj->baseForm;
		if (doSet)
		{
			if (!s_NCCSActors->Insert(actorBase)) return true;
		}
		else if (!s_NCCSActors->Erase(actorBase)) return true;
		SetNCCS(actorBase, doSet != 0);
	}
	return true;
}

__declspec(noinline) double __fastcall GetEncumbranceRate(TESObjectREFR *thisObj)
{
	if (IS_ACTOR(thisObj))
		if (auto xChanges = GetExtraType(&thisObj->extraDataList, ExtraContainerChanges); xChanges && xChanges->data)
		{
			double maxWeight = ThisCall<double>(0x8A0C20, thisObj);
			if (maxWeight > 0)
				return 100 * xChanges->data->GetInventoryWeight() / maxWeight;
		}
	return 0;
}

bool Cmd_GetEncumbranceRate_Execute(COMMAND_ARGS)
{
	*result = GetEncumbranceRate(thisObj);
	return true;
}

bool Cmd_GetEncumbranceRate_Eval(COMMAND_ARGS_EVAL)
{
	*result = GetEncumbranceRate(thisObj);
	return true;
}

bool Cmd_CCCLoadNCCS_Execute(COMMAND_ARGS)
{
	if (!s_NCCSActors->Empty())
	{
		for (auto nccs = s_NCCSActors->Begin(); nccs; ++nccs)
			SetNCCS(*nccs, false);
		s_NCCSActors->Clear();
	}
	return true;
}

bool Cmd_CCCSavedForm_Execute(COMMAND_ARGS)
{
	UInt32 idx;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &idx))
		REFR_RES = s_savedForms[idx];
	return true;
}

typedef UnorderedMap<UInt32, TESObjectREFR*, 0x80, false> MapMarkersGrid;
TempObject<Map<TESWorldSpace*, MapMarkersGrid>> s_worldspaceMap;

bool Cmd_CCCLocationName_Execute(COMMAND_ARGS)
{
	TESObjectCELL *cell = thisObj->GetParentCell();
	if (!cell)
	{
		AssignString(PASS_COMMAND_ARGS, "at an unknown location");
		return true;
	}
	char locName[0x80];
	TESWorldSpace *currentWspc = cell->worldSpace;
	if (!currentWspc)
	{
		COPY_BYTES(CPY_RET_END(locName, "in ", 3), cell->fullName.name.m_data, cell->fullName.name.m_dataLen + 1);
		AssignString(PASS_COMMAND_ARGS, locName);
		return true;
	}
	TESObjectREFR *mkRefr;
	MapMarkerData *markerData;
	Coordinate coord;
	MapMarkersGrid *markersGrid;
	if (s_worldspaceMap->Insert(currentWspc, &markersGrid))
	{
		if (currentWspc->cell && (currentWspc->worldMap.ddsPath.m_dataLen || currentWspc->parent))
		{
			auto objIter = currentWspc->cell->objectList.Head();
			do
			{
				if (!(mkRefr = objIter->data) || !(markerData = mkRefr->GetMapMarkerData()) || !markerData->fullName.name.m_dataLen)
					continue;
				coord = mkRefr->position.PS2();
				(*markersGrid)[coord] = mkRefr;
			}
			while (objIter = objIter->next);
		}
	}
	if (!markersGrid->Empty())
	{
		mkRefr = nullptr;
		Coordinate gridXY(thisObj->position.PS2());
		float distMin = 40000.0F, distTmp;
		for (char radius = 0; radius <= 8; radius++)
		{
			coord = gridXY;
			coord.x -= radius;
			coord.y -= radius;
			char modX = 0, modY = 1, count = 1;
			do
			{
				if (TESObjectREFR *findMarker = markersGrid->Get(coord))
				{
					distTmp = Point3Distance(thisObj->position, findMarker->position);
					if (distMin > distTmp)
					{
						mkRefr = findMarker;
						distMin = distTmp;
					}
				}
				if (!radius) break;
				if (modX) coord.x += modX;
				else coord.y += modY;
				if (count % (radius * 2)) continue;
				modX = modY;
				modY = -!modY;
			}
			while (++count <= (radius * 8));
			if (mkRefr) break;
		}
		if (mkRefr)
		{
			markerData = mkRefr->GetMapMarkerData();
			if (markerData)
			{
				if (distMin > 4096.0F)
				{
					distTmp = (thisObj->position.x - mkRefr->position.x) / distMin;
					distMin = thisObj->position.y - mkRefr->position.y;
					if ((distTmp >= 0.97F) && (distTmp <= 1.0F)) memcpy(locName, "East of ", 9);
					else if ((distTmp >= -1.0F) && (distTmp <= -0.97F)) memcpy(locName, "West of ", 9);
					else if ((distTmp >= -0.26F) && (distTmp <= 0.26F)) StrCopy(locName, (distMin > 0) ? "North of " : "South of ");
					else if (distTmp > 0) StrCopy(locName, (distMin > 0) ? "NE of " : "SE of ");
					else StrCopy(locName, (distMin > 0) ? "NW of " : "SW of ");
				}
				else memcpy(locName, "at ", 4);
				StrCat(locName, markerData->fullName.name.m_data);
				AssignString(PASS_COMMAND_ARGS, locName);
				return true;
			}
		}
	}
	COPY_BYTES(CPY_RET_END(locName, "at ", 3), currentWspc->fullName.name.m_data, currentWspc->fullName.name.m_dataLen + 1);
	AssignString(PASS_COMMAND_ARGS, locName);
	return true;
}

bool Cmd_CCCGetReputation_Execute(COMMAND_ARGS)
{
	if (NOT_ACTOR(thisObj))
		return true;
	auto baseFacIt = ((TESActorBase*)thisObj->baseForm)->baseData.factionList.Head();
	do
	{
		if (FactionListData *facData = baseFacIt->data; facData && facData->faction->reputation)
		{
			REFR_RES = facData->faction->reputation->refID;
			return true;
		}
	}
	while (baseFacIt = baseFacIt->next);
	if (auto xChanges = GetExtraType(&thisObj->extraDataList, ExtraFactionChanges); xChanges && xChanges->data)
	{
		auto refFacIt = xChanges->data->Head();
		do
		{
			if (FactionListData *facData = refFacIt->data; facData && facData->faction->reputation)
			{
				REFR_RES = facData->faction->reputation->refID;
				return true;
			}
		}
		while (refFacIt = refFacIt->next);
	}
	return true;
}

bool Cmd_CCCSayTo_Execute(COMMAND_ARGS)
{
	if NOT_ACTOR(thisObj) return true;
	CdeclCall(0x5C9100, PASS_COMMAND_ARGS);
	if (auto xSayTo = GetExtraType(&thisObj->extraDataList, ExtraSayToTopicInfo))
	{
		if (xSayTo->info)
			xSayTo->info->RunResultScript(1, (Actor*)thisObj);
		thisObj->extraDataList.RemoveByType(kXData_ExtraSayToTopicInfo);
	}
	return true;
}

bool Cmd_CCCRunResultScripts_Execute(COMMAND_ARGS)
{
	TESTopic *topic;
	Actor *actor = (Actor*)thisObj;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &topic) && IS_ACTOR(actor))
		if (auto topicInfo = GetTopicInfo(topic, &topic, actor, g_thePlayer))
		{
			topicInfo->RunResultScript(0, actor);
			topicInfo->RunResultScript(1, actor);
		}
	return true;
}

bool Cmd_CCCSetFollowState_Execute(COMMAND_ARGS)
{
	SInt32 state = -1;
	if (!ExtractArgsEx(EXTRACT_ARGS_EX, &state) || NOT_ACTOR(thisObj)) return true;
	Actor *actor = (Actor*)thisObj;
	if (state < 0)
	{
		if (!(actor->jipActorFlags1 & kHookActorFlag1_PCTeleportAI)) return true;
		actor->jipActorFlags1 &= ~kHookActorFlag1_PCTeleportAI;
		HOOK_DEC(TeleportWithPC);
	}
	else
	{
		UInt8 flag = state ? kHookActorFlag1_PCTeleportFollow : kHookActorFlag1_PCTeleportWait;
		if (actor->jipActorFlags1 & flag) return true;
		if (actor->jipActorFlags1 & kHookActorFlag1_PCTeleportAI)
			actor->jipActorFlags1 ^= kHookActorFlag1_PCTeleportAI;
		else
		{
			actor->jipActorFlags1 |= flag;
			HOOK_INC(TeleportWithPC);
		}
	}
	return true;
}

bool Cmd_RefToPosStr_Execute(COMMAND_ARGS)
{
	char posStr[0x40];
	TESObjectCELL *cell = thisObj->GetParentCell();
	if (cell)
	{
		char *pos = UIntToHex(posStr, cell->worldSpace ? cell->worldSpace->refID : cell->refID);
		*pos++ = ' ';
		pos = IntToStr(pos, int(thisObj->position.x));
		*pos++ = ' ';
		pos = IntToStr(pos, int(thisObj->position.y));
		*pos++ = ' ';
		IntToStr(pos, int(thisObj->position.z) + 1);
	}
	else posStr[0] = 0;
	AssignString(PASS_COMMAND_ARGS, posStr);
	return true;
}

bool Cmd_MoveToPosStr_Execute(COMMAND_ARGS)
{
	char posStr[0x40];
	if (!ExtractArgsEx(EXTRACT_ARGS_EX, &posStr)) return true;
	char *pos = posStr, *delim = GetNextToken(pos, ' ');
	UInt32 refID = HexToUInt(pos);
	if (!refID || !*delim || !GetResolvedRefID(&refID))
		return true;
	TESObjectCELL *cell = (TESObjectCELL*)LookupFormByRefID(refID);
	if (!cell) return true;
	if NOT_ID(cell, TESObjectCELL)
	{
		if NOT_ID(cell, TESWorldSpace)
			return true;
		cell = ((TESWorldSpace*)cell)->cell;
	}
	NiVector3 posVector;
	pos = delim;
	delim = GetNextToken(pos, ' ');
	posVector.x = StrToInt(pos);
	if (!*delim) return true;
	pos = delim;
	delim = GetNextToken(pos, ' ');
	posVector.y = StrToInt(pos);
	if (!*delim) return true;
	posVector.z = StrToInt(delim);
	thisObj->MoveToCell(cell, posVector);
	*result = 1;
	return true;
}

bool Cmd_LockEquipment_Execute(COMMAND_ARGS)
{
	UInt32 lockEqp;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &lockEqp) && IS_TYPE(thisObj, Character) && 
		(!lockEqp != !(((Character*)thisObj)->jipActorFlags1 & kHookActorFlag1_LockedEquipment)))
	{
		((Character*)thisObj)->jipActorFlags1 ^= kHookActorFlag1_LockedEquipment;
		HOOK_MOD(LockEquipment, lockEqp != 0);
	}
	return true;
}

bool Cmd_CCCSetEquipped_Execute(COMMAND_ARGS)
{
	Character *character;
	TESCombatStyle *cmbStyle;
	TESObjectWEAP *weapon;
	ContChangesEntry *entry;
	TESForm *item;
	ExtraDataList *xData = nullptr;
	UInt8 restrictions;
	if (thisObj)
	{
		if NOT_TYPE(thisObj, Character) return true;
		character = (Character*)thisObj;
		InventoryItemsMap *invItemsMap = GetInventoryItemsMap();
		if (!character->GetEquippedWeapon() && GetInventoryItems(character, kFormType_TESObjectWEAP, invItemsMap))
		{
			cmbStyle = character->GetCombatStyle();
			restrictions = cmbStyle ? cmbStyle->weaponRestrictions : 0;
			for (auto wpnIter = invItemsMap->Begin(); wpnIter; ++wpnIter)
			{
				weapon = (TESObjectWEAP*)wpnIter.Key();
				if (weapon->IsPlayable() || (restrictions && ((restrictions == 1) != (weapon->eWeaponType < 3))))
					continue;
				entry = wpnIter().entry;
				if (entry && entry->extendData) xData = entry->extendData->GetFirstItem();
				character->EquipItem(weapon, 1, xData, 1, 0, 0);
				break;
			}
		}
		if (!character->bipedAnims || !GetInventoryItems(character, kFormType_TESObjectARMO, invItemsMap))
			return true;
		UInt32 usedSlots = 0;
		for (BipedAnim::Data &slotData : character->bipedAnims->slotData)
		{
			item = slotData.item;
			if (!item || NOT_TYPE(item, TESObjectARMO))
				continue;
			invItemsMap->Erase(item);
			usedSlots |= ((TESObjectARMO*)item)->bipedModel.partMask;
		}
		TESBipedModelForm *biped;
		for (auto armIter = invItemsMap->Begin(); armIter; ++armIter)
		{
			item = armIter.Key();
			biped = &((TESObjectARMO*)item)->bipedModel;
			if (biped->IsPlayable() || (usedSlots & biped->partMask))
				continue;
			entry = armIter().entry;
			xData = (entry && entry->extendData) ? entry->extendData->GetFirstItem() : nullptr;
			character->EquipItem(item, 1, xData, 1, 0, 0);
			usedSlots |= biped->partMask;
		}
		character->RefreshWornDTDR();
		return true;
	}
	ContainerMenu *containerMenu = ContainerMenu::Get();
	ContChangesEntry *menuEntry = ContainerMenu::Selection();
	if (!menuEntry || !containerMenu || !containerMenu->rightItems.selected || (GetActiveTileID() != 20))
		return true;
	item = menuEntry->type;
	character = (Character*)containerMenu->containerRef;
	if (!item || NOT_TYPE(character, Character))
		return true;
	bool doEquip = true;
	SInt32 eqpCount = 1;
	if (IS_ID(item, TESObjectARMO) || IS_ID(item, TESObjectWEAP))
	{
		if (menuEntry->extendData)
			xData = menuEntry->extendData->GetFirstItem();
		if (xData && xData->HasType(kXData_ExtraWorn))
			doEquip = false;
		if IS_ID(item, TESObjectWEAP)
		{
			if (doEquip)
			{
				weapon = (TESObjectWEAP*)item;
				UInt8 wpnType = weapon->eWeaponType;
				if ((wpnType == 11) || (wpnType == 12) || (weapon->weaponFlags2 & 1))
				{
					*result = -1;
					return true;
				}
				if ((cmbStyle = character->GetCombatStyle()) && (restrictions = cmbStyle->weaponRestrictions) && ((restrictions == 1) != (wpnType < 3)))
				{
					*result = restrictions;
					return true;
				}
				if (wpnType > 9) eqpCount = menuEntry->countDelta;
			}
			else eqpCount = menuEntry->countDelta;
		}
	}
	else if NOT_ID(item, AlchemyItem)
		return true;
	s_forceEquipCall = true;
	ThisCall(doEquip ? 0x88C830 : 0x88D7D0, character, item, eqpCount, xData, 1, 0, 0);
	s_forceEquipCall = false;
	ThisCall(0x8ADED0, g_thePlayer, item, doEquip, doEquip);
	containerMenu->Refresh(item);
	return true;
}

bool Cmd_CCCSMS_Execute(COMMAND_ARGS)
{
	TESObjectREFR *objRefr = nullptr;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &objRefr) && objRefr)
	{
		auto effIter = ProcessManager::Get()->tempEffects.Head();
		do
		{
			if (MagicShaderHitEffect *mshEff = (MagicShaderHitEffect*)effIter->data; mshEff && IS_TYPE(mshEff, MagicShaderHitEffect) &&
				(mshEff->target == objRefr) && (mshEff->effectShader->modIndex == s_CCCModIdx))
				mshEff->flags = 1;
		}
		while (effIter = effIter->next);
	}
	return true;
}

bool Cmd_CCCTaskPackageFlags_Execute(COMMAND_ARGS)
{
	TESPackage *package;
	UInt32 taskType, var1, var2;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &package, &taskType, &var1, &var2))
	{
		if (taskType)
		{
			if (var1)
			{
				package->packageFlags |= (kPackageFlag_NoIdleAnims | kPackageFlag_NoCombatAlert | kPackageFlag_AlwaysWalk);
				package->packageFlags &= ~(kPackageFlag_AlwaysRun | kPackageFlag_WeaponsDrawn | kPackageFlag_ContinueDuringCombat);
			}
			else
			{
				package->packageFlags |= (kPackageFlag_AlwaysRun | kPackageFlag_NoIdleAnims | kPackageFlag_NoCombatAlert);
				package->packageFlags &= ~(kPackageFlag_WeaponsDrawn | kPackageFlag_ContinueDuringCombat | kPackageFlag_AlwaysWalk);
			}
		}
		else if (var1)
		{
			package->packageFlags |= (kPackageFlag_AlwaysRun | kPackageFlag_WeaponsDrawn | kPackageFlag_ContinueDuringCombat);
			package->packageFlags &= ~(kPackageFlag_NoIdleAnims | kPackageFlag_NoCombatAlert | kPackageFlag_AlwaysWalk);
		}
		else
		{
			package->packageFlags |= (kPackageFlag_AlwaysRun | kPackageFlag_WeaponsDrawn);
			package->packageFlags &= ~(kPackageFlag_NoIdleAnims | kPackageFlag_ContinueDuringCombat | kPackageFlag_NoCombatAlert | kPackageFlag_AlwaysWalk);
		}
		if (var2) package->packageFlags |= kPackageFlag_AlwaysSneak;
		else package->packageFlags &= ~kPackageFlag_AlwaysSneak;
	}
	return true;
}