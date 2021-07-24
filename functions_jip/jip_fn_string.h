#pragma once

DEFINE_COMMAND_PLUGIN(sv_RegexMatch, , 0, 22, kParams_JIP_OneInt_OneFormatString);
DEFINE_COMMAND_PLUGIN(sv_RegexSearch, , 0, 22, kParams_JIP_OneInt_OneFormatString);
DEFINE_COMMAND_PLUGIN(sv_RegexReplace, , 0, 22, kParams_JIP_OneInt_OneFormatString);

bool Cmd_sv_RegexMatch_Execute(COMMAND_ARGS)
{
	*result = 0;
	UInt32 strID;
	char *buffer = GetStrArgBuffer();
	if (!ExtractFormatStringArgs(1, buffer, EXTRACT_ARGS_EX, kCommandInfo_sv_RegexMatch.numParams, &strID))
		return true;
	const char *srcStr = GetStringVar(strID);
	if (srcStr) *result = std::regex_match(srcStr, std::regex(buffer));
	return true;
}

bool Cmd_sv_RegexSearch_Execute(COMMAND_ARGS)
{
	*result = 0;
	UInt32 strID;
	char *buffer = GetStrArgBuffer();
	if (!ExtractFormatStringArgs(1, buffer, EXTRACT_ARGS_EX, kCommandInfo_sv_RegexSearch.numParams, &strID))
		return true;
	const char *srcStr = GetStringVar(strID);
	if (!srcStr) return true;
	TempElements *tmpElements = GetTempElements();
	tmpElements->Clear();
	std::regex rgx(buffer);
	std::cmatch matches;
	while (std::regex_search(srcStr, matches, rgx))
	{
		tmpElements->Append(matches.str(0).c_str());
		srcStr = matches.suffix().str().c_str();
	}
	if (!tmpElements->Empty())
		AssignCommandResult(CreateArray(tmpElements->Data(), tmpElements->Size(), scriptObj), result);
	return true;
}

bool Cmd_sv_RegexReplace_Execute(COMMAND_ARGS)
{
	UInt32 strID;
	char *buffer = GetStrArgBuffer();
	if (!ExtractFormatStringArgs(1, buffer, EXTRACT_ARGS_EX, kCommandInfo_sv_RegexReplace.numParams, &strID))
		return true;
	const char *srcStr = GetStringVar(strID);
	char *rgxStr = GetNextToken(buffer, '|');
	if (srcStr && *rgxStr)
		AssignString(PASS_COMMAND_ARGS, std::regex_replace(srcStr, std::regex(rgxStr), buffer).c_str());
	else AssignString(PASS_COMMAND_ARGS, NULL);
	return true;
}