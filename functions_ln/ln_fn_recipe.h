#pragma once

DEFINE_COMMAND_PLUGIN(GetFormRecipes, , 0, 2, kParams_JIP_OneForm_OneOptionalForm);
DEFINE_COMMAND_PLUGIN(GetFormRecipeOutputs, , 0, 2, kParams_JIP_OneForm_OneOptionalForm);
DEFINE_COMMAND_PLUGIN(GetRecipeRequiredSkill, , 0, 1, kParams_OneForm);
DEFINE_COMMAND_PLUGIN(SetRecipeRequiredSkill, , 0, 2, kParams_OneForm_OneInt);
DEFINE_COMMAND_PLUGIN(GetRecipeRequiredSkillLevel, , 0, 1, kParams_OneForm);
DEFINE_COMMAND_PLUGIN(SetRecipeRequiredSkillLevel, , 0, 2, kParams_OneForm_OneInt);
DEFINE_COMMAND_PLUGIN(GetRecipeInputForms, , 0, 1, kParams_OneForm);
DEFINE_COMMAND_PLUGIN(GetRecipeOutputForms, , 0, 1, kParams_OneForm);
DEFINE_COMMAND_PLUGIN(AddRecipeInputForm, , 0, 3, kParams_JIP_TwoForms_OneInt);
DEFINE_COMMAND_PLUGIN(AddRecipeOutputForm, , 0, 3, kParams_JIP_TwoForms_OneInt);
DEFINE_COMMAND_PLUGIN(RemoveRecipeInputForm, , 0, 2, kParams_JIP_TwoForms);
DEFINE_COMMAND_PLUGIN(RemoveRecipeOutputForm, , 0, 2, kParams_JIP_TwoForms);
DEFINE_COMMAND_PLUGIN(ReplaceRecipeInputForm, , 0, 3, kParams_JIP_ThreeForms);
DEFINE_COMMAND_PLUGIN(ReplaceRecipeOutputForm, , 0, 3, kParams_JIP_ThreeForms);
DEFINE_COMMAND_PLUGIN(GetRecipeInputCount, , 0, 2, kParams_JIP_TwoForms);
DEFINE_COMMAND_PLUGIN(SetRecipeInputCount, , 0, 3, kParams_JIP_TwoForms_OneInt);
DEFINE_COMMAND_PLUGIN(GetRecipeOutputCount, , 0, 2, kParams_JIP_TwoForms);
DEFINE_COMMAND_PLUGIN(SetRecipeOutputCount, , 0, 3, kParams_JIP_TwoForms_OneInt);
DEFINE_COMMAND_PLUGIN(GetRecipeCategory, , 0, 1, kParams_OneForm);
DEFINE_COMMAND_PLUGIN(SetRecipeCategory, , 0, 2, kParams_JIP_TwoForms);
DEFINE_COMMAND_PLUGIN(GetRecipeSubcategory, , 0, 1, kParams_OneForm);
DEFINE_COMMAND_PLUGIN(SetRecipeSubcategory, , 0, 2, kParams_JIP_TwoForms);
DEFINE_COMMAND_PLUGIN(AddRecipeCondition, , 0, 5, kParams_FormCondition);

bool Cmd_GetFormRecipes_Execute(COMMAND_ARGS)
{
	TESForm *form, *filter = NULL;
	if (!ExtractArgsEx(EXTRACT_ARGS_EX, &form, &filter)) return true;
	TempElements *tmpElements = GetTempElements();
	tmpElements->Clear();
	if (filter && NOT_ID(filter, TESRecipeCategory)) filter = NULL;
	ListNode<TESRecipe> *rcpeIter = g_dataHandler->recipeList.Head();
	TESRecipe *recipe;
	ListNode<RecipeComponent> *entryIter;
	RecipeComponent *component;
	do
	{
		recipe = rcpeIter->data;
		if (!recipe || (filter && (recipe->category != filter) && (recipe->subCategory != filter))) continue;
		entryIter = recipe->inputs.Head();
		do
		{
			component = entryIter->data;
			if (component && (component->item == form))
				tmpElements->Append(recipe);
		}
		while (entryIter = entryIter->next);
	}
	while (rcpeIter = rcpeIter->next);
	AssignCommandResult(CreateArray(tmpElements->Data(), tmpElements->Size(), scriptObj), result);
	return true;
}

bool Cmd_GetFormRecipeOutputs_Execute(COMMAND_ARGS)
{
	TESForm *form, *filter;
	if (!ExtractArgsEx(EXTRACT_ARGS_EX, &form, &filter))
		return true;
	TempFormList *tmpFormLst = GetTempFormList();
	tmpFormLst->Clear();
	if (filter && NOT_ID(filter, TESRecipeCategory)) filter = NULL;
	ListNode<TESRecipe> *rcpeIter = g_dataHandler->recipeList.Head();
	TESRecipe *recipe;
	ListNode<RecipeComponent> *inputIter, *outputIter;
	RecipeComponent *component;
	do
	{
		recipe = rcpeIter->data;
		if (!recipe || (filter && (recipe->category != filter) && (recipe->subCategory != filter))) continue;
		inputIter = recipe->inputs.Head();
		do
		{
			component = inputIter->data;
			if (!component || (component->item != form)) continue;
			outputIter = recipe->outputs.Head();
			do
			{
				if (component = outputIter->data)
					tmpFormLst->Insert(component->item);
			}
			while (outputIter = outputIter->next);
		}
		while (inputIter = inputIter->next);
	}
	while (rcpeIter = rcpeIter->next);
	if (!tmpFormLst->Empty())
	{
		TempElements *tmpElements = GetTempElements();
		tmpElements->Clear();
		for (auto refIter = tmpFormLst->Begin(); refIter; ++refIter)
			tmpElements->Append(*refIter);
		AssignCommandResult(CreateArray(tmpElements->Data(), tmpElements->Size(), scriptObj), result);
	}
	return true;
}

bool Cmd_GetRecipeRequiredSkill_Execute(COMMAND_ARGS)
{
	TESRecipe *recipe;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &recipe) && IS_ID(recipe, TESRecipe))
		*result = (int)recipe->reqSkill;
	else *result = -1;
	return true;
}

bool Cmd_SetRecipeRequiredSkill_Execute(COMMAND_ARGS)
{
	TESRecipe *recipe;
	UInt32 skill;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &recipe, &skill) && IS_ID(recipe, TESRecipe))
		recipe->reqSkill = skill;
	return true;
}

bool Cmd_GetRecipeRequiredSkillLevel_Execute(COMMAND_ARGS)
{
	TESRecipe *recipe;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &recipe) && IS_ID(recipe, TESRecipe))
		*result = (int)recipe->reqSkillLevel;
	else *result = -1;
	return true;
}

bool Cmd_SetRecipeRequiredSkillLevel_Execute(COMMAND_ARGS)
{
	TESRecipe *recipe;
	UInt32 skillLvl;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &recipe, &skillLvl) && IS_ID(recipe, TESRecipe))
		recipe->reqSkillLevel = skillLvl;
	return true;
}

bool Cmd_GetRecipeInputForms_Execute(COMMAND_ARGS)
{
	TESRecipe *recipe;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &recipe) && IS_ID(recipe, TESRecipe))
		AssignCommandResult((NVSEArrayVar*)recipe->inputs.GetComponents(scriptObj), result);
	else *result = 0;
	return true;
}

bool Cmd_GetRecipeOutputForms_Execute(COMMAND_ARGS)
{
	TESRecipe *recipe;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &recipe) && IS_ID(recipe, TESRecipe))
		AssignCommandResult((NVSEArrayVar*)recipe->outputs.GetComponents(scriptObj), result);
	else *result = 0;
	return true;
}

bool Cmd_AddRecipeInputForm_Execute(COMMAND_ARGS)
{
	TESRecipe *recipe;
	TESForm *form;
	UInt32 count;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &recipe, &form, &count) && IS_ID(recipe, TESRecipe))
		recipe->inputs.AddComponent(form, count);
	return true;
}

bool Cmd_AddRecipeOutputForm_Execute(COMMAND_ARGS)
{
	TESRecipe *recipe;
	TESForm *form;
	UInt32 count;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &recipe, &form, &count) && IS_ID(recipe, TESRecipe))
		recipe->outputs.AddComponent(form, count);
	return true;
}

bool Cmd_RemoveRecipeInputForm_Execute(COMMAND_ARGS)
{
	TESRecipe *recipe;
	TESForm *form;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &recipe, &form) && IS_ID(recipe, TESRecipe))
		*result = (int)recipe->inputs.RemoveComponent(form);
	else *result = 0;
	return true;
}

bool Cmd_RemoveRecipeOutputForm_Execute(COMMAND_ARGS)
{
	TESRecipe *recipe;
	TESForm *form;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &recipe, &form) && IS_ID(recipe, TESRecipe))
		*result = (int)recipe->outputs.RemoveComponent(form);
	else *result = 0;
	return true;
}

bool Cmd_ReplaceRecipeInputForm_Execute(COMMAND_ARGS)
{
	TESRecipe *recipe;
	TESForm *form, *replace;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &recipe, &form, &replace) && IS_ID(recipe, TESRecipe))
		recipe->inputs.ReplaceComponent(form, replace);
	return true;
}

bool Cmd_ReplaceRecipeOutputForm_Execute(COMMAND_ARGS)
{
	TESRecipe *recipe;
	TESForm *form, *replace;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &recipe, &form, &replace) && IS_ID(recipe, TESRecipe))
		recipe->outputs.ReplaceComponent(form, replace);
	return true;
}

bool Cmd_GetRecipeInputCount_Execute(COMMAND_ARGS)
{
	*result = 0;
	TESRecipe *recipe;
	TESForm *form;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &recipe, &form) && IS_ID(recipe, TESRecipe))
		*result = (int)recipe->inputs.GetQuantity(form);
	else *result = 0;
	return true;
}

bool Cmd_SetRecipeInputCount_Execute(COMMAND_ARGS)
{
	TESRecipe *recipe;
	TESForm *form;
	UInt32 count;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &recipe, &form, &count) && IS_ID(recipe, TESRecipe))
		recipe->inputs.SetQuantity(form, count);
	return true;
}

bool Cmd_GetRecipeOutputCount_Execute(COMMAND_ARGS)
{
	TESRecipe *recipe;
	TESForm *form;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &recipe, &form) && IS_ID(recipe, TESRecipe))
		*result = (int)recipe->outputs.GetQuantity(form);
	else *result = 0;
	return true;
}

bool Cmd_SetRecipeOutputCount_Execute(COMMAND_ARGS)
{
	TESRecipe *recipe;
	TESForm *form;
	UInt32 count;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &recipe, &form, &count) && IS_ID(recipe, TESRecipe))
		recipe->outputs.SetQuantity(form, count);
	return true;
}

bool Cmd_GetRecipeCategory_Execute(COMMAND_ARGS)
{
	*result = 0;
	TESRecipe *recipe;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &recipe) && IS_ID(recipe, TESRecipe) && recipe->category)
		REFR_RES = recipe->category->refID;
	return true;
}

bool Cmd_SetRecipeCategory_Execute(COMMAND_ARGS)
{
	TESRecipe *recipe;
	TESRecipeCategory *category;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &recipe, &category) && IS_ID(recipe, TESRecipe) && IS_ID(category, TESRecipeCategory))
	{
		recipe->category = category;
		recipe->categoryID = category->refID;
	}
	return true;
}

bool Cmd_GetRecipeSubcategory_Execute(COMMAND_ARGS)
{
	*result = 0;
	TESRecipe *recipe;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &recipe) && IS_ID(recipe, TESRecipe) && recipe->subCategory)
		REFR_RES = recipe->subCategory->refID;
	return true;
}

bool Cmd_SetRecipeSubcategory_Execute(COMMAND_ARGS)
{
	TESRecipe *recipe;
	TESRecipeCategory *category;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &recipe, &category) && IS_ID(recipe, TESRecipe) && IS_ID(category, TESRecipeCategory))
	{
		recipe->subCategory = category;
		recipe->subCategoryID = category->refID;
	}
	return true;
}

bool Cmd_AddRecipeCondition_Execute(COMMAND_ARGS)
{
	TESRecipe *recipe;
	TESForm *param;
	UInt32 func, comp;
	float value;
	if (!ExtractArgsEx(EXTRACT_ARGS_EX, &recipe, &func, &param, &comp, &value) || NOT_ID(recipe, TESRecipe)) return true;
	Condition *condition = (Condition*)GameHeapAlloc(sizeof(Condition));
	condition->runOnType = 0;
	condition->opcode = func;
	condition->parameter1.form = param;
	condition->type = comp;
	condition->comparisonValue.value = value;
	recipe->conditions.Append(condition);
	return true;
}