#pragma once

DEFINE_COMMAND_PLUGIN(sv_RegexMatch, , 0, 22, kParams_JIP_OneInt_OneFormatString);
DEFINE_COMMAND_PLUGIN(sv_RegexSearch, , 0, 22, kParams_JIP_OneInt_OneFormatString);
DEFINE_COMMAND_PLUGIN(sv_RegexReplace, , 0, 22, kParams_JIP_OneInt_OneFormatString);

bool Cmd_sv_RegexMatch_Execute(COMMAND_ARGS)
{
	*result = 0;
	UInt32 strID;
	if (!ExtractFormatStringArgs(1, s_strArgBuffer, EXTRACT_ARGS_EX, kCommandInfo_sv_RegexMatch.numParams, &strID))
		return true;
	const char *srcStr = GetStringVar(strID);
	if (srcStr) *result = std::regex_match(srcStr, std::regex(s_strArgBuffer));
	return true;
}

bool Cmd_sv_RegexSearch_Execute(COMMAND_ARGS)
{
	*result = 0;
	UInt32 strID;
	if (!ExtractFormatStringArgs(1, s_strArgBuffer, EXTRACT_ARGS_EX, kCommandInfo_sv_RegexSearch.numParams, &strID))
		return true;
	const char *srcStr = GetStringVar(strID);
	if (!srcStr) return true;
	s_tempElements.Clear();
	std::regex rgx(s_strArgBuffer);
	std::cmatch matches;
	while (std::regex_search(srcStr, matches, rgx))
	{
		s_tempElements.Append(matches.str(0).c_str());
		srcStr = matches.suffix().str().c_str();
	}
	if (!s_tempElements.Empty())
		AssignCommandResult(CreateArray(s_tempElements.Data(), s_tempElements.Size(), scriptObj), result);
	return true;
}

bool Cmd_sv_RegexReplace_Execute(COMMAND_ARGS)
{
	UInt32 strID;
	if (!ExtractFormatStringArgs(1, s_strArgBuffer, EXTRACT_ARGS_EX, kCommandInfo_sv_RegexReplace.numParams, &strID))
		return true;
	const char *srcStr = GetStringVar(strID);
	char *rgxStr = GetNextToken(s_strArgBuffer, '|');
	if (srcStr && *rgxStr)
		AssignString(PASS_COMMAND_ARGS, std::regex_replace(srcStr, std::regex(rgxStr), s_strArgBuffer).c_str());
	else AssignString(PASS_COMMAND_ARGS, NULL);
	return true;
}