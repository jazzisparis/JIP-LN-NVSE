#pragma once

DEFINE_COMMAND_PLUGIN(sv_RegexMatch, 0, 22, kParams_OneInt_OneFormatString);
DEFINE_COMMAND_PLUGIN(sv_RegexSearch, 0, 22, kParams_OneInt_OneFormatString);
DEFINE_COMMAND_PLUGIN(sv_RegexReplace, 0, 22, kParams_OneInt_OneFormatString);

bool Cmd_sv_RegexMatch_Execute(COMMAND_ARGS)
{
	*result = 0;
	UInt32 strID;
	char rgxStr[0x80];
	if (ExtractFormatStringArgs(1, rgxStr, EXTRACT_ARGS_EX, kCommandInfo_sv_RegexMatch.numParams, &strID))
		if (const char *srcStr = GetStringVar(strID))
			*result = std::regex_match(srcStr, std::regex(rgxStr));
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