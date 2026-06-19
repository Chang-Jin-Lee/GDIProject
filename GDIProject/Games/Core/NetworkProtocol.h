#pragma once

#include <map>
#include <string>

struct NetworkLine
{
	bool Valid = false;
	std::string Name;
	std::map<std::string, std::string> Fields;
	std::string Error;
};

NetworkLine ParseNetworkLine(const std::string& line);
std::string BuildNetworkLine(const std::string& name, const std::map<std::string, std::string>& fields);
