#pragma once

// Declare Params with "static", so they are only built once (so this file can be included multiple times).

#include "nvse/CommandTable.h"
#include "nvse/PluginAPI.h"

static ParamInfo kNVSEParams_OneArray[1] =
{
	{	"array",	kNVSEParamType_Array,	0	},
};
static ParamInfo kNVSEParams_OneOptionalArray[1] =
{
	{	"array",	kNVSEParamType_Array,	1	},
};

static ParamInfo kNVSEParams_OneForm_OneArray_OneOptionalInt[3] =
{
	{	"form",	kNVSEParamType_Form,	0	},
	{	"array",	kNVSEParamType_Array,	0	},
	{	"int",	kNVSEParamType_Number,1	},
};
static ParamInfo kNVSEParams_OneArray_OneString_OneArray[3] =
{
	{	"array",	kNVSEParamType_Array,	0	},
	{	"string",	kNVSEParamType_String,0	},
	{	"array",	kNVSEParamType_Array,	0	},
};
static ParamInfo kNVSEParams_OneArray_OneString_OneDouble[3] =
{
	{	"array",	kNVSEParamType_Array,	0	},
	{	"string",	kNVSEParamType_String,0	},
	{	"float",	kNVSEParamType_Number,0	},
};
static ParamInfo  kNVSEParams_OneString_OneInt_OneArray[3] =
{
	{	"string",	kNVSEParamType_String,0	},
	{	"int",	kNVSEParamType_Number,0	},
	{	"array",	kNVSEParamType_Array,	0	},
};
static ParamInfo  kNVSEParams_OneString_OneArray_OneOptionalString[3] =
{
	{	"string",	kNVSEParamType_String,0	},
	{	"array",	kNVSEParamType_Array,	0	},
	{	"string",	kNVSEParamType_String,1	},
};
static ParamInfo  kNVSEParams_OneString_OneArray_OneOptionalForm[3] =
{
	{	"string",	kNVSEParamType_String,0	},
	{	"array",	kNVSEParamType_Array,	0	},
	{	"form",	kNVSEParamType_Form,1	},
};


static ParamInfo kNVSEParams_OneForm_OneArray[2] =
{
	{	"form",	kNVSEParamType_Form,	0	},
	{	"array",	kNVSEParamType_Array,	0	},
};

static ParamInfo kNVSEParams_OneArray_TwoOptionalBools[3] =
{
	{	"array",	kNVSEParamType_Array,	0	},
	{	"bool",	kNVSEParamType_Boolean,1	},
	{	"bool",	kNVSEParamType_Boolean,1	},
};

static ParamInfo kNVSEParams_TwoArrays[2] =
{
	{	"array",	kNVSEParamType_Array,	0	},
	{	"array",	kNVSEParamType_Array,	0	},
};

static ParamInfo kNVSEParams_TwoArrays_OneOptionalBool[3] =
{
	{	"array",	kNVSEParamType_Array,	0	},
	{	"array",	kNVSEParamType_Array,	0	},
	{	"bool",	kNVSEParamType_Boolean,1	},
};

static ParamInfo kNVSEParams_OneArray_OneInt[2] =
{
	{	"array",	kNVSEParamType_Array,	0	},
	{	"int",	kNVSEParamType_Number,0	},
};



