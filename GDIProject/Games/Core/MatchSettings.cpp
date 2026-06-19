#include "MatchSettings.h"

namespace
{
MatchSettings g_matchSettings;
}

MatchSettings& GetMutableMatchSettings()
{
	return g_matchSettings;
}

const MatchSettings& GetMatchSettings()
{
	return g_matchSettings;
}

void ResetMatchSettings()
{
	g_matchSettings = MatchSettings{};
}
