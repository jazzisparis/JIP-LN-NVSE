#pragma once

DEFINE_COMMAND_PLUGIN(sv_RegexMatch, 0, kParams_OneInt_OneFormatString);
DEFINE_COMMAND_PLUGIN(sv_RegexSearch, 0, kParams_OneInt_OneFormatString);
DEFINE_COMMAND_PLUGIN(sv_RegexReplace, 0, kParams_OneInt_OneFormatString);
DEFINE_COMMAND_ALT_PLUGIN(GetStringHash, GetHash, 0, kParams_OneString_TwoOptionalInts);

bool Cmd_sv_RegexMatch_Execute(COMMAND_ARGS)
{
	UInt32 strID;
	char rgxStr[0x80];
	if (ExtractFormatStringArgs(1, rgxStr, EXTRACT_ARGS_EX, kCommandInfo_sv_RegexMatch.numParams, &strID))
		if (const char *srcStr = GetStringVar(strID); srcStr && std::regex_match(srcStr, std::regex(rgxStr)))
			*result = 1;
	return true;
}

bool Cmd_sv_RegexSearch_Execute(COMMAND_ARGS)
{
	NVSEArrayVar *resArr = CreateArray(NULL, 0, scriptObj);
	*result = (int)resArr;
	UInt32 strID;
	char rgxStr[0x80];
	if (ExtractFormatStringArgs(1, rgxStr, EXTRACT_ARGS_EX, kCommandInfo_sv_RegexSearch.numParams, &strID))
		if (const char *srcStr = GetStringVar(strID), *pEnd = srcStr + StrLen(srcStr); srcStr != pEnd)
		{
			std::cmatch matches;
			for (std::regex rgx(rgxStr); std::regex_search(srcStr, matches, rgx); srcStr = pEnd - matches.suffix().str().size())
				AppendElement(resArr, ArrayElementL(matches.str().c_str()));
		}
	return true;
}

bool Cmd_sv_RegexReplace_Execute(COMMAND_ARGS)
{
	UInt32 strID;
	char *buffer = GetStrArgBuffer();
	if (ExtractFormatStringArgs(1, buffer, EXTRACT_ARGS_EX, kCommandInfo_sv_RegexReplace.numParams, &strID))
		if (const char *srcStr = GetStringVar(strID))
			if (char *rgxStr = GetNextToken(buffer, '|'); rgxStr && *rgxStr)
			{
				AssignString(PASS_COMMAND_ARGS, std::regex_replace(srcStr, std::regex(rgxStr), buffer).c_str());
				return true;
			}
	AssignString(PASS_COMMAND_ARGS, nullptr);
	return true;
}

bool Cmd_GetStringHash_Execute(COMMAND_ARGS)
{
	char *buffer = GetStrArgBuffer();
	UInt32 split = 0, useCase = 0;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, buffer, &split, &useCase) && *buffer)
	{
		auto StrHash = !useCase ? StrHashCI : StrHashCS;
		if (!split)
		{
			*result = (SInt32)StrHash(buffer);
			DoConsolePrint(result);
		}
		else if (FILE *theFile = fopen("GetStringHash.txt", "wb"))
		{
			while (true)
			{
				char *barPtr = GetNextToken(buffer, '|');
				fprintf(theFile, "\"%s\"\n%d\n\n", buffer, StrHash(buffer));
				if (!*barPtr) break;
				buffer = barPtr;
			}
			fclose(theFile);
		}
	}
	return true;
}