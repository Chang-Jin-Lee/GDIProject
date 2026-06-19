#pragma once

#include "GameTypes.h"

#include <string>

struct MatchSettings
{
	EMatchMode Mode = EMatchMode::SinglePlayer;
	std::string RemoteIp = "127.0.0.1";
	int Port = 43117;
	bool HostWaiting = false;
};

MatchSettings& GetMutableMatchSettings();
const MatchSettings& GetMatchSettings();
void ResetMatchSettings();
